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

    static double EaseInQuad(double start, double end, double tNorm)
    {
        return start + (end - start) * tNorm * tNorm;
    }

    static double EaseOutQuad(double start, double end, double tNorm)
    {
        return start + (end - start) * (1 - (1 - tNorm) * (1 - tNorm));
    }

    static double EaseInOutQuad(double start, double end, double tNorm)
    {
        if (tNorm < 0.5)
            return start + (end - start) * 2 * tNorm * tNorm;
        else
            return start + (end - start) * (1 - 2 * (1 - tNorm) * (1 - tNorm));
    }

    static double EaseInCubic(double start, double end, double tNorm)
    {
        return start + (end - start) * tNorm * tNorm * tNorm;
    }

    static double EaseOutCubic(double start, double end, double tNorm)
    {
        return start + (end - start) * (1 - (1 - tNorm) * (1 - tNorm) * (1 - tNorm));
    }

    static double EaseInOutCubic(double start, double end, double tNorm)
    {
        if (tNorm < 0.5)
            return start + (end - start) * 4 * tNorm * tNorm * tNorm;
        else
            return start + (end - start) * (1 - 4 * (1 - tNorm) * (1 - tNorm) * (1 - tNorm));
    }
};