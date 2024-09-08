/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include <atomic>

namespace std {

// bool atomic_flag_test(const volatile atomic_flag* flag) noexcept
//{
//     return flag->test();
// }
//
// bool atomic_flag_test(const atomic_flag* flag) noexcept
//{
//     return flag->test();
// }
//
// bool atomic_flag_test_explicit(const volatile atomic_flag* flag, memory_order order) noexcept
//{
//     return flag->test(order);
// }
//
// bool atomic_flag_test_explicit(const atomic_flag* flag, memory_order order) noexcept
//{
//     return flag->test(order);
// }

bool atomic_flag_test_and_set(volatile atomic_flag* flag) noexcept
{
    return flag->test_and_set();
}

bool atomic_flag_test_and_set(atomic_flag* flag) noexcept
{
    return flag->test_and_set();
}

bool atomic_flag_test_and_set_explicit(volatile atomic_flag* flag, memory_order order) noexcept
{
    return flag->test_and_set(order);
}

bool atomic_flag_test_and_set_explicit(atomic_flag* flag, memory_order order) noexcept
{
    return flag->test_and_set(order);
}

void atomic_flag_clear(volatile atomic_flag* flag) noexcept
{
    return flag->clear();
}

void atomic_flag_clear(atomic_flag* flag) noexcept
{
    return flag->clear();
}

void atomic_flag_clear_explicit(volatile atomic_flag* flag, memory_order order) noexcept
{
    return flag->clear(order);
}

void atomic_flag_clear_explicit(atomic_flag* flag, memory_order order) noexcept
{
    return flag->clear(order);
}

// void atomic_flag_wait(const volatile atomic_flag* flag, bool val) noexcept
//{
//     return flag->wait(val);
// }
//
// void atomic_flag_wait(const atomic_flag* flag, bool val) noexcept
//{
//     return flag->wait(val);
// }
//
// void atomic_flag_wait_explicit(const volatile atomic_flag* flag, bool val, memory_order order) noexcept
//{
//     return flag->wait(val, order);
// }
//
// void atomic_flag_wait_explicit(const atomic_flag* flag, bool val, memory_order order) noexcept
//{
//     return flag->wait(val, order);
// }
//
// void atomic_flag_notify_one(volatile atomic_flag* flag) noexcept
//{
//     return flag->notify_one();
// }
//
// void atomic_flag_notify_one(atomic_flag* flag) noexcept
//{
//     return flag->notify_one();
// }
//
// void atomic_flag_notify_all(volatile atomic_flag* flag) noexcept
//{
//     return flag->notify_all();
// }
//
// void atomic_flag_notify_all(atomic_flag* flag) noexcept
//{
//     return flag->notify_all();
// }

}
