// Standard headers
#include <stdlib.h>
#include <string>
#include <cstdarg>
#include <cstring>

// Kernel headers
#include <FreeRTOS.h>
#include <task.h>

// Config headers
#include "config/timers.h"

// Hardware headers
#include <pico/stdlib.h>
#include <pico/time.h>

#include "config/options.h"
#include "utils/status.h"

// Sensors
#include "distance4.h"

#include "utils/average2.hpp"

using namespace std::literals;

static Status *led;

static void main_task(__unused void *params)
{
    led = new Status();

    led->setPattern(Pattern::Pulse);

    DistanceSensor4 *sensor0523 = new DistanceSensor4(Config::Distance::SENSOR_0, Config::Distance::SENSOR_5, Config::Distance::SENSOR_2, Config::Distance::SENSOR_3);
    DistanceSensor4 *sensor14 = new DistanceSensor4(Config::Distance::SENSOR_1, Config::Distance::SENSOR_4);

    Average2<float> average0{};
    Average2<float> average1{};
    Average2<float> average2{};
    Average2<float> average3{};
    Average2<float> average4{};
    Average2<float> average5{};

    while (true)
    {
        absolute_time_t start = get_absolute_time();
        const auto [distance0, distance5, distance2, distance3] = sensor0523->getDistance();
        const auto [distance1, distance4, _0, _1] = sensor14->getDistance();

        printf("Distance: %.2f cm, %.2f cm, %.2f cm, %.2f cm, %.2f cm, %.2f cm\n",
               average0.updateAndGet(distance0),
               average5.updateAndGet(distance5),
               average1.updateAndGet(distance1),
               average4.updateAndGet(distance4),
               average2.updateAndGet(distance2),
               average3.updateAndGet(distance3));
        absolute_time_t end = get_absolute_time();

        printf("Time: %lld us\n", absolute_time_diff_us(start, end));
        vTaskDelay(pdMS_TO_TICKS(1));
    }

    delete sensor0523;
    delete sensor14;

    delete led;

    vTaskDelete(NULL);
}

int main()
{
    stdio_init_all();
    sleep_us(64);

    Config::init_timers();

    printf("[BOOT] Creating MainThread task\n");
    TaskHandle_t task;
    xTaskCreate(main_task, "MainThread", configMAIN_THREAD_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 4UL), &task);

    printf("[BOOT] Starting task scheduler\n");
    vTaskStartScheduler();

    Config::deinit_timers();
    return 0;
}

extern "C" void rtos_panic(const char *fmt, ...)
{
    puts("\n*** PANIC ***\n");
    if (fmt)
    {
        va_list args;
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
        puts("\n");
    }

    exit(1);
}

extern "C" void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    panic("vApplicationStackOverflowHook called (%s)", pcTaskName);
}