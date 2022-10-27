#pragma once

#include <limits>
#include <ratio>
#include <string>
#include <type_traits>

namespace std {
namespace chrono {

// 27.5, class template duration (declaration)
template <class Rep, class Period = ratio<1>>
struct duration;

template < typename T>
struct __is_duration : false_type {};
template < typename Rep, typename Period>
struct __is_duration<duration<Rep, Period>> : true_type {};

template <class T>
using __enable_if_is_duration = enable_if_t<__is_duration<T>::value, T>;

// 27.6, class template time_point (declaration)
template <class Clock, class Duration = typename Clock::duration> class time_point;

}

template <intmax_t A, intmax_t B>
struct __lcm : integral_constant<intmax_t, A / __gcd<A, B>::value * B> {};

// 27.4.3, common_type specializations
template <class Rep1, class Period1, class Rep2, class Period2>
struct common_type<chrono::duration<Rep1, Period1>, chrono::duration<Rep2, Period2>>
{
private:
	typedef __gcd<Period1::num, Period2::num> _gcbNum;
	typedef __lcm<Period1::den, Period2::den> _lcmDen;

	typedef typename common_type<Rep1, Rep2>::type _cr;
	typedef ratio<_gcbNum::value, _lcmDen::value> _ratio;

public:
	typedef chrono::duration<_cr, _ratio> type;
};

template <class Rep, class Period>
struct common_type<chrono::duration<Rep, Period>, chrono::duration<Rep, Period>>
{
	typedef chrono::duration<Rep, Period> type;
};

template <class Rep, class Period>
struct common_type<chrono::duration<Rep, Period>>
{
	typedef chrono::duration<Rep, Period> type;
};

template <class Clock, class Duration1, class Duration2>
struct common_type<chrono::time_point<Clock, Duration1>, chrono::time_point<Clock, Duration2>>
{
	typedef chrono::time_point<Clock, typename common_type<Duration1, Duration2>::type> type;
};

//template <class Clock, class Duration>
//struct common_type<chrono::time_point<Clock, Duration>, chrono::time_point<Clock, Duration>>
//{
//	typedef chrono::time_point<Clock, Duration> type;
//};
//
//template <class Clock, class Duration>
//struct common_type<chrono::time_point<Clock, Duration>>
//{
//	typedef chrono::time_point<Clock, Duration> type;
//};

namespace chrono {
// 27.4, customization traits
template <class Rep>
struct treat_as_floating_point : is_floating_point<Rep> {};
template <class Rep>
inline constexpr bool treat_as_floating_point_v = treat_as_floating_point<Rep>::value;

template <class Rep>
struct duration_values
{
	static constexpr Rep zero()
	{
		return Rep(0);
	}
	static constexpr Rep max()
	{
		return numeric_limits<Rep>::max();
	}
	static constexpr Rep min()
	{
		return numeric_limits<Rep>::min();
	}
};

template <class T>
struct is_clock;
template <class T>
inline constexpr bool is_clock_v = is_clock<T>::value;

// 27.5.7, conversions (implementation)
// TODO duration_cast: Handle overflow + num == 1 + den == 1 + cast float/int/intmax
template <class ToDuration, class Rep, class Period,
	typename = __enable_if_is_duration<ToDuration>>
	constexpr ToDuration duration_cast(const duration<Rep, Period>& d)
{
	typedef ratio_divide<Period, typename ToDuration::period> cm;
	return ToDuration(d.count() * cm::num / cm::den);
}

template <class ToDuration, class Rep, class Period,
	typename = __enable_if_is_duration<ToDuration>>
	constexpr ToDuration floor(const duration<Rep, Period>& d)
{
	auto toDur = chrono::duration_cast<ToDuration>(d);
	if (toDur > d)
		return toDur - ToDuration{ 1 };
	return toDur;
}

template <class ToDuration, class Rep, class Period,
	typename = __enable_if_is_duration<ToDuration>>
	constexpr ToDuration ceil(const duration<Rep, Period>& d)
{
	auto toDur = chrono::duration_cast<ToDuration>(d);
	if (toDur < d)
		return toDur + ToDuration{ 1 };
	return toDur;
}

template <class ToDuration, class Rep, class Period,
	typename = enable_if_t<__is_duration<ToDuration>::value && !(treat_as_floating_point_v<typename ToDuration::rep>)>>
	constexpr ToDuration round(const duration<Rep, Period>& d)
{
	auto toDur0 = chrono::floor<ToDuration>(d);
	auto toDur1 = toDur0 + ToDuration{ 1 };
	auto diff0 = d - toDur0;
	auto diff1 = toDur1 - d;
	if (diff0 < diff1) {
		return toDur0;
	} else {
		return toDur1;
	}
}

// 27.5, class template duration (implementation)
template <class Rep, class Period>
struct duration
{
	typedef Rep rep;
	typedef Period period;

