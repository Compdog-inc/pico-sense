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
#include "distance.h"

using namespace std::literals;

static Status *led;

static void main_task(__unused void *params)
{
    led = new Status();

    led->setPattern(Pattern::Pulse);

    DistanceSensor *sensor0 = new DistanceSensor(Config::Distance::SENSOR_0);
    DistanceSensor *sensor1 = new DistanceSensor(Config::Distance::SENSOR_1);
    DistanceSensor *sensor2 = new DistanceSensor(Config::Distance::SENSOR_2);
    DistanceSensor *sensor3 = new DistanceSensor(Config::Distance::SENSOR_3);
    DistanceSensor *sensor4 = new DistanceSensor(Config::Distance::SENSOR_4);
    DistanceSensor *sensor5 = new DistanceSensor(Config::Distance::SENSOR_5);

    while (true)
    {
        float distance0 = sensor0->getDistance();
        float distance5 = sensor5->getDistance();
        float distance1 = sensor1->getDistance();
        float distance4 = sensor4->getDistance();
        float distance2 = sensor2->getDistance();
        float distance3 = sensor3->getDistance();

        printf("Distance: %.2f cm, %.2f cm, %.2f cm, %.2f cm, %.2f cm, %.2f cm\n", distance0, distance5, distance1, distance4, distance2, distance3);
        vTaskDelay(pdMS_TO_TICKS(1));
    }

    delete sensor0;
    delete sensor1;
    delete sensor2;
    delete sensor3;
    delete sensor4;
    delete sensor5;

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