/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "Clock.hpp"

#include "Console.hpp"
#include <asm/io.h>
#include <stdio.h>
#include <proc/ProcessScheduler.hpp>

#define QUARTZ 0x1234DD
#define CLOCKFREQ 60

uint64_t Clock::h = 0;
uint64_t Clock::m = 0;
uint64_t Clock::s = 0;
uint64_t Clock::intern = 0;
uint64_t Clock::totalIntern = 0;
std::list<Event> Clock::timers;

void PIT_handler()
{
	Clock::tic();
}

int Clock::tic()
{
	totalIntern++;
	intern++;
	if (intern >= CLOCKFREQ) {
		intern = 0;
		s++;
	}
	if (s >= 60) {
		s = 0;
		m++;
	}
	if (m >= 60) {
		m = 0;
		h++;
	}
	checkTimers();
	ProcessScheduler::schedule();
	return 0;
}

Clock::Clock()
{}

Clock::~Clock()
{}

void Clock::checkTimers()
{
	for (auto it = timers.begin(); it != timers.end();) {
		auto ev = *it;
		if (ev.getId() <= s) {
			ProcessScheduler::wakeUp(ev);
			it = timers.erase(it);
		} else {
			it++;
		}
	}
}

void Clock::setFreq()
{
	outb(0x43, 0x34);
	outb(0x40, (QUARTZ / CLOCKFREQ) % 256);
	outb(0x40, (QUARTZ / CLOCKFREQ) >> 8);
}

int Clock::nanosleep(const struct timespec* req, struct timespec*)
{
	Event ev(Event::EventType::TimerEvent, s + req->tv_sec);
	timers.push_back(ev);
	ProcessScheduler::wait(ev);
	//ProcessScheduler::sleep();
	return 0;
}
