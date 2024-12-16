#ifndef _AVERAGE_2_H
#define _AVERAGE_2_H

#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <concepts>

template <typename T>
concept arithmetic = std::integral<T> or std::floating_point<T>;

template <typename T>
    requires arithmetic<T>
struct Average2
{
private:
    T oldest;
    T latest;
    uint8_t _count;

public:
    Average2() : oldest(0), latest(0), _count(0) {}
    Average2(T value) : oldest(value), latest(value), _count(2) {}
    Average2(T oldest, T latest) : oldest(oldest), latest(latest), _count(2) {}

    T get() const
    {
        if (_count < 2)
            return latest;

        return (oldest + latest) / 2;
    }

    uint8_t count() const
    {
        return _count;
    }

    void update(T value)
    {
        oldest = latest;
        latest = value;

        if (_count < 2)
            _count++;
    }

    T updateAndGet(T value)
    {
        update(value);
        return get();
    }
};

#endif