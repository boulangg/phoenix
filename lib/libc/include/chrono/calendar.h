#pragma once

#include "duration.h"

namespace std {
namespace chrono {

// 27.8.2, class last_spec
struct last_spec
{
	explicit last_spec() = default;
};
inline constexpr last_spec last{};


namespace detail {
constexpr unsigned __modulo(long long n, unsigned d)
{
	if (n >= 0)
		return n % d;
	else
		return (d + (n % d)) % d;
}

inline constexpr unsigned __days_per_month[12] =
{ 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
}

// 27.8.3, class day
class day
{
public:
	day() = default;

	explicit constexpr day(unsigned d) noexcept : _d(d)
	{};

	constexpr day& operator++() noexcept
	{
		*this += days{ 1 };
		return *this;
	}

	constexpr day operator++(int) noexcept
	{
		day ret = day{ _d };
		*this += days{ 1 };
		return ret;
	}

	constexpr day& operator--() noexcept
	{
		*this -= days{ 1 };
		return *this;
	}

	constexpr day operator--(int) noexcept
	{
		day ret = day{ _d };
		*this -= days{ 1 };
		return ret;
	}

	constexpr day& operator+=(const days& d) noexcept
	{
		_d += d.count();
		return *this;
	}
	constexpr day& operator-=(const days& d) noexcept
	{
		_d -= d.count();
		return *this;
	}

	explicit constexpr operator unsigned() const noexcept
	{
		return _d;
	}

	constexpr bool ok() const noexcept
	{
		return 1 <= _d && _d <= 31;
	}

private:
	unsigned _d;
};
constexpr bool operator==(const day& x, const day& y) noexcept
{
	return static_cast<unsigned>(x) == static_cast<unsigned>(y);
}

//constexpr strong_ordering operator<=>(const day& x, const day& y) noexcept;
constexpr day operator+(const day& d, const days& ds) noexcept
{
	return day(unsigned(d) + ds.count());
}

constexpr day operator+(const days& ds, const day& d) noexcept
{
	return day(unsigned(d) + ds.count());
}

constexpr day operator-(const day& d, const days& ds) noexcept
{
	return day(unsigned(d) - ds.count());
}

constexpr days operator-(const day& d1, const day& d2) noexcept
{
	return days(int(unsigned(d1)) - int(unsigned(d2)));
}

//template <class charT, class traits>
//basic_ostream<charT, traits>& operator<<(basic_ostream<charT, traits>& os, const day& d);
//template <class charT, class traits, class Alloc = allocator<charT>>
//basic_istream<charT, traits>&
//from_stream(basic_istream<charT, traits>& is, const charT* fmt,
//			day& d, basic_string<charT, traits, Alloc>* abbrev = nullptr,
//			minutes* offset = nullptr);

// 27.8.4, class month
class month;
constexpr month operator+(const month& m, const months& ms) noexcept;
constexpr month operator-(const month& m, const months& ms) noexcept;

class month
{
public:
	month() = default;

	explicit constexpr month(unsigned m) noexcept : _m(m)
	{}

	constexpr month& operator++() noexcept
	{
		*this += months(1);
		return *this;
	}

	constexpr month operator++(int) noexcept
	{
		month ret = month{ _m };
		++(*this);
		return ret;
	}

	constexpr month& operator--() noexcept
	{
		*this -= months(1);
		return *this;
	}

	constexpr month operator--(int) noexcept
	{
		month ret = month{ _m };
		--(*this);
		return ret;
	}

	constexpr month& operator+=(const months & m) noexcept
	{
		*this = *this + m;
		return *this;
	}

	constexpr month& operator-=(const months & m) noexcept
	{
		*this = *this - m;
		return *this;
	}

	explicit constexpr operator unsigned() const noexcept
	{
		return _m;
	}

