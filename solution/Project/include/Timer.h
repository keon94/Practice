#ifndef TIMER_H_
#define TIMER_H_
//#include <fstream>

namespace PerfAssessment {
 
    class __declspec(dllexport) Timer {

        using ullong = unsigned long long;
        using ulong = unsigned long;

        public:

            enum Units { SECONDS, MILISECONDS, MICROSECONDS, NANOSECONDS };

            Timer(const Units& units = Units::NANOSECONDS);

           ~Timer() = default;

            template <typename F, typename... Args>
            const Timer& execute(F&& f, Args&&... args);
            
            Timer& times(unsigned ntimes);

            Timer& verbose();
            
            Timer& enableCaching();
            
            void showStats(const char* functionName = 0) const;

        private:

            struct UnitAttributes {
                ulong _divider;
                unsigned _sigfigs;
                const char* _unitName;
            } _unitAttributes;

            struct TimeAttributes {
                ullong _maxDuration;
                ullong _minDuration;
                double _avgDuration;
                double _variance;
            } _timeStats;
        
        private:
            
            void flushCpuCache() const;

            void setUnitAttributes(const Units& units);
        
        private:
            unsigned _execTimes;
            bool _cachingEnabled;
            bool _verbose;
    };
}
#endif

#include <chrono>

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
                printf("Run %d: %.*f %s\n", timesExecuted, _unitAttributes._sigfigs, static_cast<double>(duration)/_unitAttributes._divider, _unitAttributes._unitName);
            _timeStats._maxDuration = duration > _timeStats._maxDuration ? duration : _timeStats._maxDuration;
            _timeStats._minDuration = duration < _timeStats._minDuration ? duration : _timeStats._minDuration;
            _timeStats._avgDuration = ((timesExecuted - 1)*_timeStats._avgDuration + duration) / timesExecuted;
            auto Sq = [](auto x){ return x*x; };
            _timeStats._variance = timesExecuted < 2 ? 0 : ((timesExecuted - 2)*_timeStats._variance + Sq(duration - _timeStats._avgDuration)) / timesExecuted;
        }
        return *this;
    }
}