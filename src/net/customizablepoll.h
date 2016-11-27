#ifndef CUSTOMIZABLEPOLL_H
#define CUSTOMIZABLEPOLL_H

#include <tgbot/Bot.h>
#include <tgbot/Api.h>

#include <memory>

class CustomizablePoll
{
private:
	const TgBot::Api* api_;
	const TgBot::EventHandler* eventHandler_;
	int32_t lastUpdateId_;
	int32_t timeout_;
public:
	CustomizablePoll(const std::shared_ptr<TgBot::Bot> &bot, int32_t timeout = 0);
	CustomizablePoll(const CustomizablePoll &poll);
	CustomizablePoll& operator=(const CustomizablePoll &poll);

	bool operator==(const CustomizablePoll &poll) const;

	~CustomizablePoll();
	void start();

	int32_t timeout() const;
	void setTimeout(const int32_t &timeout);
};
#endif
