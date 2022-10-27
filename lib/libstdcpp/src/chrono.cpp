#include <chrono>

#include <time.h>

namespace std {
namespace chrono {

system_clock::time_point system_clock::now() noexcept
{
	struct ::timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	auto dur = chrono::round<system_clock::duration>(seconds{ ts.tv_sec } + nanoseconds{ ts.tv_nsec });
	return system_clock::time_point{ dur };
}

std::time_t system_clock::to_time_t(const time_point& t) noexcept
{
	seconds sec = chrono::round<seconds>(t.time_since_epoch());
	return sec.count();
}

system_clock::time_point system_clock::from_time_t(std::time_t t) noexcept
{
	auto dur = chrono::round<system_clock::duration>(seconds{ t });
	return system_clock::time_point{ dur };
}

steady_clock::time_point steady_clock::now() noexcept
{
	struct ::timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	auto dur = chrono::round<steady_clock::duration>(nanoseconds{ ts.tv_nsec });
	return steady_clock::time_point{ dur };
}

}

}