	constexpr bool ok() const noexcept
	{
		return 1 <= _m && _m <= 12;
	}

private:
	unsigned _m;
};

inline constexpr month January{ 1 };
inline constexpr month February{ 2 };
inline constexpr month March{ 3 };
inline constexpr month April{ 4 };
inline constexpr month May{ 5 };
inline constexpr month June{ 6 };
inline constexpr month July{ 7 };
inline constexpr month August{ 8 };
inline constexpr month September{ 9 };
inline constexpr month October{ 10 };
inline constexpr month November{ 11 };
inline constexpr month December{ 12 };

constexpr bool operator==(const month& x, const month& y) noexcept
{
	return unsigned(x) == unsigned(y);
}

//constexpr strong_ordering operator<=>(const month& x, const month& y) noexcept;
constexpr month operator+(const month& m, const months& ms) noexcept
{
	auto n = static_cast<long long>(unsigned{ m }) + (ms.count() - 1);
	return month{ detail::__modulo(n, 12) + 1 };
}

constexpr month operator+(const months& ms, const month& m) noexcept
{
	return m + ms;
}

constexpr month operator-(const month& m, const months& ms) noexcept
{
	return m + -ms;
}

constexpr months operator-(const month& m1, const month& m2) noexcept
{
	const auto dm = int(unsigned(m1)) - int(unsigned(m2));
	return months{ dm < 0 ? 12 + dm : dm };
}

//template <class charT, class traits>
//basic_ostream<charT, traits>& operator<<(basic_ostream<charT, traits>& os, const month& m);
//template <class charT, class traits, class Alloc = allocator<charT>>
//basic_istream<charT, traits>&
//from_stream(basic_istream<charT, traits>& is, const charT* fmt,
//			month& m, basic_string<charT, traits, Alloc>* abbrev = nullptr,
//			minutes* offset = nullptr);

// 27.8.5, class year
class year;
constexpr year operator+(const year& y, const years& ys) noexcept;
constexpr year operator-(const year& y, const years& ys) noexcept;

class year
{
public:
	year() = default;

	explicit constexpr year(int y) noexcept : _y(y)
	{}

	constexpr year& operator++() noexcept
	{
		*this += years(1);
		return *this;
	}

	constexpr year operator++(int) noexcept
	{
		year ret = year{ _y };
		++(*this);
		return ret;
	}

	constexpr year& operator--() noexcept
	{
		*this -= years(1);
		return *this;
	}

	constexpr year operator--(int) noexcept
	{
		year ret = year{ _y };
		--(*this);
		return ret;
	}

	constexpr year& operator+=(const years& y) noexcept
	{
		*this = *this + y;
		return *this;
	}

	constexpr year& operator-=(const years& y) noexcept
	{
		*this = *this - y;
		return *this;
	}

	constexpr bool is_leap() const noexcept
	{
		return (_y % 100 != 0 || _y % 400 == 0) && _y % 4 == 0;
	}

	explicit constexpr operator int() const noexcept
	{
		return _y;
	}

	constexpr bool ok() const noexcept
	{
		return -32767 <= _y && _y <= 32767;
	}

	static constexpr year min() noexcept
	{
		return year(-32767);
	}

	static constexpr year max() noexcept
	{
		return year(32767);
	}

private:
	int _y;
};

constexpr bool operator==(const year& x, const year& y) noexcept;
//constexpr strong_ordering operator<=>(const year& x, const year& y) noexcept;
constexpr year operator+(const year& y, const years& ys) noexcept
{
	return year(int(y) + ys.count());
}

constexpr year operator+(const years& ys, const year& y) noexcept
{
	return year(int(y) + ys.count());
}

constexpr year operator-(const year& y, const years& ys) noexcept
{
	return year(int(y) - ys.count());
}

constexpr years operator-(const year& y1, const year& y2) noexcept
{
	return years(int(y1) - int(y2));
}

//template <class charT, class traits>
//basic_ostream<charT, traits>& operator<<(basic_ostream<charT, traits>& os, const year& y);
//template <class charT, class traits, class Alloc = allocator<charT>>
//basic_istream<charT, traits>&
//from_stream(basic_istream<charT, traits>& is, const charT* fmt,
//			year& y, basic_string<charT, traits, Alloc>* abbrev = nullptr,
//			minutes* offset = nullptr);

// 27.8.6, class weekday
class weekday;
constexpr weekday operator+(const weekday& x, const days& y) noexcept;
constexpr weekday operator-(const weekday& x, const days& y) noexcept;

class weekday_indexed;
class weekday_last;

class weekday
{
public:
	weekday() = default;
	explicit constexpr weekday(unsigned wd) noexcept : 
		_wd(wd % 7) {};
	
