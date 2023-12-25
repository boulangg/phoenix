/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

#include <list>

#include "asm/spinlock.h"

namespace kernel::proc {

class WaitQueueEntry;

class WaitQueue
{
public:
    WaitQueue();

    WaitQueueEntry* queue(bool exclusive);
    void dequeue(WaitQueueEntry* entry);
    void refreshState(WaitQueueEntry* entry);

    void wake_up();

private:
    spinlock _lock;
    std::list<WaitQueueEntry*> _queue;
};

#define wait_event(wq, condition)                                                                                      \
    kernel::proc::WaitQueueEntry* entry = wq.queue(false);                                                             \
    for (;;) {                                                                                                         \
        wq.refreshState(entry);                                                                                        \
        if (condition) {                                                                                               \
            break;                                                                                                     \
        }                                                                                                              \
        schedule();                                                                                                    \
    }                                                                                                                  \
    wq.dequeue(entry);

#define wait_event_exclusive(wq, condition)                                                                            \
    kernel::proc::WaitQueueEntry* entry = wq.queue(true);                                                              \
    for (;;) {                                                                                                         \
        wq.refreshState(entry);                                                                                        \
        if (condition) {                                                                                               \
            break;                                                                                                     \
        }                                                                                                              \
        schedule();                                                                                                    \
    }                                                                                                                  \
    wq.dequeue(entry);

}
