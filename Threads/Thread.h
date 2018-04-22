#ifndef THREAD_H
#define THREAD_H

#include "Runnable.h"
#include <memory>
#include <future>
#include <atomic>

class Thread {
    Runnable::Ptr _runnable;
    std::atomic_bool _isUsed;
    std::atomic_bool _isWaiting;
    std::atomic_bool _isStopped;
    std::atomic_bool _isActive;
    std::future<void> _thrFuture;
    void mainLoop();

public:
    Thread();
    using Ptr = std::unique_ptr<Thread>;
    void setRunnable(Runnable::Ptr &&runnable);
    static void sleep(size_t msec);

    bool isWaiting() const { return !_isUsed && !_isStopped; }
    bool isUsed() const { return _isUsed; }
    bool isStopped() const { return _isStopped; }
    void stopLoop() { _isActive = false; }

};

#endif
