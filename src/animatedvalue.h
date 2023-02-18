#pragma once

#include <functional>
#include <string>
struct AnimatedValue
{
    double startValue, endValue;

    std::function<void(AnimatedValue *sender, double tNorm, double value)> onValueChanged;

    std::string description = "";

    std::function<double(double start, double end, double tNorm)> easingFunction = Linear;

    static double Linear(double start, double end, double tNorm)
    {
        return start + (end - start) * tNorm;
    }
};