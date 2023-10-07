#include <exception>

#include <stdlib.h>

using namespace std;

exception::exception() noexcept : _msg("")
{}

exception::exception(const exception& exc) noexcept : _msg(exc._msg)
{}

exception& exception::operator=(const exception& exc) noexcept
{
	_msg = exc._msg;
	return *this;
}

exception::~exception()
{}

const char* exception::what() const noexcept
{
	return _msg;
}

void std::terminate() noexcept
{
	abort();
}
