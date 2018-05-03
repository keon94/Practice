#ifndef TIMER_H_
#define TIMER_H_

#include <stdio.h>

#define PORT_DLL __declspec(dllexport) 

#include "u_ptr.h"

namespace perf {
    namespace assess {

#define F_CALL(F) [this]() { return F; }
#define F_OVERWRITE 1

        class PORT_DLL Timer {

            using ullong = unsigned long long;
            using ulong = unsigned long;

        public:

            struct  TimeAttributes {
                ullong MaxDuration;
                ullong MinDuration;
                double AvgDuration;
                double Variance;
            };

            enum Units { SECONDS, MILISECONDS, MICROSECONDS, NANOSECONDS };

            Timer(const Units& units = Units::NANOSECONDS);

            ~Timer() = default;

            template <typename F>
            const Timer& execute(F&& f);

            Timer& times(unsigned ntimes);

            Timer& verbose();

            Timer& enableCaching();

            const Timer& showStats(const char* functionName = 0) const;

            const Timer& writeStats(const char* fileName, bool overwrite = true) const;

            const Timer& writeStats(const char* functionName, const char* fileName, bool overwrite = true) const;

            TimeAttributes getStats() const;

            template <typename R>
            R getResult() const;

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
            u_ptr<void> _result;
        };
    }
}
#endif

#include <chrono>

namespace perf {
    namespace assess {
        template <typename T, typename U> 
        struct Types { 
            static constexpr bool ARE_SAME = false; 
        };

        template <typename T> 
        struct Types<T, T> { 
            static constexpr bool ARE_SAME = true;
        };

        template<typename R>
        R Timer::getResult() const {
            if (!_result)
                throw std::exception("Error in Timer::getResult(): execute() has not been called.");
            return *static_cast<R*>(_result);
        }

        template <typename F>
        const Timer& Timer::execute(F&& f) {
            using namespace std::chrono;
            time_point<high_resolution_clock> start, end;
            unsigned timesExecuted = 0;
            while (timesExecuted < _execTimes) {
                if (!_cachingEnabled) {
                    flushCpuCache();
                }
                using R = decltype(f());
                if constexpr (Types<R, void>::ARE_SAME) {
                    start = high_resolution_clock::now();
                    f();
                    end = high_resolution_clock::now();
                }
                else {
                    start = high_resolution_clock::now();
                    R returnedResult = f();
                    end = high_resolution_clock::now();
                    _result = u_ptr<R>(returnedResult);
                }
                ullong duration = duration_cast<nanoseconds>(end - start).count();
                timesExecuted++;
                if(_verbose)
                    std::printf("Run %d: %.*g %s\n", timesExecuted, _unitAttributes.Sigfigs, static_cast<double>(duration)/_unitAttributes.Divider, _unitAttributes.UnitName);
                _timeStats.MaxDuration = duration > _timeStats.MaxDuration ? duration : _timeStats.MaxDuration;
                _timeStats.MinDuration = duration < _timeStats.MinDuration ? duration : _timeStats.MinDuration;
                _timeStats.AvgDuration = ((timesExecuted - 1)*_timeStats.AvgDuration + duration) / timesExecuted;
                auto Sq = [](auto x){ return x*x; };
                _timeStats.Variance = timesExecuted < 2 ? 0 : ((timesExecuted - 2)*_timeStats.Variance + Sq(duration - _timeStats.AvgDuration)) / timesExecuted;
            }
            return *this;
        }
    }
}