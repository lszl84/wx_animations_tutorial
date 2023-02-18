#pragma once

#include <functional>

struct AnimatedValue
{
    double startValue, endValue;

    std::function<void(AnimatedValue *sender, double tNorm, double value)> onValueChanged;
};