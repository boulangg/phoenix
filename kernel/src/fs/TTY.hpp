/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#ifndef _TTY_HPP_
#define _TTY_HPP_

#include <termios.h>
#include <fs/vfs/File.hpp>




/*class TTY : public File {
public:
	TTY() : buffer(), nbDelim(0){
		initTermios();
	}
	~TTY() {}

	//size_t write(void* ptr, size_t count);

	int ioctl(size_t request, void* argp);

	int addInput(const char val);

private:
	virtual ssize_t read_internal(char* ptr, size_t count, loff_t offset) override;
	virtual ssize_t write_internal(char* ptr, size_t count, loff_t offset) override;

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

};*/

#endif // _TTY_HPP_
