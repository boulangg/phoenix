/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

#include "WaitQueue.h"
#include "asm/spinlock.h"

namespace kernel::proc {

class Semaphore
{
public:
    Semaphore(std::uint64_t maxUnits, std::uint64_t initialUnits) :
        _lock(), _queue(), _maxUnits(maxUnits), _units(initialUnits)
    {}

    void wait(std::uint64_t units)
    {
        for (;;) {
            wait_event(_queue, _units + units > _maxUnits);
            _lock.lock();
            if (_units + units <= _maxUnits) {
                _units += units;
                _lock.unlock();
                break;
            }
            _lock.unlock();
        }
    }

    void signal(std::uint64_t units)
    {
        _lock.lock();
        _units -= units;
        _lock.unlock();
        _queue.wake_up();
    }

private:
    spinlock _lock;
    WaitQueue _queue;
    std::uint64_t _maxUnits;
    std::uint64_t _units;
};

}
