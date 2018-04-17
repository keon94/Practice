#ifndef TIMER_H_
#define TIMER_H_
#include <stdio.h>

namespace PerfAssessment {
 
    class __declspec(dllexport) Timer {

        using ullong = unsigned long long;
        using ulong = unsigned long;

        public:

			struct TimeAttributes {
				ullong MaxDuration;
				ullong MinDuration;
				double AvgDuration;
				double Variance;
			};

            enum Units { SECONDS, MILISECONDS, MICROSECONDS, NANOSECONDS };

            Timer(const Units& units = Units::NANOSECONDS);

           ~Timer() = default;

            template <typename F, typename... Args>
            const Timer& execute(F&& f, Args&&... args);
            
            Timer& times(unsigned ntimes);

            Timer& verbose();
            
            Timer& enableCaching();
            
			const Timer& showStats(const char* functionName = 0) const;

			const Timer& writeStats(const char* fileName, bool overwrite = true) const;

			const Timer& writeStats(const char* functionName, const char* fileName, bool overwrite = true) const;

			TimeAttributes getStats() const;

        private:

            struct UnitAttributes {
                ulong Divider;
                unsigned Sigfigs;
                const char* UnitName;
            } _unitAttributes;

			TimeAttributes _timeStats;
        
        private:
            
            void flushCpuCache() const;

            void setUnitAttributes(const Units& units);

			const Timer& streamStats(FILE* out, const char* functionName = 0) const;
        
        private:
            unsigned _execTimes;
            bool _cachingEnabled;
            bool _verbose;
    };
}
#endif

#include <chrono>

#define F_Call(F) [this]() { F; }

namespace PerfAssessment {
    template <typename F, typename... Args>
    const Timer& Timer::execute(F&& f, Args&&... args) {
        using namespace std::chrono;
        time_point<high_resolution_clock> start, end;
        unsigned timesExecuted = 0;
        while (timesExecuted < _execTimes) {
            if (!_cachingEnabled) {
                flushCpuCache();
            }
            start = high_resolution_clock::now();
            f(args...);
            end = high_resolution_clock::now();
            ullong duration = duration_cast<nanoseconds>(end - start).count();
            timesExecuted++;
            if(_verbose)
                printf("Run %d: %.*g %s\n", timesExecuted, _unitAttributes.Sigfigs, static_cast<double>(duration)/_unitAttributes.Divider, _unitAttributes.UnitName);
            _timeStats.MaxDuration = duration > _timeStats.MaxDuration ? duration : _timeStats.MaxDuration;
            _timeStats.MinDuration = duration < _timeStats.MinDuration ? duration : _timeStats.MinDuration;
            _timeStats.AvgDuration = ((timesExecuted - 1)*_timeStats.AvgDuration + duration) / timesExecuted;
            auto Sq = [](auto x){ return x*x; };
            _timeStats.Variance = timesExecuted < 2 ? 0 : ((timesExecuted - 2)*_timeStats.Variance + Sq(duration - _timeStats.AvgDuration)) / timesExecuted;
        }
        return *this;
    }
}