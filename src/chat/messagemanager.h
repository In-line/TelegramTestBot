#ifndef MESSAGEMANAGER_H
#define MESSAGEMANAGER_H

#include <string>
#include <ctime>
#include <queue>
#include <memory>

#include <tgbot/types/GenericReply.h>

class Message
{
	std::string	message_;
	int64_t receiver_;
	TgBot::GenericReply::Ptr replyMarkup_;
	std::time_t sendTime_;
public:
	Message(const int64_t receiver,const	std::string &message, TgBot::GenericReply::Ptr replyMarkup);
	~Message();
	std::string message() const;
	void setMessage(const std::string &message);

	int64_t receiver() const;
	void setReceiver(const int64_t &receiver);

	const std::time_t &sendTime() const;
	void setSendTime(const std::time_t &sendTime);

	TgBot::GenericReply::Ptr replyMarkup() const;
	void setReplyMarkup(const TgBot::GenericReply::Ptr &replyMarkup);
};

#include <tgbot/Bot.h>
#include <tgbot/Api.h>

class MessageManager
{
private:
	std::queue<std::shared_ptr<Message>> queue_;
	std::shared_ptr<TgBot::Bot> bot_;

	int32_t updateDelayTime_ = 60;
public:
	MessageManager(const std::shared_ptr<TgBot::Bot> &bot);
	~MessageManager();
	void sendMessage(const std::shared_ptr<Message> &message);
	void sendMessage(const Message &message);
	void sendMessage(const int64_t receiver,const std::__cxx11::string &message,
							const TgBot::GenericReply::Ptr &replyMark = TgBot::GenericReply::Ptr());
	void startSending();

	int32_t getUpdateDelayTime();
};

#endif // MESSAGEMANAGER_H
