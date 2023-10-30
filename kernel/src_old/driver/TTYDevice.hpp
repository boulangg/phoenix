#pragma once

#include <termios.h>

#include <core/Console.hpp>

#include <proc/ProcessScheduler.hpp>

#include <driver/input/InputEventCode.hpp>
#include <driver/input/InputHandler.hpp>
#include <driver/input/InputManager.hpp>
#include <driver/CharacterDevice.hpp>

#define NEW_LINE '\n'

struct keybind
{
	char map[256];
};

extern struct keybind normal_keybind;

#define BUFFER_SIZE 1024

class InputBuffer
{
public:
	typedef char value_type;
	typedef size_t size_type;
	typedef value_type& reference;

	InputBuffer() : start(0), end(0), _size(0)
	{}

	void push_back(const value_type& val)
	{
		if (_size < BUFFER_SIZE) {
			buffer[end] = val;
			end = (end + 1) % BUFFER_SIZE;
			_size++;
		}
	}

	reference front()
	{
		return buffer[start];
	}

	reference back()
	{
		size_t last = (end + BUFFER_SIZE - 1) % BUFFER_SIZE;
		return buffer[last];
	}

	void pop_back()
	{
		end = (end + BUFFER_SIZE - 1) % BUFFER_SIZE;
		_size--;
	}

	void pop_front()
	{
		start = (start + 1) % BUFFER_SIZE;
		_size--;
	}

	size_type size() const
	{
		return _size;
	}

private:
	value_type buffer[BUFFER_SIZE];
	int start, end, _size;
};

class TTYDevice : public InputHandler, public CharacterDevice
{
public:
	TTYDevice(dev_t devID) : InputHandler(), CharacterDevice(), buffer(), nbDelim(0)
	{
		initTermios();
		setDeviceID(devID);
		InputManager::registerHandler(this);
	}

	virtual ~TTYDevice()
	{}

//virtual dev_t getDeviceID();

/*virtual int connect(InputDevice *dev, const struct input_device_id *id) override {

}*/

/*virtual void disconnect(InputDevice *handle) override {

}*/

	virtual ssize_t read(char* ptr, size_t count, loff_t) override
	{
		char* buf = (char*)ptr;
		size_t curr = 0;
		if (tios.c_lflag & ICANON) {
			while (nbDelim == 0) {
				Event ev(Event::EventType::FileEvent, (uint64_t)this);
				ProcessScheduler::wait(ev);
			}
			while ((buffer.size() > 0) && (curr < count)) {
				buf[curr] = buffer.front();
				buffer.pop_front();
				curr++;
				if (buf[curr - 1] == NEW_LINE) {
					nbDelim--;
					break;
				}
			}
		} else {
			while ((buffer.size() < (size_t)tios.c_cc[VMIN]) && (buffer.size() < count)) {
				Event ev(Event::EventType::FileEvent, (uint64_t)this);
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

	virtual ssize_t write(const char* ptr, size_t count, loff_t) override
	{
		char* str = (char*)ptr;
		size_t i = 0;
		for (; i < count; i++) {
			Console::write(str[i]);
		}
		return i;
	}

	virtual void handleEvents(const std::vector<InputValue>& vals)
	{
		for (auto inputVal : vals) {
			if (inputVal.value == 0) {
				continue;
			}
			if (inputVal.type != EV_KEY) {
				continue;
			}
			int32_t code = inputVal.code;
			char val = normal_keybind.map[code];
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
					Event ev(Event::EventType::FileEvent, (uint64_t)this);
					ProcessScheduler::wakeUp(ev);
				}
			} else {
				buffer.push_back(val);
				Event ev(Event::EventType::FileEvent, (uint64_t)this);
				ProcessScheduler::wakeUp(ev);
			}
		}
	}


private:
	int getTermios(struct termios*);
	int setTermios(struct termios*);

	void initTermios()
	{
		tios.c_cc[VEOF] = 0;
		tios.c_cc[VEOL] = 0;
		tios.c_cc[VERASE] = '\b';
		tios.c_cc[VINTR] = 0;
		tios.c_cc[VKILL] = 0;
		tios.c_cc[VMIN] = 5;
		tios.c_cc[VQUIT] = 0;
		tios.c_cc[VSTART] = 0;
		tios.c_cc[VSTOP] = 0;
		tios.c_cc[VSUSP] = 0;
		tios.c_cc[VTIME] = 0;

		tios.c_iflag = 0;
		tios.c_oflag = 0;
		tios.c_cflag = 0;
		tios.c_lflag = ECHO | ICANON;
		tios.c_ispeed = 0;
		tios.c_ospeed = 0;
	}

	struct termios tios;
	InputBuffer buffer;
	size_t nbDelim;



};