	constexpr duration() = default;
	duration(const duration& d) = default;

	template <class Rep2,
		typename = enable_if_t<is_convertible_v<Rep2, rep> && (treat_as_floating_point_v<rep> || !treat_as_floating_point_v<Rep2>)>>
		constexpr explicit duration(const Rep2& r) : __r(static_cast<rep>(r))
	{}

	template <class Rep2, class Period2,
		typename = enable_if_t<is_convertible_v<Rep2, rep> && (treat_as_floating_point_v<rep> || (ratio_divide<Period2, period>::type::den == 1 && !treat_as_floating_point_v<Rep2>))>>
		constexpr duration(const duration<Rep2, Period2>& d) : __r(duration_cast<duration>(d).count())
	{}

	duration& operator=(const duration& other) = default;

	constexpr rep count() const
	{
		return __r;
	}

	static constexpr duration zero() noexcept
	{
		return duration(duration_values<rep>::zero());
	}

	static constexpr duration min() noexcept
	{
		return duration(duration_values<rep>::min());
	}

	static constexpr duration max() noexcept
	{
		return duration(duration_values<rep>::max());
	}


	constexpr duration operator+() const
	{
		return duration<Rep, Period>(__r);
	}

	constexpr duration operator-() const
	{
		return duration<Rep, Period>(-__r);
	}

	constexpr duration& operator++()
	{
		++__r;
		return *this;
	}

	constexpr duration operator++(int)
	{
		return duration(__r++);
	}

	constexpr duration& operator--()
	{
		--__r;
		return *this;
	}

	constexpr duration operator--(int)
	{
		return duration<Rep, Period>(__r--);
	}

	constexpr duration& operator+=(const duration& d)
	{
		__r += d.count();
		return *this;
	}

	constexpr duration& operator-=(const duration& d)
	{
		__r -= d.count();
		return *this;
	}

	constexpr duration& operator*=(const rep& rhs)
	{
		__r *= rhs;
		return *this;
	}

	constexpr duration& operator/=(const rep& rhs)
	{
		__r /= rhs;
		return *this;
	}

	constexpr duration& operator%=(const rep& rhs)
	{
		__r %= rhs;
		return *this;
	}

