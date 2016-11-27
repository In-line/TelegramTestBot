#ifndef TIMER_H
#define TIMER_H

#include <ctime>
#include <functional>
#include <vector>
class Timer
{
private:
	std::vector<std::function<void(void)>> listeners_;
	std::time_t time_;
	int maxTime_;
	bool expired_;
public:
	Timer(const int duration);
	void tick();
	void onTimeout(const std::function<void(void)> &listener);

	bool expired() const;
	void setExpired(bool expired);
};

#endif // TIMER_H
