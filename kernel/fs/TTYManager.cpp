/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include <fs/TTYManager.hpp>

TTY* TTYManager::ttys[MAX_TTY] = {};
size_t TTYManager::activeTTY = 0;


TTY* TTYManager::getActiveTTY() {
	if (ttys[activeTTY] == nullptr) {
		ttys[activeTTY] = new TTY();
	}
	return ttys[activeTTY];
}

TTY* TTYManager::getTTY(size_t num) {
	if (num < MAX_TTY) {
		if (ttys[num] == nullptr) {
			ttys[num] = new TTY();
		}
		return ttys[num];
	}
	return nullptr;
}

int TTYManager::setActiveTTY(size_t num) {
	if (num < MAX_TTY) {
		activeTTY = num;
		return 0;
	}
	return -1;
}
