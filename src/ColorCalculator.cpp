#include "ColorCalculator.h"
#include <qmath.h>
float CalculateValue(float *array, float value)
{

    int downV = int(value);

    if (downV*1.0 == value) return array[downV];

    int upV = downV + 1;
    float scale = value - downV;

    return array[downV] + scale*(array[upV] - array[downV]);
}

void GetMushroomColor(float state, float *color)
{

    float RedMushColor[]   = { 32,  32,   0, 64, 96,  32, 0, 0, 0, 0};
    float GreenMushColor[] = { 32,  64, 128, 64, 32,  32, 0, 0, 0, 0};
    float BlueMushColor[]  = { 32,   0,   0,   0, 0,  0, 0, 0, 0, 0};
    float AlphaMushColor[] = { 1,  1,   1,   1,   1,  1,  1, 1, 1, 1};

    float R = CalculateValue(RedMushColor,   6 + state);
    float G = CalculateValue(GreenMushColor, 6 + state);
    float B = CalculateValue(BlueMushColor,  6 + state);
    float A = CalculateValue(AlphaMushColor, 6 + state);

    color[0] = R;
    color[1] = G;
    color[2] = B;
    color[3] = A;
}

float GetMushroomScale(float state)
{
    float Scales[] = {0, 0.25, 0.50, 0.75, 1, 0.5, 0, 0, 0, 0};
    if (state < 0 && state > -1) return CalculateValue(Scales, 10*state + 6);
    return CalculateValue(Scales, state + 6);
}
