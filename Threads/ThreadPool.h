#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <cstddef>
#include "Thread.h"
#include "Runnable.h"

class ThreadPool {
    std::vector<Thread::Ptr> _threads;
    Thread::Ptr &getFreeThread();
    std::atomic_bool _stopping;

public:
    class ExhausedThreads : public std::exception {
        std::string _message;
    public:
        ExhausedThreads(const char *msg = "ho have free threads") : _message(msg) {}
        const char *what() const throw() override { return _message.c_str(); }
    };
    ThreadPool(size_t capacity = 4);
    ~ThreadPool();
    bool start(Runnable::Ptr &&runObj);
    bool isStopping() const { return _stopping; }
    void stopAllThreads();
    void waitAll();
};

#endif
