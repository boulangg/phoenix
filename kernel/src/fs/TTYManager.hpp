/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#ifndef _TTYManager_HPP_
#define _TTYManager_HPP_

#include <stddef.h>
#include <fs/TTY.hpp>

#define MAX_TTY 16

/*class TTYManager {
public:
	static TTY* getActiveTTY();
	static TTY* getTTY(size_t num);
	static int setActiveTTY(size_t num);

private:
	static TTY *ttys[MAX_TTY];
	static size_t activeTTY;
};*/

#endif // _TTYManager_HPP_
