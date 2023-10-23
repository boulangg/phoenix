/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
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
