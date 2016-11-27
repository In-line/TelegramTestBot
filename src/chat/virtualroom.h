#ifndef VIRTUALROOM_H
#define VIRTUALROOM_H

#include <vector>
#include <memory>
#include <stdint.h>
#include <tgbot/Bot.h>
#include <tgbot/Api.h>

#include <string>

#include "../chat/messagemanager.h"
class BotUser;

class VirtualRoom : public std::vector<std::shared_ptr<BotUser>>
{
	int64_t sqlId_ = 0;
public:
	VirtualRoom();
	~VirtualRoom();
	void broadcastMessage(const std::shared_ptr<MessageManager> &messageManager, const std::shared_ptr<BotUser> &user, const std::__cxx11::string &message);

	int64_t sqlId() const;
	void setSqlId(const int64_t &sqlId);
};

#endif // VIRTUALROOM_H
