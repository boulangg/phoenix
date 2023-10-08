#include "math.h"

float ceilf(float num)
{
	return __builtin_ceilf(num);
}

double ceil(double num)
{
	return __builtin_ceil(num);
}

long double ceill(long double num)
{
	return __builtin_ceill(num);
}
