#pragma once
#include <chrono>
#include <functional>
#include <iostream>
namespace Testing
{
	template<typename timeMeasure = typename std::chrono::microseconds>
	class Timer
	{
	private:
		std::chrono::time_point<std::chrono::steady_clock> mStartTime;
	public:
		std::vector<uint64_t> mSavedTimes;

		void startTimer() noexcept { mStartTime = std::chrono::steady_clock::now(); }

		//calculates average time. make sure you keep time measurement consistant 
		uint64_t averageTime() noexcept
		{

			uint64_t average = 0;
			const int size = mSavedTimes.size();
			if (size < 10000) //if this overflows you have the wrong time mesurement.
			{
				for (int i = 0; i < size; ++i)
				{
					average += (mSavedTimes[i]);
				}
				average /= size;
			}
			else
			{
				//more cautious approach to avoid int overflow. not likely with uint64t unluss
				for (int i = 0; i < size; ++i)
				{
					average += (mSavedTimes[i] / size);
				}
			}

			return average;
		}
		inline void saveTime() noexcept { mSavedTimes.push_back(timeElapsed()); }
		inline void clearSavedTimes() noexcept { mSavedTimes.clear(); }
		void printTime(const std::string& message)
		{
			const long long timeTaken = timeElapsed();
			std::cout << message.c_str() << timeTaken << " micro seconds";
		}
		//returns time since startTimer called in micro seconds.
		long long timeElapsed() noexcept
		{ return std::chrono::duration_cast<timeMeasure>(std::chrono::steady_clock::now() - mStartTime).count(); }
		
	};

	template<typename Return, typename... Ts>
	uint64_t timeFunction(const std::function<Return(Ts...)> & func, Ts... params)
	{
		Timer timer;
		timer.startTimer();
		func(params...);
		return timer.timeElapsed();
	};
	//should return time taken for function called functionCalls times
	template<typename Return, typename... Ts>
	uint64_t timeFunction(int functionCalls, const std::function<Return(Ts...)> & func, Ts &&... params )
	{
		Timer timer;
		timer.startTimer();
		
		for (int i = 0; i < functionCalls; i++)
		{
			func(std::forward<Ts>(params)...);
		}
		return timer.timeElapsed();

	}
}