#pragma once

#include <string>
#include <cstdint>
#include <bitset>
#include <list>
#include <vector>

#include <sys/times.h>
#include <asm/io.h>

#include "InputEventCode.hpp"
#include <driver/CharacterDevice.hpp>
#include <core/interrupt/InterruptManager.hpp>

struct InputEvent {
	struct timeval time;
	uint16_t type;
	uint16_t code;
	int32_t value;
};


#include <termios.h>

#define BUFFER_SIZE 1024


#include <proc/ProcessScheduler.hpp>
#include <core/Console.hpp>
#include <fs/TTY.hpp>
#include <driver/input/KeyboardDevice.hpp>


