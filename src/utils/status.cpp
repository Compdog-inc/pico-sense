// Standard headers
#include <stdlib.h>
#include <math.h>

// Kernel headers
#include <FreeRTOS.h>
#include <task.h>

// Hardware headers
#include <pico/stdlib.h>
#include <hardware/pwm.h>

// Config headers
#include "config/options.h"

#include "utils/status.h"

void pwm_set_squared(uint gpio, uint16_t level)
{
    uint16_t squared = ((level >> 8) + 1) * ((level >> 8) + 1) - 1;
    pwm_set_gpio_level(gpio, squared);
}

void apply_pattern(uint gpio, Pattern pattern)
{
    switch (pattern)
    {
    case Pattern::Off:
    {
        pwm_set_squared(gpio, 0);
    }
    break;
    case Pattern::On:
    {
        pwm_set_squared(gpio, 0xFFFF);
    }
    break;
    case Pattern::Pulse:
    {
        uint32_t cycleTime = time_us_32() % Config::Lights::PULSE_LENGTH;
        if (cycleTime < Config::Lights::PULSE_HALF_LENGTH)
            pwm_set_squared(gpio, cycleTime / Config::Lights::PULSE_DIVISOR);
        else
            pwm_set_squared(gpio, (Config::Lights::PULSE_LENGTH - cycleTime) / Config::Lights::PULSE_DIVISOR);
    }
    break;
    case Pattern::Blink:
    {
        uint32_t cycleTime = time_us_32() % Config::Lights::BLINK_LENGTH;
        if (cycleTime < Config::Lights::BLINK_ON_LENGTH)
            pwm_set_squared(gpio, 0xFFFF);
        else
            pwm_set_squared(gpio, 0);
    }
    break;
    case Pattern::Alt1:
    {
        uint32_t cycleTime = time_us_32() % Config::Lights::PULSE_LENGTH;
        if (cycleTime < Config::Lights::PULSE_HALF_LENGTH)
            pwm_set_squared(gpio, cycleTime / Config::Lights::PULSE_DIVISOR);
        else
            pwm_set_squared(gpio, (Config::Lights::PULSE_LENGTH - cycleTime) / Config::Lights::PULSE_DIVISOR);
    }
    break;
    case Pattern::Alt2:
    {
        uint32_t cycleTime = time_us_32() % Config::Lights::PULSE_LENGTH;
        if (cycleTime < Config::Lights::PULSE_HALF_LENGTH)
            pwm_set_squared(gpio, 62500 - cycleTime / Config::Lights::PULSE_DIVISOR);
        else
            pwm_set_squared(gpio, 62500 - (Config::Lights::PULSE_LENGTH - cycleTime) / Config::Lights::PULSE_DIVISOR);
    }
    break;
    }
}

void animation_task(void *pv_lights)
{
    Status *lights = (Status *)pv_lights;

    while (lights->isAnimationRunning())
    {
        apply_pattern(PICO_DEFAULT_LED_PIN, lights->getPattern());
        vTaskDelay(pdMS_TO_TICKS(2));
    }

    vTaskDelete(NULL);
}

Status::Status() : animationRunning(true)
{
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, true);
    gpio_set_function(PICO_DEFAULT_LED_PIN, GPIO_FUNC_PWM);

    uint slice_num = pwm_gpio_to_slice_num(PICO_DEFAULT_LED_PIN);

    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 4.f);
    pwm_init(slice_num, &config, true);

    xTaskCreate(animation_task, "LightAnimationThread", configMINIMAL_STACK_SIZE, this, (tskIDLE_PRIORITY + 2UL), &animationTask);
}

Status::~Status()
{
    animationRunning = false;
}

void Status::setPattern(Pattern pattern)
{
    this->pattern = pattern;
}