/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

#include "../src/TestFailureException.h"

#include <string>

namespace UnitTestFramework {

struct Assert {
	template<typename T>
	static void AreEqual(const T& u, const T& v, const std::string& message);
	template<typename T>
	static void AreEqual(const T& u, const T& v);

	template<typename T>
	static void AreNotEqual(const T& u, const T& v, const std::string& message);
	template<typename T>
	static void AreNotEqual(const T& u, const T& v);

	static void IsTrue(const bool& u, const std::string& message);
	static void IsTrue(const bool& u);

	static void IsFalse(const bool& u, const std::string& message);
	static void IsFalse(const bool& u);
};

template <typename T>
void Assert::AreEqual(const T& u, const T& v, const std::string& message)
{
    if (u != v) {
        throw TestFailureException(std::to_string(u), std::to_string(v), message);
    }
}

template<typename T>
void Assert::AreEqual(const T& u, const T& v)
{
	AreEqual(u, v, "");
}


template<typename T>
void Assert::AreNotEqual(const T& u, const T& v, const std::string& message)
{
	if (u == v) {
		throw TestFailureException(std::to_string(u), std::to_string(v), message);
	}
}

template<typename T>
void Assert::AreNotEqual(const T& u, const T& v)
{
	AreNotEqual(u, v, "");
}

}
