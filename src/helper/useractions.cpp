#include "../helper/useractions.h"
#include "../helper/stocks.h"
#include <tgbot/types/ReplyKeyboardMarkup.h>
#include <tgbot/types/ReplyKeyboardHide.h>

#include "../chat/virtualroom.h"

const std::shared_ptr<Localization> &UserActions::localization() const
{
	return localization_;
}

void UserActions::setLocalization(const std::shared_ptr<Localization> &localization)
{
	localization_ = localization;
}

std::shared_ptr<MessageManager> UserActions::messageManager() const
{
	return messageManager_;
}

void UserActions::setMessageManager(const std::shared_ptr<MessageManager> &messageManager)
{
	messageManager_ = messageManager;
}

UserActions::UserActions(const std::shared_ptr<MessageManager> &messageManager,
												 const std::shared_ptr<Localization> &localization,
												 const std::shared_ptr<UsersCollection> &users) :
	messageManager_(messageManager),
	localization_(localization),
	users_(users)
{
#ifdef DEBUG
	printf("%s\n", __FUNCTION__);
#endif
}

UserActions::~UserActions()
{
#ifdef DEBUG
	printf("~%s\n", __FUNCTION__);
#endif
}

void UserActions::changeNickName(const std::shared_ptr<BotUser> &user)
{
	messageManager_->sendMessage(user->id(), localization()->tr(user->language(), "USER_CHANGE_NAME"));
	user->setWaitingStatus(INPUT_WAITING_NICKNAME);
}

void UserActions::handleNickNameChange(const std::shared_ptr<BotUser> &user, const std::__cxx11::string &message)
{
	if(is_string_empty(message))
	{
		changeNickName(user);
		return;
	}
	std::string trimmedMessage = trim(message);
	if(trimmedMessage.size() > 32) trimmedMessage.erase(32);
	messageManager_->sendMessage(user->id(),
															 (localization()->tr(user->language(), "USER_CHANGED_NAME") + " " + trimmedMessage));
	user->setNickname(trimmedMessage);
	user->setWaitingStatus(INPUT_WAITING_NONE);
}
void UserActions::changeLanguage(const std::shared_ptr<BotUser> &user)
{
	auto l = localization()->availableLanguages();

	TgBot::ReplyKeyboardMarkup::Ptr markup(new TgBot::ReplyKeyboardMarkup);
	markup->oneTimeKeyboard = true;
	int i = 0 ;
	for(auto it = l.begin();it!=l.end();++it,++i)
	{
		if(i%3 == 0)
		{
			markup->keyboard.push_back(std::vector<std::string>());
		}
		markup->keyboard.back().push_back(localization()->tr(*it, "LANGUAGE_NAME"));
	}
	messageManager_->sendMessage(user->id(), "[RandomChat] Select language!", markup);
	user->setWaitingStatus(INPUT_WAITING_LANGUAGE);
}

void UserActions::handleLanguageChange(const std::shared_ptr<BotUser> &user, const std::__cxx11::string &message)
{
	bool found = false;
	auto l = localization()->availableLanguages();
	auto it = l.begin();
	for(;it!=l.end();++it)
	{
		if(localization()->tr(*it, "LANGUAGE_NAME") == message)
		{
			found = true;
			break;
		}
	}
	if(!found)
	{
		changeLanguage(user);
		return;
	}

	TgBot::ReplyKeyboardHide::Ptr hide(new TgBot::ReplyKeyboardHide);

	messageManager_->sendMessage(user->id(), localization()->tr(*it, "USER_WELCOME_MSG"), hide);
	user->setLanguage(*it);
	user->setWaitingStatus(INPUT_WAITING_NONE);
}

bool UserActions::searchPartner(const std::shared_ptr<BotUser> &user)
{
	std::vector<std::shared_ptr<BotUser>> freeUsers;
	for(auto it = users_->begin(); it!=users_->end(); ++it)
	{
		std::shared_ptr<BotUser> selectedUser = it->second;
		if(selectedUser->id() == user->id())
			continue;
		if(!selectedUser->room() && selectedUser->searchingStatus() == PARTNER_SEARCHING_STARTED)
			freeUsers.push_back(selectedUser);
	}

	if(freeUsers.empty())
		return false;

	for(short i = 0;i<3;++i)
	{
		int nSize = (int)freeUsers.size();
		for(int i = 0;i<nSize;++i)
		{
			if(random_int(0, nSize - 1) == i)
			{
				auto selectedUser = freeUsers[i];
				auto room = std::make_shared<VirtualRoom>();
				room->push_back(selectedUser);
				room->push_back(user);
				selectedUser->setRoom(room);
				user->setRoom(room);
				return true;
			}
		}
	}
	return false;
}

void UserActions::startPartnerSearching(const std::shared_ptr<BotUser> &user)
{
	messageManager_->sendMessage(user->id(), localization()->tr(user->language(), "PARTNER_SEARCHING_STARTED"));
	if(!searchPartner(user))
	{
		auto timer = std::make_shared<Timer>(5);
		user->pushTimer(timer);
		user->setSearchingStatus(PARTNER_SEARCHING_STARTED);
		timer->onTimeout([this, user](){
			if(user->searchingStatus() == PARTNER_SEARCHING_STARTED)
			{
				startPartnerSearching(user);
			}
		});
	}
	else
	{
		for (auto &selectedUser : *user->room())
		{
			if(selectedUser->searchingStatus() != PARTNER_SEARCHING_FOUND)
				selectedUser->setSearchingStatus(PARTNER_SEARCHING_FOUND);
			messageManager_->sendMessage(selectedUser->id(),
																	 localization()->tr(selectedUser->language(), "PARTNER_FOUND"));
		}
	}
}

void UserActions::handlePartnerChat(const std::shared_ptr<BotUser> &user, TgBot::Message::Ptr message)
{
	if(user->room())
	{
		if(user->room()->size() < 2)
		{
			endRoomChatting(user);
			return;
		}
		user->room()->broadcastMessage(messageManager_, user, message->text);
	}
}

void UserActions::endRoomChatting(const std::shared_ptr<BotUser> &user)
{
	if(user->searchingStatus() == PARTNER_SEARCHING_FOUND)
	{
		if(user->room())
		{
			auto it = std::find(user->room()->begin(), user->room()->end(), user);
			if(it != user->room()->end())
			{
				for(std::shared_ptr<BotUser> &selectedUser : *user->room())
				{
					if(selectedUser->id() != user->id())
					{
						messageManager_->sendMessage(selectedUser->id(),"[RandomChat] ["+std::to_string(selectedUser->virtualId())+"] " + selectedUser->nickname() +
																				 localization()->tr(selectedUser->language(), "HUMAN_DISCONNECTED_FROM_CHAT"));
					}
				}
				messageManager_->sendMessage(user->id(), localization()->tr(user->language(), "YOU_WAS_DISCONNECTED"));
				user->room()->erase(it);
				{
					std::shared_ptr<VirtualRoom> nullRoom = std::make_shared<VirtualRoom>();
					nullRoom.reset();
					if(user->room()->size() < 2)
					{
						for(auto &selectedUser : *user->room())
						{
							user->setSearchingStatus(PARTNER_SEARCHING_NONE);
							selectedUser->setRoom(nullRoom);
							this->startPartnerSearching(selectedUser);
						}
					}
					user->setRoom(nullRoom);
				}
			}
			user->setSearchingStatus(PARTNER_SEARCHING_NONE);
		}
	}
}
