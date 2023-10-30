/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

#include <atomic>

#include "cpu.h"

namespace kernel {

class spinlock
{
public:
    void lock()
    {
        _interruptEnabled = isInterruptEnabled();
        cli();
        while (_state.test_and_set()) {
            do {
                pause();
            } while (_state.test());
        }
    }

    void unlock()
    {
        _state.clear();
        if (!_interruptEnabled) {
            sti();
        }
    }

private:
    std::atomic_flag _state;
    bool _interruptEnabled;
};

}
