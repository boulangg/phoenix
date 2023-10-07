#pragma once

#include <stdexcept>
#include <string>
#include <string.h>

#include "utils.h"

class TestFailureException
{
public:
	TestFailureException(std::string expected, std::string actual, std::string message)
	{
		std::string msg = message;
		if (msg.empty()) {
			msg += "Assert failed. Expected:<" + expected + "> Actual:<" + actual + ">";
		}

		_message = new char[msg.size() + 1];
		strncpy(_message, msg.c_str(), msg.size() + 1);
	}

	~TestFailureException()
	{
		if (_message != nullptr) {
			delete _message;
			_message = nullptr;
		}
	}

	std::string getMessage() const
	{
		return std::string(_message);
	}

private:
	// TestFailureException can't store std::string because the string are created in the testuite and then read by the test runner. 
	// They could have different structure if they are not build with the same stdlib.
	char* _message = nullptr;
};
