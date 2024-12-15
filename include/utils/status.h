#ifndef _STATUS_H
#define _STATUS_H

#include <stdlib.h>
#include <pico/stdlib.h>
#include <FreeRTOS.h>
#include <task.h>

enum class Pattern
{
    Off,
    On,
    Pulse,
    Blink,
    Alt1,
    Alt2,
};

class Status
{
public:
    Status();
    ~Status();

    void setPattern(Pattern pattern);

    Pattern getPattern()
    {
        return pattern;
    }

    bool isAnimationRunning()
    {
        return animationRunning;
    }

private:
    TaskHandle_t animationTask;
    bool animationRunning;
    Pattern pattern;
};

#endif