	constexpr weekday(const std::chrono::sys_days& sd) noexcept : 
		_wd()
	{
		detail::__modulo(sd.time_since_epoch().count() + 4, 7); // epoch is a Thursday
	};
	
	explicit constexpr weekday(const std::chrono::local_days& dp) noexcept : 
		weekday(std::chrono::sys_days{ dp.time_since_epoch() }) 
	{}

	constexpr std::chrono::weekday& operator++() noexcept
	{
		*this += std::chrono::days{ 1 };
		return *this;
	}

	constexpr std::chrono::weekday operator++(int) noexcept
	{
		weekday ret = weekday{ _wd };
		++(*this);
		return ret;
	}

	constexpr std::chrono::weekday& operator--() noexcept
	{
		*this -= std::chrono::days{ 1 };
		return *this;
	}
	constexpr std::chrono::weekday operator--(int) noexcept
	{
		weekday ret = weekday{ _wd };
		--(*this);
		return ret;
	}

	constexpr std::chrono::weekday& operator+=(const std::chrono::days& d) noexcept
	{
		*this = *this + d;
		return *this;
	}

	constexpr std::chrono::weekday& operator-=(const std::chrono::days& d) noexcept
	{
		*this = *this - d;
		return *this;
	}

	constexpr unsigned c_encoding() const noexcept
	{
		return _wd;
	}

	constexpr unsigned iso_encoding() const noexcept
	{
		return c_encoding() == 0u ? 7u : c_encoding();
	}

	constexpr bool ok() const noexcept
	{
		return _wd <= 6;
	}

	constexpr weekday_indexed operator[](unsigned index) const noexcept;
	constexpr weekday_last operator[](std::chrono::last_spec) const noexcept;

private:
	unsigned _wd;
};

inline constexpr weekday Sunday{ 0 };
inline constexpr weekday Monday{ 1 };
inline constexpr weekday Tuesday{ 2 };
inline constexpr weekday Wednesday{ 3 };
inline constexpr weekday Thursday{ 4 };
inline constexpr weekday Friday{ 5 };
inline constexpr weekday Saturday{ 6 };

constexpr bool operator==(const weekday& x, const weekday& y) noexcept
{
	return x.c_encoding() == y.c_encoding();
}

constexpr weekday operator+(const weekday& wd, const days& ds) noexcept
{
	auto n = static_cast<long long>(wd.c_encoding()) + ds.count();
	return weekday{ detail::__modulo(n, 7) };
}

constexpr weekday operator+(const days& ds, const weekday& wd) noexcept
{
	return wd + ds;
}

constexpr weekday operator-(const weekday& wd, const days& ds) noexcept
{
	return wd + -ds;
}

constexpr days operator-(const weekday& wd1, const weekday& wd2) noexcept
{
	const auto dwd = int(wd1.c_encoding()) - int(wd2.c_encoding());
	return days{ dwd < 0 ? 7 + dwd : dwd };
}

//template <class charT, class traits>
//basic_ostream<charT, traits>& operator<<(basic_ostream<charT, traits>& os, const weekday& wd);
//template <class charT, class traits, class Alloc = allocator<charT>>
//basic_istream<charT, traits>&
//from_stream(basic_istream<charT, traits>& is, const charT* fmt,
//			weekday& wd, basic_string<charT, traits, Alloc>* abbrev = nullptr,
//			minutes* offset = nullptr);

// 27.8.7, class weekday_indexed
class weekday_indexed 
{
public:
	weekday_indexed() = default;
	constexpr weekday_indexed(const chrono::weekday& wd, unsigned index) noexcept : _wd(wd), _index(index) {}

	constexpr chrono::weekday weekday() const noexcept
	{
		return _wd;
	}

	constexpr unsigned index() const noexcept
	{
		return _index;
	}

