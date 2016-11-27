#ifndef USERACTIONS_H
#define USERACTIONS_H
#include "../chat/messagemanager.h"
#include "../helper/localization.h"
#include "../chat/botuser.h"

#include <memory>
#include <unordered_map>

#include "typedefs.h"

class UserActions
{
private:
	std::shared_ptr<MessageManager> messageManager_;
	std::shared_ptr<Localization> localization_;
	std::shared_ptr<UsersCollection> users_;

	bool searchPartner(const std::shared_ptr<BotUser> &user);
public:
	UserActions(const std::shared_ptr<MessageManager> &messageManager,
							const std::shared_ptr<Localization> &localization,
							const std::shared_ptr<UsersCollection> &users);
	~UserActions();
	void changeNickName(const std::shared_ptr<BotUser> &user);
	void handleNickNameChange(const std::shared_ptr<BotUser> &user, const std::string &message);

	void changeLanguage(const std::shared_ptr<BotUser> &user);
	void handleLanguageChange(const std::shared_ptr<BotUser> &user, const std::string &message);

	void startPartnerSearching(const std::shared_ptr<BotUser> &user);
	void handlePartnerChat(const std::shared_ptr<BotUser> &user, TgBot::Message::Ptr message);

	void endRoomChatting(const std::shared_ptr<BotUser> &user);

	const std::shared_ptr<Localization> &localization() const;
	void setLocalization(const std::shared_ptr<Localization> &localization);

	std::shared_ptr<MessageManager> messageManager() const;
	void setMessageManager(const std::shared_ptr<MessageManager> &messageManager);
};

#endif // USERACTIONS_H
