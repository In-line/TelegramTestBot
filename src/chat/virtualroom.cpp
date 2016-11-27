#include "virtualroom.h"

#include "../chat/botuser.h"
#include "../helper/stocks.h"
int64_t VirtualRoom::sqlId() const
{
	return sqlId_;
}

void VirtualRoom::setSqlId(const int64_t &sqlId)
{
	sqlId_ = sqlId;
}

VirtualRoom::VirtualRoom()
{
#ifdef DEBUG
	printf("%s\n", __FUNCTION__);
#endif
}

VirtualRoom::~VirtualRoom()
{
#ifdef DEBUG
	printf("~%s\n", __FUNCTION__);
#endif
}

void VirtualRoom::broadcastMessage(const std::shared_ptr<MessageManager> &messageManager,const std::shared_ptr<BotUser> &user,const std::string &message)
{
	for(auto it = this->begin();it!=this->end();++it)
	{
		auto selectedUser = *it;
		if(selectedUser->id() != user->id())
		{
			messageManager->sendMessage(selectedUser->id(),
																	("["+std::to_string(user->virtualId())+"] " +
																	 (!is_string_empty(user->nickname()) ? (user->nickname() + " : ") : " : " )) + message);
		}

	}
}

