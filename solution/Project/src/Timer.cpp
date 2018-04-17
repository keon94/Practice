#include <math.h>
#include "Timer.h"
namespace PerfAssessment {
    Timer::Timer(const Units& units):
        _execTimes(1),
        _cachingEnabled(false),
        _verbose(false)
    {
        _timeStats.MinDuration = ULLONG_MAX;
        _timeStats.Variance = 0;
        _timeStats.MaxDuration = 0;
        _timeStats.AvgDuration = 0;
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

    const Timer& Timer::showStats(const char* functionName) const {
		streamStats(stdout, functionName);
		return *this;
    }

	const Timer& Timer::writeStats(const char* functionName, const char* fileName, bool overwrite) const {
		FILE* stream;
		fopen_s(&stream, "output.txt", overwrite ? "w" : "a");
		streamStats(stream, functionName);
		fclose(stream);
		return *this;
	}

	const Timer& Timer::writeStats(const char* fileName, bool overwrite) const {
		return writeStats(0, fileName, overwrite);
	}

	const Timer& Timer::streamStats(FILE* output, const char* functionName) const {
		if (functionName == 0)
			std::fprintf(output, "=========Timing results summary after %d runs:=========\n", _execTimes);
		else
			std::fprintf(output, "=========Timing results summary after %d runs of %s:=========\n", _execTimes, functionName);
		std::fprintf(output, "{Caching: %s}\n", _cachingEnabled ? "Enabled" : "Disabled");
		std::fprintf(output, "Minimum: %.*g %s\n", _unitAttributes.Sigfigs, static_cast<double>(_timeStats.MinDuration) / _unitAttributes.Divider, _unitAttributes.UnitName);
		std::fprintf(output, "Maximum: %.*g %s\n", _unitAttributes.Sigfigs, static_cast<double>(_timeStats.MaxDuration) / _unitAttributes.Divider, _unitAttributes.UnitName);
		std::fprintf(output, "Standard deviation: %.*g %s\n", _unitAttributes.Sigfigs, sqrt(_timeStats.Variance) / _unitAttributes.Divider, _unitAttributes.UnitName);
		std::fprintf(output, "Average: %.*g %s\n", _unitAttributes.Sigfigs, _timeStats.AvgDuration / _unitAttributes.Divider, _unitAttributes.UnitName);
		return *this;
	}

	Timer::TimeAttributes Timer::getStats() const {
		return _timeStats;
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
        _unitAttributes.Divider = 1;
        _unitAttributes.Sigfigs = 6;
        _unitAttributes.UnitName = "ns";
        switch (units) {
            case(SECONDS): {
                _unitAttributes.Divider = 1000000000;
                _unitAttributes.UnitName = "s";
                break;
            }
            case(MILISECONDS): {
                _unitAttributes.Divider = 1000000;
                _unitAttributes.UnitName = "ms";
                break;
            }
            case(MICROSECONDS): {
                _unitAttributes.Divider = 1000;
                _unitAttributes.UnitName = "us";
                break;
            }
        }
    }
}