	constexpr duration& operator%=(const duration& rhs)
	{
		__r %= rhs.count();
		return *this;
	}

private:
	Rep __r;
};

// 27.5.5, duration arithmetic
template <class Rep1, class Period1, class Rep2, class Period2>
constexpr common_type_t<duration<Rep1, Period1>, duration<Rep2, Period2>> operator+(const duration<Rep1, Period1>& lhs, const duration<Rep2, Period2>& rhs)
{
	typedef typename std::chrono::duration<Rep1, Period1> dur1;
	typedef typename std::chrono::duration<Rep2, Period2> dur2;
	typedef typename std::common_type_t<dur1, dur2> CT;
	return CT(CT(lhs).count() + CT(rhs).count());
}

template <class Rep1, class Period1, class Rep2, class Period2>
constexpr common_type_t<duration<Rep1, Period1>, duration<Rep2, Period2>> operator-(const duration<Rep1, Period1>& lhs, const duration<Rep2, Period2>& rhs)
{
	typedef typename std::chrono::duration<Rep1, Period1> dur1;
	typedef typename std::chrono::duration<Rep2, Period2> dur2;
	typedef typename std::common_type_t<dur1, dur2> CT;
	return CT(CT(lhs).count() - CT(rhs).count());
}

//template <class Rep1, class Period, class Rep2>
//constexpr duration<common_type_t<Rep1, Rep2>, Period> operator*(const duration<Rep1, Period>& d, const Rep2& s);

//template <class Rep1, class Rep2, class Period>
//constexpr duration<common_type_t<Rep1, Rep2>, Period> operator*(const Rep1& s, const duration<Rep2, Period>& d);

//template <class Rep1, class Period, class Rep2>
//constexpr duration<common_type_t<Rep1, Rep2>, Period> operator/(const duration<Rep1, Period>& d, const Rep2& s);

template <class Rep1, class Period1, class Rep2, class Period2>
constexpr common_type_t<Rep1, Rep2> operator/(const duration<Rep1, Period1>& lhs, const duration<Rep2, Period2>& rhs)
{
	typedef typename std::chrono::duration<Rep1, Period1> dur1;
	typedef typename std::chrono::duration<Rep2, Period2> dur2;
	typedef typename std::common_type_t<dur1, dur2> CT;
	return CT(lhs).count() / CT(rhs).count;
}

//template <class Rep1, class Period, class Rep2>
//constexpr duration<common_type_t<Rep1, Rep2>, Period> operator%(const duration<Rep1, Period>& d, const Rep2& s);

template <class Rep1, class Period1, class Rep2, class Period2>
constexpr common_type_t<duration<Rep1, Period1>, duration<Rep2, Period2>> operator%(const duration<Rep1, Period1>& lhs, const duration<Rep2, Period2>& rhs)
{
	typedef typename std::chrono::duration<Rep1, Period1> dur1;
	typedef typename std::chrono::duration<Rep2, Period2> dur2;
	typedef typename std::common_type_t<dur1, dur2> CT;
	return CT(lhs).count() %= rhs;
}

// 27.5.6, duration comparisons
template <class Rep1, class Period1, class Rep2, class Period2>
constexpr bool operator==(const std::chrono::duration<Rep1, Period1>& lhs,
						  const std::chrono::duration<Rep2, Period2>& rhs)
{
	typedef typename std::chrono::duration<Rep1, Period1> dur1;
	typedef typename std::chrono::duration<Rep2, Period2> dur2;
	typedef typename std::common_type_t<dur1, dur2> CT;
	return CT(lhs).count() == CT(rhs).count();
}

template <class Rep1, class Period1, class Rep2, class Period2>
constexpr bool operator!=(const std::chrono::duration<Rep1, Period1>& lhs,
						  const std::chrono::duration<Rep2, Period2>& rhs)
{
	return !(lhs == rhs);
}

template <class Rep1, class Period1, class Rep2, class Period2>
constexpr bool operator<(const std::chrono::duration<Rep1, Period1>& lhs,
	const std::chrono::duration<Rep2, Period2>& rhs)
{
	typedef typename std::chrono::duration<Rep1, Period1> dur1;
	typedef typename std::chrono::duration<Rep2, Period2> dur2;
	typedef typename std::common_type_t<dur1, dur2> CT;
	return CT(lhs).count() < CT(rhs).count();
}


template <class Rep1, class Period1, class Rep2, class Period2>
constexpr bool operator<=(const std::chrono::duration<Rep1, Period1>& lhs,
						  const std::chrono::duration<Rep2, Period2>& rhs)
{
	return !(rhs < lhs);
}


template <class Rep1, class Period1, class Rep2, class Period2>
constexpr bool operator>(const std::chrono::duration<Rep1, Period1>& lhs,
						 const std::chrono::duration<Rep2, Period2>& rhs)
{
	return rhs < lhs;
}


template <class Rep1, class Period1, class Rep2, class Period2>
constexpr bool operator>=(const std::chrono::duration<Rep1, Period1>& lhs,
						  const std::chrono::duration<Rep2, Period2>& rhs)
{
	return !(lhs < rhs);
}

//template <class Rep1, class Period1, class Rep2, class Period2>
//constexpr auto operator<=>(const duration<Rep1, Period1>& lhs,
//						   const duration<Rep2, Period2>& rhs);


// 27.5.10, duration I/O
//template <class charT, class traits, class Rep, class Period>
//basic_ostream<charT, traits>& operator<<(basic_ostream<charT, traits>& os, const duration<Rep, Period>& d);
//template <class charT, class traits, class Rep, class Period, class Alloc = allocator<charT>>
//basic_istream<charT, traits>&
//from_stream(basic_istream<charT, traits>& is, const charT* fmt,
//			duration<Rep, Period>& d,
//			basic_string<charT, traits, Alloc>* abbrev = nullptr,
//			minutes* offset = nullptr);

// convenience typedefs
using nanoseconds = duration<int64_t, nano>;
using microseconds = duration<int64_t, micro>;
using milliseconds = duration<int64_t, milli>;
using seconds = duration<int64_t>;
using minutes = duration<int64_t, ratio<60>>;
using hours = duration<int64_t, ratio<3600>>;
using days = duration<int64_t, ratio_multiply<ratio<24>, hours::period>>;
using weeks = duration<int64_t, ratio_multiply<ratio<7>, days::period>>;
using years = duration<int64_t, ratio_multiply<ratio<146097, 400>, days::period>>;
using months = duration<int64_t, ratio_divide<years::period, ratio<12>>>;


// 27.6, class template time_point (implementation)
template <class Clock, class Duration>
struct time_point
{
	typedef Clock clock;
	typedef Duration duration;
	typedef typename Duration::rep rep;
	typedef typename Duration::period period;

