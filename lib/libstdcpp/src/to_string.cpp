#include <string>

#include <cstdio>

#define BUFFER_SIZE 128
template<typename T>
static std::string string_format(const char* format, T value)
{
	char buffer[BUFFER_SIZE];
	int size = snprintf(buffer, BUFFER_SIZE, format, value);
	buffer[size] = '\0';
	return std::string(buffer);
}

std::string to_string(int value)
{
	return string_format("%d", value);
}

std::string to_string(long value)
{
	return string_format("%ld", value);
}

std::string to_string(long long value)
{
	return string_format("%lld", value);
}

std::string to_string(unsigned value)
{
	return string_format("%u", value);
}

std::string to_string(unsigned long value)
{
	return string_format("%lu", value);
}

std::string to_string(unsigned long long value)
{
	return string_format("%llu", value);
}

std::string to_string(float value)
{
	return string_format("%f", value);
}

std::string to_string(double value)
{
	return string_format("%f", value);
}

std::string to_string(long double value)
{
	return string_format("%Lf", value);
}
