#include "Timer.h"
#include <thread>
#include <gtest/gtest.h>

class TimerTestsFixture : public testing::Test {
protected:
    class ClassUnderTest {
    public:
        int testFunction1(int x, int y) {
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(1us);
            return x + y;
        }
    };
    using ClassUnderTest = TimerTestsFixture::ClassUnderTest;
    ClassUnderTest _cut;
};

using PerfAssessment::Timer;

TEST_F(TimerTestsFixture, testWithNoCacheOptimization) {
    auto f = [this](int x, int y)
    {
        _cut.testFunction1(x, y);
    };
    Timer(Timer::Units::MICROSECONDS).times(10).execute(f, 3, 5).showStats("testFunction1");
}

TEST_F(TimerTestsFixture, testWithCacheOptimization) {
    auto f = [this](int x, int y)
    {
        _cut.testFunction1(x, y);
    };
    Timer(Timer::Units::MICROSECONDS).times(10).enableCaching().execute(f, 3, 5).showStats();
}
