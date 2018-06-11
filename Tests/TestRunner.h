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

template <typename SubTest>
class TestRunner : public Runnable, public SubTest {

public:
    struct ResultTest {
        using Ptr = std::unique_ptr<ResultTest>;
        bool finished = false;
        int64_t microsec;
    };
    virtual ~TestRunner() = default;

public:
    static int64_t getItervalBetweenCalculate(const std::function<void ()> &func);

private:
    template <typename ...Args>
    TestRunner(condition_variable &conditionFinishTest,
         ResultTest &resultTest,
         Args &&...args) :
        Runnable(),
        SubTest(std::forward<Args>(args)...),
        _condFinishTest(conditionFinishTest),
        _result(resultTest)
    {}

    virtual void run() override;
    inline const ResultTest &getResult() const;

public:
    template <typename ...Args>
    static typename ResultTest::Ptr startInNewThread(condition_variable &conditionFinishTest, Args ...args) {

        typename ResultTest::Ptr resultObj(new ResultTest);
        Runnable::Ptr p(new TestRunner<SubTest>(conditionFinishTest,
                                          *resultObj,
                                          std::forward<Args>(args)...));

        ThreadPool::defaultPool()->start(std::move(p));

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

private:
    condition_variable &_condFinishTest;
    ResultTest &_result;
};

// IMPLEMENTATION TEMPLATE 'TEST' CLASS


template<typename SubTest>
int64_t TestRunner<SubTest>::getItervalBetweenCalculate(const std::function<void ()> &func)
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

    _result.microsec =
    getItervalBetweenCalculate([&](){
        SubTest::startTest();
    });

    SubTest::preparationAfterTest();

    _result.finished = true;
    _condFinishTest.notify_all();
    std::this_thread::yield();
}

template<typename SubTest>
const typename TestRunner<SubTest>::ResultTest &TestRunner<SubTest>::getResult() const
{ return _result; }

#endif // TEST_H
