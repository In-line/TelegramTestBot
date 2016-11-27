#ifndef TIMERMANAGER_H
#define TIMERMANAGER_H

#include <vector>
#include <memory>
#include <unordered_map>

#include "../chat/botuser.h"

class TimerManager
{
private:
	std::shared_ptr<std::unordered_map<int64_t, std::shared_ptr<BotUser>>> users_;
public:
	TimerManager(std::shared_ptr<std::unordered_map<int64_t, std::shared_ptr<BotUser>>> &users);

	void tick();
};

#endif // TIMERMANAGER_H
