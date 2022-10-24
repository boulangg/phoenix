#pragma once

#include "../src/TestFailureException.h"

#include <string>

namespace UnitTestFramework 
{

class Assert
{
public:
	template<typename T>
	static void AreEqual(const T& u, const T& v);

	template<typename T>
	static void AreEqual(const T& u, const T& v, const std::string& message);
};

template<typename T>
void Assert::AreEqual(const T& u, const T& v)
{
	AreEqual(u, v, "");
}

template<typename T>
void Assert::AreEqual(const T& u, const T& v, const std::string& message)
{
	if (u != v) {
		throw TestFailureException(std::to_string(u), std::to_string(v), message);
	}
}

}