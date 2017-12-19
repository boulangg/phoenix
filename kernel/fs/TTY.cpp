/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include <fs/TTY.hpp>

#include <proc/ProcessScheduler.hpp>
#include <core/Console.hpp>

#define NEW_LINE '\n'

ssize_t TTY::read_internal(char* ptr, size_t count, loff_t) {
		char* buf = (char*)ptr;
		size_t curr = 0;
		if (tios.c_lflag & ICANON) {
			while (nbDelim == 0) {
				Event ev(Event::EventType::FileEvent, 0);
				ProcessScheduler::wait(ev);
			}
			while ((buffer.size() > 0) && (curr < count)) {
				buf[curr] = buffer.front();
				buffer.pop_front();
				curr++;
				if (buf[curr-1] == NEW_LINE) {
					nbDelim--;
					break;
				}
			}
		} else {
			while ((buffer.size() < (size_t)tios.c_cc[VMIN]) && (buffer.size() < count)) {
				Event ev(Event::EventType::FileEvent, 0);
				ProcessScheduler::wait(ev);
			}
			while ((buffer.size() > 0) && (curr < count)) {
				buf[curr] = buffer.front();
				buffer.pop_front();
				curr++;
			}
		}
		return curr;
	}

ssize_t TTY::write_internal(char* ptr, size_t count, loff_t) {
	char* str = (char*)ptr;
	size_t i = 0;
	for (; i < count; i++) {
		Console::write(str[i]);
	}
	return i;
}

int TTY::addInput(const char val) {
	//buffer.push_back(val);
	if (tios.c_lflag & ECHO) {
		Console::write(val);
	}
	if (tios.c_lflag & ICANON) {
		if (val == tios.c_cc[VERASE]) {
			if (buffer.back() != NEW_LINE) {
				buffer.pop_back();
			}
		} else {
			buffer.push_back(val);
		}
		if (val == NEW_LINE) {
			nbDelim++;
			Event ev(Event::EventType::FileEvent, 0);
			ProcessScheduler::wakeUp(ev);
		}
	} else {
		buffer.push_back(val);
		Event ev(Event::EventType::FileEvent, 0);
		ProcessScheduler::wakeUp(ev);
	}
	return 0;
}
