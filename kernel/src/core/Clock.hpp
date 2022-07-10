/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#ifndef KERNEL_CORE_CLOCK_HPP_
#define KERNEL_CORE_CLOCK_HPP_

#include <cstdint>
#include <time.h>
#include <list>
#include <proc/ProcessScheduler.hpp>

#include <core/interrupt/InterruptManager.hpp>

#ifdef __cplusplus
extern "C" {
#endif

void PIT_handler();

#ifdef __cplusplus
}
#endif

class Clock {
public:
	Clock();
	~Clock();
	static void init() {
		setFreq();
		InterruptHandler* handler = new InterruptHandlerFunction<Clock::tic>("Clock", {true, false}, nullptr);
		InterruptManager::requestIRQ(0, handler);
	}

	static void setFreq();
	static uint64_t sec(){return s;}
	static uint64_t min(){return m;}
	static uint64_t hour(){return h;}
	static uint64_t current_clock_kernel(){return totalIntern;}
	static int tic();
	static int nanosleep(const struct timespec *req, struct timespec *rem);

private:
	static void checkTimers();

	static uint64_t s;
	static uint64_t m;
	static uint64_t h;
	static uint64_t intern;
	static uint64_t totalIntern;
	static std::list<Event> timers;
};

#endif /* KERNEL_CORE_CLOCK_HPP_ */
