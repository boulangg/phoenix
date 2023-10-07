#include "UnitTestFrameworkAssert.h"

namespace UnitTestFramework {

void Assert::IsTrue(const bool& u, const std::string& message)
{
	if (!u) {
		throw TestFailureException(std::to_string(true), std::to_string(u), message);
	}
}

void Assert::IsTrue(const bool& u)
{
	IsTrue(u, "");
}

void Assert::IsFalse(const bool& u, const std::string& message)
{
	if (u) {
		throw TestFailureException(std::to_string(false), std::to_string(u), message);
	}
}

void Assert::IsFalse(const bool& u)
{
	IsFalse(u, "");
}

}
