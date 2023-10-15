/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include <stdio.h>

#include <string>

void fn_throw_and_catch()
{
    std::string test = "test";
    try {
        throw std::string("bis");
    } catch (int a) {
        printf("%i\n", a);
    } catch (std::string& a) {
        printf("%s, %s\n", a.c_str(), test.c_str());
    }
}

void fn_throw()
{
    std::string test = "test";
    throw std::string("throw");
}

void fn_rethrow()
{
    try {
        fn_throw();
    } catch (std::string str) {
        throw;
    }
}

void fn_throw_again()
{
    try {
        fn_throw();
    } catch (std::string&) {
        throw std::string("throw again");
    }
}

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    fn_throw_and_catch();

    try {
        fn_throw();
    } catch (std::string str) {
        printf("%s\n", str.c_str());
    }

    try {
        fn_rethrow();
    } catch (std::string& str) {
        printf("%s\n", str.c_str());
    }

    try {
        fn_throw_again();
    } catch (std::string& str) {
        printf("%s\n", str.c_str());
    }

    return 0;
}
