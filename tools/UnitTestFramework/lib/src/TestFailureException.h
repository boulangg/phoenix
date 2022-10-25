#pragma once

#include <stdexcept>
#include <string>

#include "utils.h"

class TestFailureException : public std::runtime_error
{
public:
	TestFailureException(const std::string& expected, const std::string& actual, const std::string& message) :
		runtime_error(message), _expected(expected), _actual(actual), _message(message)
	{
		if (_message.empty()) {
			_message += "Assert failed. Expected:<" + _expected + "> Actual:<" + _actual + ">";
		}
	}

	std::string getMessage() const
	{
		return _message;
	}

private:
	std::string _expected;
	std::string _actual;
	std::string _message;
};
