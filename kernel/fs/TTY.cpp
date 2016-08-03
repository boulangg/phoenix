#include <fs/TTY.hpp>

#include <proc/ProcessScheduler.hpp>
#include <core/Console.hpp>

size_t TTY::read(void* ptr, size_t count) {
		char* buf = (char*)ptr;
		size_t curr = 0;
		while (buffer.size() == 0) {
			Event ev(Event::EventType::FileEvent, 0);
			ProcessScheduler::wait(ev);
		}
		while ((buffer.size() > 0) && (curr < count)) {
			buf[curr] = buffer.front();
			buffer.pop_front();
			curr++;
		}
		return curr;
	}

size_t TTY::write(void* ptr, size_t count) {
	char* str = (char*)ptr;
	size_t i = 0;
	for (; i < count; i++) {
		Console::write(str[i]);
	}
	return i;
}

int TTY::addInput(const char val) {
	buffer.push_back(val);
	Event ev(Event::EventType::FileEvent, 0);
	ProcessScheduler::wakeUp(ev);
	Console::write(val);
	return 0;
}
