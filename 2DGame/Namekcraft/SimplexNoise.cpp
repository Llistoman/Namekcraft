
#include "SimplexNoise.h"

SimplexNoise::SimplexNoise(mt19937 *gen, float s, float minimum, float maximum) {
    scale = s;
    min = minimum;
    max = maximum;
    std::uniform_int_distribution<int> distribution(0,255);
    permutations.resize(256);
    for (int i = 0; i < 256; i++)
        permutations[i] = i;
    for (int i = 0; i < 256; i++) {
        int j = distribution(*gen);
        //Swap permutations[i] and permutations[j]
        int aux = permutations[i];
        permutations[i] = permutations[j];
        permutations[j] = aux;
    }
    permutations.resize(512);
    for (int i = 0; i < 256; i++)
        permutations[i+256] = permutations[i];
}


float SimplexNoise::dotprod(const int *grad, const float x, const float y) {
    return grad[0]*x + grad[1]*y;
}

int SimplexNoise::fastfloor( const float x ) {
    //return x > 0 ? (int) x : (int) x - 1;
    int i = (int)x;
    return (x < i) ? (i - 1) : (i);
}

const int SimplexNoise::g3[12][3] = {
        {1,0,-1}, {-1,0,-1}, {1,-1,0}, {-1,-1,0},
        {0,1,1}, {0,-1,1}, {1,1,0}, {-1,1,0},
        {1,0,1}, {-1,0,1}, {0,1,-1}, {0,-1,-1}
};

float SimplexNoise::noise(float x, float y) {

    x = x/scale;
    y = y/scale;

    // Noise contributions from the three corners
    float a, b, c;

    // Skew the input space to determine which simplex cell we're in
    float F2 = 0.5f * (sqrtf(3.0f) - 1.0f);
    // Hairy factor for 2D
    float s = (x + y) * F2;
    int i = fastfloor( x + s );
    int j = fastfloor( y + s );

    float G2 = (3.0f - sqrtf(3.0f)) / 6.0f;
    float t = (i + j) * G2;
    // Unskew the cell origin back to (x,y) space
    float X0 = i-t;
    float Y0 = j-t;
    // The x,y distances from the cell origin
    float x0 = x-X0;
    float y0 = y-Y0;

    // For the 2D case, the simplex shape is an equilateral triangle.
    // Determine which simplex we are in.
    int i1, j1; // Offsets for second (middle) corner of simplex in (i,j) coords
    if(x0>y0) {i1=1; j1=0;} // lower triangle, XY order: (0,0)->(1,0)->(1,1)
    else {i1=0; j1=1;} // upper triangle, YX order: (0,0)->(0,1)->(1,1)

    // A step of (1,0) in (i,j) means a step of (1-c,-c) in (x,y), and
    // a step of (0,1) in (i,j) means a step of (-c,1-c) in (x,y), where
    // c = (3-sqrt(3))/6
    float x1 = x0 - i1 + G2; // Offsets for middle corner in (x,y) unskewed coords
    float y1 = y0 - j1 + G2;
    float x2 = x0 - 1.0f + 2.0f * G2; // Offsets for last corner in (x,y) unskewed coords
    float y2 = y0 - 1.0f + 2.0f * G2;

    // Work out the hashed gradient indices of the three simplex corners
    int ii = i & 255;
    int jj = j & 255;
    int gi0 = permutations[ii+permutations[jj]] % 12;
    int gi1 = permutations[ii+i1+permutations[jj+j1]] % 12;
    int gi2 = permutations[ii+1+permutations[jj+1]] % 12;

    // Calculate the contribution from the three corners
    float t0 = 0.5f - x0*x0-y0*y0;
    if(t0<0) a = 0.0;
    else {
        t0 *= t0;
        a = t0 * t0 * dotprod(g3[gi0], x0, y0); // (x,y) of g3 used for 2D gradient
    }

    float t1 = 0.5f - x1*x1-y1*y1;
    if(t1<0) b = 0.0;
    else {
        t1 *= t1;
        b = t1 * t1 * dotprod(g3[gi1], x1, y1);
    }

    float t2 = 0.5f - x2*x2-y2*y2;
    if(t2<0) c = 0.0;
    else {
        t2 *= t2;
        c = t2 * t2 * dotprod(g3[gi2], x2, y2);
    }

    // Add contributions from each corner to get the final noise value.
    // The result is scaled to return values in the interval [-1,1].
    float result = 70.0f * (a + b + c);
    //scale the result
    result += 1; //in [0,2]
    result *= 0.5; // in [0,1]
    float difference = max-min;
    return (min + difference * result);
}

float SimplexNoise::fractal(int octaves, float freq, float ampl, float x, float y) {
    float output = 0.f;
    float denom = 0.f;

    for (int i = 0; i < octaves; i++) {
            output += (ampl * noise(x * freq, y * freq));
            denom += ampl;

            freq *= 2.0f;  //Lacunarity
            ampl *= 1.0f/2.0f; //Persistence = 1/lacunarity;
        }

    return (output / denom);
}