	constexpr time_point() : __d(duration::zero())
	{}

	constexpr explicit time_point(const duration& d) : __d(d)
	{}

	template< class Duration2 >
	constexpr time_point(const time_point<Clock, Duration2>& t) : __d(t.time_since_epoch())
	{}

	constexpr duration time_since_epoch() const
	{
		return __d;
	}

	constexpr time_point& operator+=(const duration& d)
	{
		__d += d;
		return *this;
	}

	constexpr time_point& operator-=(const duration& d)
	{
		__d -= d;
		return *this;
	}
	constexpr time_point& operator++()
	{
		++__d;
		return *this;
	}

	constexpr time_point operator++(int)
	{
		return time_point(__d++);
	}

	constexpr time_point& operator--()
	{
		--__d;
		return *this;
	}

	constexpr time_point operator--(int)
	{
		return time_point(__d--);
	}

	static constexpr time_point min() noexcept
	{
		return time_point(duration::min());
	}

	static constexpr time_point max() noexcept
	{
		return time_point(duration::max());
	}

private:
	Duration __d;
};

// 27.6.5, time_point arithmetic
template <class C, class D1, class R2, class P2>
constexpr time_point<C, common_type_t<D1, duration<R2, P2>>> operator+(const time_point<C, D1>& pt, const duration<R2, P2>& d)
{
	typedef common_type_t<D1, duration<R2, P2>> CT;
	typedef time_point<C, CT> TP;
	return TP(pt.time_since_epoch() + d);
}

template <class R1, class P1, class C, class D2>
constexpr time_point<C, std::common_type_t<duration<R1, P1>, D2>> operator+(const duration<R1, P1>& d, const time_point<C, D2>& pt)
{
	typedef common_type_t<duration<R1, P1>, D2> CT;
	typedef time_point<C, CT> TP;
	return TP(pt.time_since_epoch() + d);
}

template <class C, class D1, class R2, class P2>
constexpr time_point<C, std::common_type_t<D1, duration<R2, P2>>> operator-(const time_point<C, D1>& pt, const duration<R2, P2>& d)
{
	typedef common_type_t<D1, duration<R2, P2>> CT;
	typedef time_point<C, CT> TP;
	return TP(pt.time_since_epoch() - d);
}

template <class C, class D1, class D2>
constexpr std::common_type_t<D1, D2> operator-(const time_point<C, D1>& pt_lhs, const time_point<C, D2>& pt_rhs)
{
	return pt_lhs.time_since_epoch() - pt_rhs.time_since_epoch();
}

// 27.6.6, time_point comparisons
template <class Clock, class Dur1, class Dur2>
constexpr bool operator==(const std::chrono::time_point<Clock, Dur1>& lhs,
						  const std::chrono::time_point<Clock, Dur2>& rhs)
{
	return lhs.time_since_epoch() == rhs.time_since_epoch();
}

template <class Clock, class Dur1, class Dur2>
constexpr bool operator!=(const std::chrono::time_point<Clock, Dur1>& lhs,
						  const std::chrono::time_point<Clock, Dur2>& rhs)
{
	return !(lhs == rhs);
}


template <class Clock, class Dur1, class Dur2>
constexpr bool operator<(const std::chrono::time_point<Clock, Dur1>& lhs,
	const std::chrono::time_point<Clock, Dur2>& rhs)
{
	return lhs.time_since_epoch() < rhs.time_since_epoch();
}


template <class Clock, class Dur1, class Dur2>
constexpr bool operator<=(const std::chrono::time_point<Clock, Dur1>& lhs,
						  const std::chrono::time_point<Clock, Dur2>& rhs)
{
	return !(rhs < lhs);
}


template <class Clock, class Dur1, class Dur2>
constexpr bool operator>(const std::chrono::time_point<Clock, Dur1>& lhs,
						 const std::chrono::time_point<Clock, Dur2>& rhs)
{
	return rhs < lhs;
}


template <class Clock, class Dur1, class Dur2>
constexpr bool operator>=(const std::chrono::time_point<Clock, Dur1>& lhs,
						  const std::chrono::time_point<Clock, Dur2>& rhs)
{
	return !(lhs < rhs);
}

//template <class Clock, class Duration1, three_way_comparable_with<Duration1> Duration2>
//constexpr auto operator<=>(const time_point<Clock, Duration1>& lhs,
//						   const time_point<Clock, Duration2>& rhs);

// 27.6.7, conversions
template <class ToDuration, class Clock, class Duration, typename = __enable_if_is_duration<ToDuration>>
constexpr time_point<Clock, ToDuration> time_point_cast(const time_point<Clock, Duration>& t)
{
	return time_point<Clock, ToDuration>(duration_cast<ToDuration>(t.time_since_epoch()));
}

template <class ToDuration, class Clock, class Duration, typename = __enable_if_is_duration<ToDuration>>
constexpr time_point<Clock, ToDuration> floor(const std::chrono::time_point<Clock, Duration>& tp)
{
	return time_point<Clock, ToDuration>(floor<ToDuration>(tp.time_since_epoch()));
}

template <class ToDuration, class Clock, class Duration, typename = __enable_if_is_duration<ToDuration>>
constexpr time_point<Clock, ToDuration> ceil(const std::chrono::time_point<Clock, Duration>& tp)
{
	return time_point<Clock, ToDuration>(ceil<ToDuration>(tp.time_since_epoch()));
}

template <class ToDuration, class Clock, class Duration, typename = __enable_if_is_duration<ToDuration>>
constexpr time_point<Clock, ToDuration> round(const std::chrono::time_point<Clock, Duration>& tp)
{
	return time_point<Clock, ToDuration>(round<ToDuration>(tp.time_since_epoch()));
}

// 27.5.9, specialized algorithms
template <class Rep, class Period, typename = enable_if<numeric_limits<Rep>::is_signed>>
constexpr duration<Rep, Period> abs(duration<Rep, Period> d)
{
	return d >= d.zero() ? d : -d;
}

}

inline namespace literals {
inline namespace chrono_literals {

// 27.5.8, suffixes for duration literals
constexpr chrono::hours operator ""h(unsigned long long h)
{
	return chrono::hours(h);
}
constexpr chrono::duration<long double, ratio<3600, 1>> operator ""h(long double h)
{
	return chrono::duration<long double, ratio<3600, 1>>(h);
}

constexpr chrono::minutes operator ""min(unsigned long long m)
{
	return chrono::minutes(m);
}
constexpr chrono::duration<long double, ratio<60, 1>> operator ""min(long double m)
{
	return chrono::duration<long double, ratio<60, 1>>(m);
}

constexpr chrono::seconds operator ""s(unsigned long long s)
{
	return chrono::seconds(s);
}
constexpr chrono::duration<long double> operator ""s(long double s)
{
	return chrono::duration<long double>(s);
}

constexpr chrono::milliseconds operator ""ms(unsigned long long ms)
{
	return chrono::milliseconds(ms);
}
constexpr chrono::duration<long double, milli> operator ""ms(long double ms)
{
	return chrono::duration<long double, milli>(ms);
}


constexpr chrono::microseconds operator ""us(unsigned long long us)
{
	return chrono::microseconds(us);
}
constexpr chrono::duration<long double, micro> operator ""us(long double us)
{
	return chrono::duration<long double, micro>(us);
}

constexpr chrono::nanoseconds operator ""ns(unsigned long long ns)
{
	return chrono::nanoseconds(ns);
}
constexpr chrono::duration<long double, nano> operator ""ns(long double ns)
{
	return chrono::duration<long double, nano>(ns);
}

}
}


}
