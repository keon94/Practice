#include "Timer.h"
#include <thread>
#include <gtest/gtest.h>

class TimerTestsFixture : public testing::Test {
protected:
    class ClassUnderTest {
    public:
        int testFunction1(int x, int y) {
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(1ms);
            return x + y;
        }
    };
    using ClassUnderTest = TimerTestsFixture::ClassUnderTest;
    ClassUnderTest _UUT;
};

using perf::assess::Timer;

TEST_F(TimerTestsFixture, testWithNoCacheOptimization) {
    auto f = [this]() {
        _UUT.testFunction1(3, 5);
    };
    Timer(Timer::Units::MILISECONDS)
		.verbose()
		.times(10)
        .execute(f)
		.showStats("testFunction1");
}

TEST_F(TimerTestsFixture, testWithCacheOptimization) {
    auto result = Timer(Timer::Units::MILISECONDS)
        .verbose()
        .times(10)
        .enableCaching()
        .execute(F_CALL(_UUT.testFunction1(3, 5)))
        .showStats()
        .getResult<int>();
    ASSERT_TRUE(result == 8);
}

TEST_F(TimerTestsFixture, writeOutputToFile) {
	auto stats = Timer(Timer::Units::MILISECONDS)
		.verbose()
		.times(10)
		.enableCaching()
        .execute(F_CALL(_UUT.testFunction1(3, 5)))
		.showStats("testFunction1")
		.getStats();
}

TEST_F(TimerTestsFixture, writeOutputToFileAndStdOut) {
    auto& stats = Timer(Timer::Units::NANOSECONDS)
        .verbose()
        .times(10)
        .enableCaching()
        .execute(F_CALL(_UUT.testFunction1(3, 5)))
		.showStats("testFunction1")
		.writeStats("testFunction1", "output.txt", F_OVERWRITE);
}
