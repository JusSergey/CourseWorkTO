#ifndef RUNNABLE_H
#define RUNNABLE_H
#include <memory>

class Runnable {
public:
    using Ptr = std::unique_ptr<Runnable>;
    Runnable() = default;
    ~Runnable() = default;
    virtual void run() {}
};

#endif // RUNNABLE_H
