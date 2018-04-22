#include "ThreadPool.h"
#include "MultithreadingConstant.h"
#include <iostream>

ThreadPool::ThreadPool(size_t capacity)
    : _threads(0)
{
    _threads.reserve(capacity);
    for (int i = 0; i < capacity; ++i) {
        _threads.push_back(Thread::Ptr(new Thread()));
    }
}

Thread::Ptr &ThreadPool::getFreeThread() {
    if (_stopping)
        goto throw_exception;

    for (Thread::Ptr &thread : _threads) {
        if (!thread->isUsed()) {
            return thread;
        }
    }

    throw_exception:
    throw ExhausedThreads();
}

ThreadPool::~ThreadPool() {
    stopAllThreads();
    waitAll();
}

bool ThreadPool::start(Runnable::Ptr &&runObj) {
    try {
        getFreeThread()->setRunnable(std::move(runObj));
    } catch (ExhausedThreads &e) {
        std::cerr << e.what();
        return false;
    }

    return true;
}

void ThreadPool::stopAllThreads() {
    for (Thread::Ptr &p : _threads)
        p->stopLoop();
}

void ThreadPool::waitAll() {
    while (true) {
        Thread::sleep(DELAY_THREAD_MSEC);
        for (Thread::Ptr &p : _threads)
            if (!p->isStopped())
                continue;
        break;
    }
}
