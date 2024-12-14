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

using namespace std::literals;

static void main_task(__unused void *params)
{
    while (true)
    {
        vTaskDelay(pdMS_TO_TICKS(500));
        printf("[MAIN] Main update!\n");
    }

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