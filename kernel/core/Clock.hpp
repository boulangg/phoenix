/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#ifndef KERNEL_CORE_CLOCK_HPP_
#define KERNEL_CORE_CLOCK_HPP_

#include <cstdint>

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
	static void setFreq();
	static uint64_t sec(){return s;}
	static uint64_t min(){return m;}
	static uint64_t hour(){return h;}
	static uint64_t current_clock_kernel(){return totalIntern;}
	static void tic();
private:
	static uint64_t s;
	static uint64_t m;
	static uint64_t h;
	static uint64_t intern;
	static uint64_t totalIntern;
};

#endif /* KERNEL_CORE_CLOCK_HPP_ */