	constexpr bool ok() const noexcept
	{
		return weekday().ok() && 1 <= index() && index() <= 5;
	}

private:
	chrono::weekday _wd;
	unsigned _index;
};
constexpr bool operator==(const weekday_indexed& x, const weekday_indexed& y) noexcept
{
	return x.weekday() == y.weekday() && x.index() == y.index();
}

//template <class charT, class traits>
//basic_ostream<charT, traits>& operator<<(basic_ostream<charT, traits>& os, const weekday_indexed& wdi);

// 27.8.8, class weekday_last
class weekday_last
{
public:
	explicit constexpr weekday_last(const std::chrono::weekday& wd) noexcept : _wd(wd) {}

	constexpr chrono::weekday weekday() const noexcept
	{
		return _wd;
	}
	constexpr bool ok() const noexcept
	{
		return weekday().ok();
	}

private:
	chrono::weekday _wd;
};
constexpr bool operator==(const weekday_last& x, const weekday_last& y) noexcept
{
	return x.weekday() == y.weekday();
}
//template <class charT, class traits>
//basic_ostream<charT, traits>& operator<<(basic_ostream<charT, traits>& os, const weekday_last& wdl);

constexpr weekday_indexed weekday::operator[](unsigned index) const noexcept
{
	return weekday_indexed(*this, index);
}

constexpr weekday_last weekday::operator[](std::chrono::last_spec) const noexcept
{
	return weekday_last(*this);
}

// 27.8.9, class month_day
class month_day;
constexpr bool operator==(const month_day& x, const month_day& y) noexcept;
//constexpr strong_ordering operator<=>(const month_day& x, const month_day& y) noexcept;
//template <class charT, class traits>
//basic_ostream<charT, traits>& operator<<(basic_ostream<charT, traits>& os, const month_day& md);
//template <class charT, class traits, class Alloc = allocator<charT>>
//basic_istream<charT, traits>&
//from_stream(basic_istream<charT, traits>& is, const charT* fmt,
//			month_day& md, basic_string<charT, traits, Alloc>* abbrev = nullptr,
//			minutes* offset = nullptr);

// 27.8.10, class month_day_last
class month_day_last;
constexpr bool operator==(const month_day_last& x, const month_day_last& y) noexcept;
//constexpr strong_ordering operator<=>(const month_day_last& x,
//									  const month_day_last& y) noexcept;
//template <class charT, class traits>
//basic_ostream<charT, traits>& operator<<(basic_ostream<charT, traits>& os, const month_day_last& mdl);

// 27.8.11, class month_weekday
class month_weekday;
constexpr bool operator==(const month_weekday& x, const month_weekday& y) noexcept;
//template <class charT, class traits>
//basic_ostream<charT, traits>& operator<<(basic_ostream<charT, traits>& os, const month_weekday& mwd);

// 27.8.12, class month_weekday_last
class month_weekday_last;
constexpr bool operator==(const month_weekday_last& x, const month_weekday_last& y) noexcept;
//template <class charT, class traits>
//basic_ostream<charT, traits>& operator<<(basic_ostream<charT, traits>& os, const month_weekday_last& mwdl);

// 27.8.13, class year_month
class year_month;
constexpr bool operator==(const year_month& x, const year_month& y) noexcept;
//constexpr strong_ordering operator<=>(const year_month& x, const year_month& y) noexcept;
constexpr year_month operator+(const year_month& ym, const months& dm) noexcept;
constexpr year_month operator+(const months& dm, const year_month& ym) noexcept;
constexpr year_month operator-(const year_month& ym, const months& dm) noexcept;
constexpr months operator-(const year_month& x, const year_month& y) noexcept;
constexpr year_month operator+(const year_month& ym, const years& dy) noexcept;
constexpr year_month operator+(const years& dy, const year_month& ym) noexcept;
constexpr year_month operator-(const year_month& ym, const years& dy) noexcept;
//template <class charT, class traits>
//basic_ostream<charT, traits>& operator<<(basic_ostream<charT, traits>& os, const year_month& ym);
//template <class charT, class traits, class Alloc = allocator<charT>>
//basic_istream<charT, traits>&
//from_stream(basic_istream<charT, traits>& is, const charT* fmt,
//			year_month& ym, basic_string<charT, traits, Alloc>* abbrev = nullptr,
//			minutes* offset = nullptr);

// 27.8.14, class year_month_day
class year_month_day;
constexpr bool operator==(const year_month_day& x, const year_month_day& y) noexcept;
//constexpr strong_ordering operator<=>(const year_month_day& x,
//									  const year_month_day& y) noexcept;
constexpr year_month_day operator+(const year_month_day& ymd, const months& dm) noexcept;
constexpr year_month_day operator+(const months& dm, const year_month_day& ymd) noexcept;
constexpr year_month_day operator+(const year_month_day& ymd, const years& dy) noexcept;
constexpr year_month_day operator+(const years& dy, const year_month_day& ymd) noexcept;
constexpr year_month_day operator-(const year_month_day& ymd, const months& dm) noexcept;
constexpr year_month_day operator-(const year_month_day& ymd, const years& dy) noexcept;
//template <class charT, class traits>
//basic_ostream<charT, traits>& operator<<(basic_ostream<charT, traits>& os, const year_month_day& ymd);
//template <class charT, class traits, class Alloc = allocator<charT>>
//basic_istream<charT, traits>&
//from_stream(basic_istream<charT, traits>& is, const charT* fmt,
//			year_month_day& ymd,
//			basic_string<charT, traits, Alloc>* abbrev = nullptr,
//			minutes* offset = nullptr);

// 27.8.15, class year_month_day_last
class year_month_day_last;
constexpr bool operator==(const year_month_day_last& x,
						  const year_month_day_last& y) noexcept;
//constexpr strong_ordering operator<=>(const year_month_day_last& x,
//									  const year_month_day_last& y) noexcept;
constexpr year_month_day_last operator+(const year_month_day_last& ymdl, const months& dm) noexcept;
constexpr year_month_day_last operator+(const months& dm, const year_month_day_last& ymdl) noexcept;
constexpr year_month_day_last operator+(const year_month_day_last& ymdl, const years& dy) noexcept;
constexpr year_month_day_last operator+(const years& dy, const year_month_day_last& ymdl) noexcept;
constexpr year_month_day_last operator-(const year_month_day_last& ymdl, const months& dm) noexcept;
constexpr year_month_day_last operator-(const year_month_day_last& ymdl, const years& dy) noexcept;
//template <class charT, class traits>
//basic_ostream<charT, traits>& operator<<(basic_ostream<charT, traits>& os, const year_month_day_last& ymdl);

// 27.8.16, class year_month_weekday
class year_month_weekday;
constexpr bool operator==(const year_month_weekday& x,
						  const year_month_weekday& y) noexcept;
constexpr year_month_weekday operator+(const year_month_weekday& ymwd, const months& dm) noexcept;
constexpr year_month_weekday operator+(const months& dm, const year_month_weekday& ymwd) noexcept;
constexpr year_month_weekday operator+(const year_month_weekday& ymwd, const years& dy) noexcept;
constexpr year_month_weekday operator+(const years& dy, const year_month_weekday& ymwd) noexcept;
constexpr year_month_weekday operator-(const year_month_weekday& ymwd, const months& dm) noexcept;
constexpr year_month_weekday operator-(const year_month_weekday& ymwd, const years& dy) noexcept;
//template <class charT, class traits>
//basic_ostream<charT, traits>& operator<<(basic_ostream<charT, traits>& os, const year_month_weekday& ymwdi);

// 27.8.17, class year_month_weekday_last
class year_month_weekday_last;
constexpr bool operator==(const year_month_weekday_last& x,
						  const year_month_weekday_last& y) noexcept;
constexpr year_month_weekday_last operator+(const year_month_weekday_last& ymwdl, const months& dm) noexcept;
constexpr year_month_weekday_last operator+(const months& dm, const year_month_weekday_last& ymwdl) noexcept;
constexpr year_month_weekday_last operator+(const year_month_weekday_last& ymwdl, const years& dy) noexcept;
constexpr year_month_weekday_last operator+(const years& dy, const year_month_weekday_last& ymwdl) noexcept;
constexpr year_month_weekday_last operator-(const year_month_weekday_last& ymwdl, const months& dm) noexcept;
constexpr year_month_weekday_last operator-(const year_month_weekday_last& ymwdl, const years& dy) noexcept;
//template <class charT, class traits>
//basic_ostream<charT, traits>& operator<<(basic_ostream<charT, traits>& os, const year_month_weekday_last& ymwdl);

// 27.8.18, civil calendar conventional syntax operators
constexpr year_month operator/(const year& y, const month& m) noexcept;
constexpr year_month operator/(const year& y, int m) noexcept;
constexpr month_day operator/(const month& m, const day& d) noexcept;
constexpr month_day operator/(const month& m, int d) noexcept;
constexpr month_day operator/(int m, const day& d) noexcept;
constexpr month_day operator/(const day& d, const month& m) noexcept;
constexpr month_day operator/(const day& d, int m) noexcept;
constexpr month_day_last operator/(const month& m, last_spec) noexcept;
constexpr month_day_last operator/(int m, last_spec) noexcept;
constexpr month_day_last operator/(last_spec, const month& m) noexcept;
constexpr month_day_last operator/(last_spec, int m) noexcept;
constexpr month_weekday operator/(const month& m, const weekday_indexed& wdi) noexcept;
constexpr month_weekday operator/(int m, const weekday_indexed& wdi) noexcept;
constexpr month_weekday operator/(const weekday_indexed& wdi, const month& m) noexcept;
constexpr month_weekday operator/(const weekday_indexed& wdi, int m) noexcept;
constexpr month_weekday_last operator/(const month& m, const weekday_last& wdl) noexcept;
constexpr month_weekday_last operator/(int m, const weekday_last& wdl) noexcept;
constexpr month_weekday_last operator/(const weekday_last& wdl, const month& m) noexcept;
constexpr month_weekday_last operator/(const weekday_last& wdl, int m) noexcept;
constexpr year_month_day operator/(const year_month& ym, const day& d) noexcept;
constexpr year_month_day operator/(const year_month& ym, int d) noexcept;
constexpr year_month_day operator/(const year& y, const month_day& md) noexcept;
constexpr year_month_day operator/(int y, const month_day& md) noexcept;
constexpr year_month_day operator/(const month_day& md, const year& y) noexcept;
constexpr year_month_day operator/(const month_day& md, int y) noexcept;
constexpr year_month_day_last operator/(const year_month& ym, last_spec) noexcept;
constexpr year_month_day_last operator/(const year& y, const month_day_last& mdl) noexcept;
constexpr year_month_day_last operator/(int y, const month_day_last& mdl) noexcept;
constexpr year_month_day_last operator/(const month_day_last& mdl, const year& y) noexcept;
constexpr year_month_day_last operator/(const month_day_last& mdl, int y) noexcept;
constexpr year_month_weekday operator/(const year_month& ym, const weekday_indexed& wdi) noexcept;
constexpr year_month_weekday operator/(const year& y, const month_weekday& mwd) noexcept;
constexpr year_month_weekday operator/(int y, const month_weekday& mwd) noexcept;
constexpr year_month_weekday operator/(const month_weekday& mwd, const year& y) noexcept;
constexpr year_month_weekday operator/(const month_weekday& mwd, int y) noexcept;
constexpr year_month_weekday_last operator/(const year_month& ym, const weekday_last& wdl) noexcept;
constexpr year_month_weekday_last operator/(const year& y, const month_weekday_last& mwdl) noexcept;
constexpr year_month_weekday_last operator/(int y, const month_weekday_last& mwdl) noexcept;
constexpr year_month_weekday_last operator/(const month_weekday_last& mwdl, const year& y) noexcept;
constexpr year_month_weekday_last operator/(const month_weekday_last& mwdl, int y) noexcept;
}

inline namespace literals {
inline namespace chrono_literals {

// 27.8.3.3, non-member functions
constexpr chrono::day operator ""d(unsigned long long d) noexcept
{
	return chrono::day(d);
}

// 27.8.5.3, non-member functions
constexpr chrono::year operator ""y(unsigned long long y) noexcept
{
	return chrono::year(static_cast<int>(y));
}

}
}

}
