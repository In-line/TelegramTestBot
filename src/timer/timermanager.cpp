#include "timermanager.h"

TimerManager::TimerManager(std::shared_ptr<std::unordered_map<int64_t, std::shared_ptr<BotUser>>> &users) :
	users_(users)
{

}

void TimerManager::tick()
{
	for(auto it = users_->begin(); it!=users_->end();++it)
	{
		auto selectedUser = it->second;
		auto timersVector(selectedUser->timers());

		for(auto &timer : timersVector)
		{
			if(!timer->expired())
				timer->tick();
		}

		auto timerIterator = selectedUser->timers().begin();
		while(timerIterator != selectedUser->timers().end())
		{
			if( (*timerIterator)->expired() )
			{
				timerIterator = selectedUser->eraseTimer(timerIterator);
			}
			else
				++timerIterator;
		}
	}
}
