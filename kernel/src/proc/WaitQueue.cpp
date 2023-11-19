/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #include "WaitQueue.h"

#include "Kernel.h"

namespace kernel::proc {

class WaitQueueEntry
{
public:
    WaitQueueEntry(bool exclusive, Process* process) : _exclusive(exclusive), _process(process) {}

private:
    friend class WaitQueue;

    bool _exclusive;
    Process* _process;
};

WaitQueue::WaitQueue(){};

WaitQueueEntry* WaitQueue::queue(bool exclusive)
{
    Process* current = Kernel::scheduler.getCurrent();
    WaitQueueEntry* entry = new WaitQueueEntry(exclusive, current);
    _lock.lock();
    Kernel::scheduler.changeState(entry->_process, Process::State::Sleeping);
    if (entry->_exclusive) {
        _queue.push_back(entry);
    } else {
        _queue.push_front(entry);
    }
    _lock.unlock();
    return entry;
}

void WaitQueue::refreshState(WaitQueueEntry* entry)
{
    Kernel::scheduler.changeState(entry->_process, Process::State::Sleeping);
}

void WaitQueue::dequeue(WaitQueueEntry* entry)
{
    _lock.lock();
    _queue.remove(entry);
    Kernel::scheduler.changeState(entry->_process, Process::State::Running);
    _lock.unlock();
}

void WaitQueue::wake_up()
{
    _lock.lock();
    for (auto& entry : _queue) {
        Kernel::scheduler.changeState(entry->_process, Process::State::Running);
        if (entry->_exclusive) {
            break;
        }
    }
    _lock.unlock();
}

}
