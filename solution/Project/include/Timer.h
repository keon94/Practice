#ifndef TIMER_H_
#define TIMER_H_

#include <stdio.h>
#include <memory>

#define PORT_DLL __declspec(dllexport) 

namespace PerfAssessment {

    #define F_CALL(F) [this]() { return F; }
    #define F_OVERWRITE 1
 
    class Timer {

        using ullong = unsigned long long;
        using ulong = unsigned long;
        
        public:

			struct PORT_DLL TimeAttributes {
				ullong MaxDuration;
				ullong MinDuration;
				double AvgDuration;
				double Variance;
			};

            enum PORT_DLL Units { SECONDS, MILISECONDS, MICROSECONDS, NANOSECONDS };

            PORT_DLL Timer(const Units& units = Units::NANOSECONDS);

            PORT_DLL ~Timer() = default;

            template <typename F>
            PORT_DLL const Timer& execute(F&& f);
            
            PORT_DLL Timer& times(unsigned ntimes);

            PORT_DLL Timer& verbose();
            
            PORT_DLL Timer& enableCaching();
            
            PORT_DLL const Timer& showStats(const char* functionName = 0) const;

            PORT_DLL const Timer& writeStats(const char* fileName, bool overwrite = true) const;

            PORT_DLL const Timer& writeStats(const char* functionName, const char* fileName, bool overwrite = true) const;

            PORT_DLL TimeAttributes getStats() const;

            template <typename R>
            PORT_DLL R getResult() const;

        private:

            struct UnitAttributes {
                ulong Divider;
                unsigned Sigfigs;
                const char* UnitName;
            } _unitAttributes;

            template<typename R>
            struct Result {
                Result(const R& value) : value(value) {}
                const R value;
            };

			TimeAttributes _timeStats;
        
        private:
            
            PORT_DLL void flushCpuCache() const;

            void setUnitAttributes(const Units& units);

			const Timer& streamStats(FILE* out, const char* functionName = 0) const;
        
        private:
            unsigned _execTimes;
            bool _cachingEnabled;
            bool _verbose;
            std::shared_ptr<void> _result;
    };
}
#endif

#include <chrono>

namespace PerfAssessment {

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
        return static_cast<Result<R>*>(_result.get())->value;
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
                _result = std::make_shared<Result<R>>(returnedResult);
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