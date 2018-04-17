#include "Timer.h"
#include <thread>
#include <gtest/gtest.h>

class TimerTestsFixture : public testing::Test {
protected:
    class ClassUnderTest {
    public:
        int testFunction1(int x, int y) {
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(5s);
            return x + y;
        }
    };
    using ClassUnderTest = TimerTestsFixture::ClassUnderTest;
    ClassUnderTest _UUT;
};

using PerfAssessment::Timer;

TEST_F(TimerTestsFixture, testWithNoCacheOptimization) {
    auto f = [this](int x, int y)
    {
		_UUT.testFunction1(x, y);
    };
    Timer(Timer::Units::MILISECONDS)
		.verbose()
		.times(2)
		.execute(f, 3, 5)
		.showStats("testFunction1");
}

TEST_F(TimerTestsFixture, testWithCacheOptimization) {
    auto f = [this]()
    {
		_UUT.testFunction1(3, 5);
    };
	auto stats = Timer(Timer::Units::MILISECONDS)
					.verbose()
					.times(2)
					.enableCaching()
					.execute(f)
					.showStats()
					.getStats();
}

TEST_F(TimerTestsFixture, writeOutputToFile) {
	auto f = [this]()
	{
		_UUT.testFunction1(3, 5);
	};
	auto stats = Timer(Timer::Units::MILISECONDS)
		.verbose()
		.times(2)
		.enableCaching()
		.execute(F_Call(_UUT.testFunction1(3, 5)))
		.showStats("testFunction1")
		.getStats();
}

TEST_F(TimerTestsFixture, writeOutputToFileAndStdOut) {
	auto f = [this]()
	{
		_UUT.testFunction1(3, 5);
	};
	auto stats = Timer(Timer::Units::NANOSECONDS)
		.verbose()
		.times(2)
		.enableCaching()
		.execute(f)
		.showStats("testFunction1")
		.writeStats("testFunction1", "output.txt", true);
}
