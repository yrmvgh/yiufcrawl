/**
 * @file
 * @brief Stepdown functions.
**/

#ifndef STEPDOWN_H
#define STEPDOWN_H

// stepdowns

enum rounding_type
{
    ROUND_DOWN,
    ROUND_CLOSE,
    ROUND_RANDOM
};

int stepup(int value, int multiplier = 1, int power = 2, int divisor = 1);
double stepdown(double value, double step);
int stepdown(int value, int step, rounding_type = ROUND_CLOSE, int max = 0);
int stepdown_value(int base_value, int stepping, int first_step,
                   int /*last_step*/, int ceiling_value);

#endif
