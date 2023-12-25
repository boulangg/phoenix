/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "Page.h"

#include "KernelGlobals.h"
#include "proc/WaitQueue.h"

namespace kernel::mem {


static constexpr std::size_t PAGE_LOCK_WAIT_QUEUE_SIZE = 64;
proc::WaitQueue PageLockWaitQueues[PAGE_LOCK_WAIT_QUEUE_SIZE];

void Page::lock() 
{
    wait_event(PageLockWaitQueues[0], !_locked.test_and_set());
}

void Page::unlock()
{
    _locked.clear();
    PageLockWaitQueues[0].wake_up();
}

}
