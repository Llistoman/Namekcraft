#ifndef _SIMPLEXNOISE_INCLUDE
#define _SIMPLEXNOISE_INCLUDE

#include <random>

using namespace std;

class SimplexNoise
{
public:
    SimplexNoise(mt19937* gen, float s, float minimum, float maximum);
    ~SimplexNoise();
    float noise(float x, float y);
    float fractal(int octaves, float freq, float ampl, float x, float y);

private:
    float dotprod(const int* grad, const float x, const float y);
    int fastfloor(const float x);

    vector<int> permutations;
    static const int g3[12][3];
    mt19937* gen;
    float scale;
    float min;
    float max;
};

#endif // _SIMPLEXNOISE_INCLUDE
