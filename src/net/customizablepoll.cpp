#include "customizablepoll.h"

int32_t CustomizablePoll::timeout() const
{
	return timeout_;
}

void CustomizablePoll::setTimeout(const int32_t &timeout)
{
	timeout_ = timeout;
}

CustomizablePoll::CustomizablePoll(const std::shared_ptr<TgBot::Bot> &bot, int32_t timeout) :
	api_(&bot->getApi()),
	eventHandler_(&bot->getEventHandler()),
	lastUpdateId_(0),
	timeout_(timeout)
{
#ifdef DEBUG
	printf("%s\n", __FUNCTION__);
#endif
}

CustomizablePoll::CustomizablePoll(const CustomizablePoll &poll) :
	api_(poll.api_),
	eventHandler_(poll.eventHandler_),
	lastUpdateId_(0),
	timeout_(poll.timeout())
{
}

CustomizablePoll& CustomizablePoll::operator=(const CustomizablePoll &poll)
{
	if(poll == *this)
		return *this;

	api_ = poll.api_;
	eventHandler_ = poll.eventHandler_;
	lastUpdateId_  = 0;
	timeout_ = poll.timeout_;

	return *this;
}

bool CustomizablePoll::operator==(const CustomizablePoll &poll) const
{
	return
			api_ == poll.api_ &&
			eventHandler_ == poll.eventHandler_ &&
			timeout_ == poll.timeout_;
}

CustomizablePoll::~CustomizablePoll()
{
#ifdef DEBUG
	printf("~%s\n", __FUNCTION__);
#endif
}

void CustomizablePoll::start()
{
	std::vector<TgBot::Update::Ptr> updates = api_->getUpdates(lastUpdateId_, 100, timeout_);
	for(TgBot::Update::Ptr &item : updates)
	{
		if(item->updateId >= lastUpdateId_ && item->updateId < INT_MAX)
		{
			lastUpdateId_ = item->updateId + 1;
		}
		eventHandler_->handleUpdate(item);
	}
}
