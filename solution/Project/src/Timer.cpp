#include <math.h>
#include <limits.h>
#include "Timer.h"
namespace PerfAssessment {
    Timer::Timer(const Units& units):
        _execTimes(1),
        _cachingEnabled(false),
        _verbose(false)
    {
        _timeStats._minDuration = ULLONG_MAX;
        _timeStats._variance = 0;
        _timeStats._maxDuration = 0;
        _timeStats._avgDuration = 0;
        setUnitAttributes(units);
    }

        Timer& Timer::times(unsigned ntimes) {
        _execTimes = ntimes;
        return *this;
    }

    Timer& Timer::enableCaching() {
        _cachingEnabled = true;
        return *this;
    }

    Timer& Timer::verbose() {
        _verbose = true;
        return *this;
    }

    void Timer::showStats(const char* functionName) const {
        if(functionName == 0)
            std::printf("=========Timing results summary after running %d times:=========\n", _execTimes);
        else 
            std::printf("=========Timing results summary of running %s %d times:=========\n", functionName, _execTimes);
        std::printf("Maximum: %.*f %s\n", _unitAttributes._sigfigs, static_cast<double>(_timeStats._maxDuration) / _unitAttributes._divider, _unitAttributes._unitName);
        std::printf("Minimum: %.*f %s\n", _unitAttributes._sigfigs, static_cast<double>(_timeStats._minDuration) / _unitAttributes._divider, _unitAttributes._unitName);
        std::printf("Average: %.*f %s\n", _unitAttributes._sigfigs, _timeStats._avgDuration / _unitAttributes._divider, _unitAttributes._unitName);
        std::printf("Standard deviation: %.*f %s\n", _unitAttributes._sigfigs, sqrt(_timeStats._variance) / _unitAttributes._divider, _unitAttributes._unitName);
    }

    void Timer::flushCpuCache() const {
        const size_t dataSize = 100000000; //100 MB 
        void* tempData1 = std::calloc(dataSize, 1);
        void* tempData2 = std::calloc(dataSize, 1);
        memcpy(tempData2, tempData1, dataSize);
        std::free(tempData1);
        std::free(tempData2);
    }

    void Timer::setUnitAttributes(const Units& units) {
        _unitAttributes._divider = 1;
        _unitAttributes._sigfigs = 0;
        _unitAttributes._unitName = "ns";
        switch (units) {
            case(SECONDS): {
                _unitAttributes._divider = 1000000000;
                _unitAttributes._sigfigs = 9;
                _unitAttributes._unitName = "s";
                break;
            }
            case(MILISECONDS): {
                _unitAttributes._divider = 1000000;
                _unitAttributes._sigfigs = 6;
                _unitAttributes._unitName = "ms";
                break;
            }
            case(MICROSECONDS): {
                _unitAttributes._divider = 1000;
                _unitAttributes._sigfigs = 3;
                _unitAttributes._unitName = "us";
                break;
            }
        }
    }
}