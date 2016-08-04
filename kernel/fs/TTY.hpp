/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#ifndef _TTY_HPP_
#define _TTY_HPP_

#include <termios.h>
#include <fs/File.hpp>

#define BUFFER_SIZE 1024

class InputBuffer {
public:
	typedef char value_type;
	typedef size_t size_type;
	typedef value_type& reference;

	InputBuffer() : start(0), end(0), _size(0) {}

	void push_back(const value_type& val) {
		if (_size < BUFFER_SIZE) {
			buffer[end] = val;
			end = (end+1) % BUFFER_SIZE;
			_size++;
		}
	}

	reference front() {
		return buffer[start];
	}

	reference back() {
		size_t last = (end + BUFFER_SIZE - 1) % BUFFER_SIZE;
		return buffer[last];
	}

	void pop_back() {
		end = (end + BUFFER_SIZE - 1) % BUFFER_SIZE;
		_size--;
	}

	void pop_front() {
		start = (start + 1) % BUFFER_SIZE;
		_size--;
	}

	size_type size() const {
		return _size;
	}

private:
	value_type buffer[BUFFER_SIZE];
	int start, end, _size;
};

class TTY : public File {
public:
	TTY() : buffer(), nbDelim(0){
		initTermios();
	}
	~TTY() {}

	size_t read(void* ptr, size_t count);

	size_t write(void* ptr, size_t count);

	int ioctl(size_t request, void* argp);

	int addInput(const char val);

private:
	int getTermios(struct termios*);
	int setTermios(struct termios*);

	void initTermios() {
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

#endif // _TTY_HPP_
