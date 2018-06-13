#ifndef TEST_H
#define TEST_H

#include "DepsHeaders.h"
#include <utility>
#include <functional>
#include <condition_variable>

using std::chrono::steady_clock;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::microseconds;
using condition_variable = std::condition_variable;
using TimeType = int64_t;

struct ResultTest {
    using Ptr = std::unique_ptr<ResultTest>;
    bool finished = false;
    void setInMicrosec(TimeType _mcsec) {
        microsec = _mcsec;
        sec = ((double)(_mcsec / 1000)) / 1000.f;
    }
    TimeType microsec = 0;
    float sec = 0;
};

template <typename SubTest>
class TestRunner : public Runnable, public SubTest {

public:

    virtual ~TestRunner() = default;

public:
    static TimeType getItervalBetweenCalculate(const std::function<void ()> &func);

private:
    template <typename ...Args>
    TestRunner(condition_variable &conditionFinishTest,
         ResultTest &result,
         Args &&...args) :
        Runnable(),
        SubTest(std::forward<Args>(args)...),
        _result(result),
        _condFinishTest(conditionFinishTest)
    {}

    virtual void run() override;
    inline const ResultTest &getResult() const;

public:
    template <typename ...Args>
    static typename ResultTest::Ptr startInNewThread(condition_variable &conditionFinishTest, ThreadPool &pool, Args &&...args) {

        typename ResultTest::Ptr resultObj(new ResultTest);
        Runnable::Ptr p(new TestRunner<SubTest>(conditionFinishTest,
                                                *resultObj,
                                                std::forward<Args>(args)...));

        pool.start(std::move(p), true);

        return std::move(resultObj);
    }

    template <typename ...Args>
    static typename ResultTest::Ptr startInCurrentThread(condition_variable &conditionFinishTest, Args ...args) {

        typename ResultTest::Ptr resultObj(new ResultTest);
        Runnable::Ptr p(new TestRunner<SubTest>(conditionFinishTest,
                                                *resultObj,
                                                std::forward<Args>(args)...));

        p->run();

        return std::move(resultObj);
    }

    void joinTest() const {
        while (!_result.finished)
            Thread::sleep(2);
    }

private:
    condition_variable &_condFinishTest;
    ResultTest &_result;
};

// IMPLEMENTATION TEMPLATE 'TEST' CLASS


template<typename SubTest>
TimeType TestRunner<SubTest>::getItervalBetweenCalculate(const std::function<void ()> &func)
{
    auto start = steady_clock::now();
    // calculating...
    func();
    // end...
    auto end = steady_clock::now();
    return duration_cast<microseconds>(end - start).count();
}

template<typename SubTest>
void TestRunner<SubTest>::run() {

    SubTest::preparationBeforeTest();

    _result.setInMicrosec(
    getItervalBetweenCalculate([&](){
        SubTest::startTest();
    }) );

    SubTest::preparationAfterTest();

    _result.finished = true;
    _condFinishTest.notify_all();
    std::this_thread::yield();
}

template<typename SubTest>
const ResultTest &TestRunner<SubTest>::getResult() const
{ return _result; }

#endif // TEST_H
