#ifndef RANDOMCHATBOT_H
#define RANDOMCHATBOT_H

#include <tgbot/tgbot.h>
#include <tgbot/Bot.h>
#include <tgbot/types/Message.h>

#include <unordered_map>
#include <memory>

#include "chat/botuser.h"
#include "chat/messagemanager.h"
#include "helper/useractions.h"
#include "helper/localization.h"
#include "net/customizablepoll.h"

#include "chat/virtualroom.h"

#include "timer/timermanager.h"

#include "sql/sqlwrapper.h"

class RandomChatBot
{
private:
	std::shared_ptr<UsersCollection> users_;
	std::shared_ptr<Localization> localization_;
	std::shared_ptr<TgBot::Bot> bot_;
	std::shared_ptr<MessageManager> messageManager_;

	std::shared_ptr<UserActions> actions_;

	TimerManager timerManager_;

	SqlWrapper sqlWrapper_;

	CustomizablePoll poll_;
	std::shared_ptr<BotUser> &getUser(int64_t userId);

	void disconnectIdlersFromChats();

public:
	RandomChatBot();
	~RandomChatBot();
	void start();
};

#endif // RANDOMCHATBOT_H
