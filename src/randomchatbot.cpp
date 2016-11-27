#include "randomchatbot.h"
#include "helper/stocks.h"

#include <iostream>
RandomChatBot::RandomChatBot() :
	users_(std::make_shared<UsersCollection>()),
	localization_(std::make_shared<Localization>("dictionary.txt")),
	bot_(std::make_shared<TgBot::Bot>("210592923:AAEVTilxxmpXv3yaJiXhRvPu5sCQV2L2y0s")),
	messageManager_(std::make_shared<MessageManager>(bot_)),
	actions_(std::make_shared<UserActions>(messageManager_, localization_, users_)),
	timerManager_(users_),
	sqlWrapper_(users_, actions_),
	poll_(bot_)
{
#ifdef DEBUG
	printf("%s\n", __FUNCTION__);
#endif
	std::cout << "Using Boost "
						<< BOOST_VERSION / 100000     << "."  // major version
						<< BOOST_VERSION / 100 % 1000 << "."  // minor version
						<< BOOST_VERSION % 100                // patch level
						<< std::endl;
	bot_->getEvents().onCommand("start", [this](TgBot::Message::Ptr message)
	{
		this->actions_->changeLanguage(this->getUser(message->chat->id));
	});


	bot_->getEvents().onCommand("setnick", [this](TgBot::Message::Ptr message)
	{
		this->actions_->changeNickName(this->getUser(message->chat->id));
	});

	bot_->getEvents().onCommand("setlang", [this](TgBot::Message::Ptr message)
	{
		this->actions_->changeLanguage(this->getUser(message->chat->id));
	});
	{
		auto nextPartnerLambda = [this](TgBot::Message::Ptr &message, bool handleMultiple)
		{
			auto user = this->getUser(message->chat->id);
			if(user->searchingStatus() != PARTNER_SEARCHING_STARTED)
			{
				if(user->searchingStatus() == PARTNER_SEARCHING_FOUND)
				{
					if(!handleMultiple)
						return;
					this->actions_->endRoomChatting(user);
				}
				this->actions_->startPartnerSearching(user);
			}
		};
		bot_->getEvents().onCommand("go", [&nextPartnerLambda](TgBot::Message::Ptr message)
		{
			nextPartnerLambda(message, false);
		});
		bot_->getEvents().onCommand("next",[&nextPartnerLambda](TgBot::Message::Ptr message)
		{
			nextPartnerLambda(message, true);
		});
	}
	bot_->getEvents().onCommand("end", [this](TgBot::Message::Ptr message){
		auto user = this->getUser(message->chat->id);
		this->actions_->endRoomChatting(user);
	});

	bot_->getEvents().onAnyMessage([this](TgBot::Message::Ptr message)
	{
		this->getUser(message->chat->id)->setLastMessageTime(std::time_t(std::time(nullptr)));
		printf("%s %s : %s\n",
					 message->chat->firstName.c_str(),
					 message->chat->lastName.c_str(),
					 message->text.c_str());
	});

	bot_->getEvents().onNonCommandMessage([this](TgBot::Message::Ptr message)
	{
		std::shared_ptr<BotUser> user = this->getUser(message->chat->id);
		switch(user->waitingStatus())
		{
			case INPUT_WAITING_NICKNAME:
			{
				this->actions_->handleNickNameChange(user, message->text);
				break;
			}
			case INPUT_WAITING_LANGUAGE:
			{
				this->actions_->handleLanguageChange(user, message->text);
				break;
			}
			default:
			{
				if(user->searchingStatus() == PARTNER_SEARCHING_FOUND)
				{
					this->actions_->handlePartnerChat(user, message);
				}
				break;
			}
		}
	});
}

RandomChatBot::~RandomChatBot()
{
#ifdef DEBUG
	printf("~%s\n", __FUNCTION__);
#endif
}

std::shared_ptr<BotUser> &RandomChatBot::getUser(int64_t userId)
{
	auto result = users_->find(userId);
	if(result != users_->end())
		return result->second;

	auto user = std::make_shared<BotUser>(userId);
	sqlWrapper_.insertUser(user);
	return (users_->operator[](userId) = user);
}

void RandomChatBot::disconnectIdlersFromChats()
{
	std::time_t currentTime = std::time(nullptr);
	for (auto &userPair : *users_)
	{
		auto selectedUser = userPair.second;
		if(selectedUser->searchingStatus() == PARTNER_SEARCHING_FOUND &&
			 std::difftime(currentTime, selectedUser->lastMessageTime()) > 600.0)
		{
			actions_->endRoomChatting(selectedUser);
		}
	}
}


void RandomChatBot::start()
{
	messageManager_->startSending();
	poll_.setTimeout(messageManager_->getUpdateDelayTime());
	poll_.start();
	timerManager_.tick();
	disconnectIdlersFromChats();
	for (auto &user : *users_) {
		if (user.second->changedFields() != 0) {
			sqlWrapper_.updateUser(user.second);
			user.second->setChangedFields(0);
		}
	}
}

