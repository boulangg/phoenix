/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

#include "duration.h"

#include <ctime>

namespace std {
namespace chrono {

// 27.7.1, class system_clock
struct system_clock;
template <class Duration>
using sys_time = time_point<system_clock, Duration>;
using sys_seconds = sys_time<seconds>;
using sys_days = sys_time<days>;

struct system_clock
{
    typedef nanoseconds duration;
    typedef duration::rep rep;
    typedef duration::period period;
    typedef chrono::time_point<system_clock, duration> time_point;

    static constexpr bool is_steady = false;

    static time_point now() noexcept;

    static std::time_t to_time_t(const time_point& t) noexcept;
    static time_point from_time_t(std::time_t t) noexcept;
};

// template <class charT, class traits, class Duration>
// basic_ostream<charT, traits>& operator<<(basic_ostream<charT, traits>& os, const sys_time<Duration>& tp);
// template <class charT, class traits>
// basic_ostream<charT, traits>& operator<<(basic_ostream<charT, traits>& os, const sys_days& dp);
// template <class charT, class traits, class Duration, class Alloc = allocator<charT>>
// basic_istream<charT, traits>&
// from_stream(basic_istream<charT, traits>& is, const charT* fmt,
//			sys_time<Duration>& tp,
//			basic_string<charT, traits, Alloc>* abbrev = nullptr,
//			minutes* offset = nullptr);

// 27.7.2, class utc_clock
struct utc_clock;
template <class Duration>
using utc_time = time_point<utc_clock, Duration>;
using utc_seconds = utc_time<seconds>;

struct utc_clock
{
    typedef nanoseconds duration;
    typedef duration::rep rep;
    typedef duration::period period;
    typedef chrono::time_point<utc_clock, duration> time_point;

    static constexpr bool is_steady = false;

    static time_point now() noexcept;

    template <class Duration>
    static sys_time<common_type_t<Duration, seconds>> to_sys(const utc_time<Duration>& t);
    template <class Duration>
    static utc_time<common_type_t<Duration, seconds>> from_sys(const sys_time<Duration>& t);
};

// template <class charT, class traits, class Duration>
// basic_ostream<charT, traits>& operator<<(basic_ostream<charT, traits>& os, const utc_time<Duration>& t);
// template <class charT, class traits, class Duration, class Alloc = allocator<charT>>
// basic_istream<charT, traits>&
// from_stream(basic_istream<charT, traits>& is, const charT* fmt,
//			utc_time<Duration>& tp,
//			basic_string<charT, traits, Alloc>* abbrev = nullptr,
//			minutes* offset = nullptr);

struct leap_second_info
{
    bool is_leap_second;
    seconds elapsed;
};

template <class Duration>
leap_second_info get_leap_second_info(const utc_time<Duration>& ut);

// 27.7.3, class tai_clock
// class tai_clock;
// template <class Duration>
// using tai_time = time_point<tai_clock, Duration>;
// using tai_seconds = tai_time<seconds>;
// template <class charT, class traits, class Duration>
// basic_ostream<charT, traits>& operator<<(basic_ostream<charT, traits>& os, const tai_time<Duration>& t);
// template <class charT, class traits, class Duration, class Alloc = allocator<charT>>
// basic_istream<charT, traits>&
// from_stream(basic_istream<charT, traits>& is, const charT* fmt,
//			tai_time<Duration>& tp,
//			basic_string<charT, traits, Alloc>* abbrev = nullptr,
//			minutes* offset = nullptr);

// 27.7.4, class gps_clock
// class gps_clock;
// template <class Duration>
// using gps_time = time_point<gps_clock, Duration>;
// using gps_seconds = gps_time<seconds>;
// template <class charT, class traits, class Duration>
// basic_ostream<charT, traits>& operator<<(basic_ostream<charT, traits>& os, const gps_time<Duration>& t);
// template <class charT, class traits, class Duration, class Alloc = allocator<charT>>
// basic_istream<charT, traits>&
// from_stream(basic_istream<charT, traits>& is, const charT* fmt,
//			gps_time<Duration>& tp,
//			basic_string<charT, traits, Alloc>* abbrev = nullptr,
//			minutes* offset = nullptr);

// 27.7.5, type file_clock
// using file_clock = see below;
// template <class Duration>
// using file_time = time_point<file_clock, Duration>;
// template <class charT, class traits, class Duration>
// basic_ostream<charT, traits>& operator<<(basic_ostream<charT, traits>& os, const file_time<Duration>& tp);
// template <class charT, class traits, class Duration, class Alloc = allocator<charT>>
// basic_istream<charT, traits>&
// from_stream(basic_istream<charT, traits>& is, const charT* fmt,
//			file_time<Duration>& tp,
//			basic_string<charT, traits, Alloc>* abbrev = nullptr,
//			minutes* offset = nullptr);

// 27.7.6, class steady_clock
struct steady_clock
{
    typedef nanoseconds duration;
    typedef duration::rep rep;
    typedef duration::period period;
    typedef chrono::time_point<steady_clock, duration> time_point;

    static constexpr bool is_steady = true;

    static time_point now() noexcept;
};

// 27.7.7, class high_resolution_clock
using high_resolution_clock = system_clock;

// 27.7.8, local time
struct local_t
{};
template <class Duration>
using local_time = time_point<local_t, Duration>;
using local_seconds = local_time<seconds>;
using local_days = local_time<days>;
// template <class charT, class traits, class Duration>
// basic_ostream<charT, traits>& operator<<(basic_ostream<charT, traits>& os, const local_time<Duration>& tp);
// template <class charT, class traits, class Duration, class Alloc = allocator<charT>>
// basic_istream<charT, traits>&
// from_stream(basic_istream<charT, traits>& is, const charT* fmt,
//			local_time<Duration>& tp,
//			basic_string<charT, traits, Alloc>* abbrev = nullptr,
//			minutes* offset = nullptr);

// 27.7.9, time_point conversions
template <class DestClock, class SourceClock>
struct clock_time_conversion;
template <class DestClock, class SourceClock, class Duration>
auto clock_cast(const time_point<SourceClock, Duration>& t);

}
}
