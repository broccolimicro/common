#include "math.h"

#include <random>

unsigned int count_ones(unsigned int x)
{
    x = x - ((x >> 1) & 0x55555555);
    x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
    x = (x & 0x0F0F0F0F) + ((x >> 4) & 0x0F0F0F0F);
    x = x + (x >> 8);
    x = x + (x >> 16);
    return x & 0x0000003F;
}

unsigned int count_zeros(unsigned int x)
{
	x = x - ((x >> 1) & 0x55555555);
	x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
	x = (x & 0x0F0F0F0F) + ((x >> 4) & 0x0F0F0F0F);
	x = x + (x >> 8);
	x = x + (x >> 16);
    return 32 - (x & 0x0000003F);
}

int powi(int base, int exp)
{
    int result = 1;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        base *= base;
    }

    return result;
}

int log2i(unsigned long long value)
{
  static const unsigned long long t[6] = {
    0xFFFFFFFF00000000ull,
    0x00000000FFFF0000ull,
    0x000000000000FF00ull,
    0x00000000000000F0ull,
    0x000000000000000Cull,
    0x0000000000000002ull
  };

  int y = (((value & (value - 1)) == 0) ? 0 : 1);
  int j = 32;
  int i;

  for (i = 0; i < 6; i++) {
    int k = (((value & t[i]) == 0) ? 0 : j);
    y += k;
    value >>= k;
    j >>= 1;
  }

  return y;
}

uint64_t pareto(uint64_t scale, double shape) {
	static std::random_device rd;  // Seed for randomness
	static std::mt19937_64 gen(rd()); // Mersenne Twister generator

	std::uniform_real_distribution<> dis(0.0,1.0);
	double c = pow(dis(gen), 1.0/shape);
	uint64_t ci = (uint64_t)(((double)(1ul<<32))*c);
	return min(scale, (scale<<32)/max((uint64_t)1ul,ci) - scale);
}
