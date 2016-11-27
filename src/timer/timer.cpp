#include "timer.h"

bool Timer::expired() const
{
	return expired_;
}

void Timer::setExpired(bool expired)
{
	expired_ = expired;
}

Timer::Timer(const int duration) : listeners_(), time_(std::time(nullptr)), maxTime_(duration), expired_(false)
{
}

void Timer::tick()
{
	if(expired_)
		return;
	if(maxTime_ > 0)
	{
		auto duration = std::difftime(std::time(nullptr), time_);
		if(duration >= double(maxTime_) )
		{
			for(auto function : listeners_)
			{
				function();
			}
			expired_ = true;
		}
	}

	}

void Timer::onTimeout(const std::function<void ()> &listener)
{
	listeners_.push_back(listener);
}

