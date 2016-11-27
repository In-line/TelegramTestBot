#include "messagemanager.h"

MessageManager::MessageManager(const std::shared_ptr<TgBot::Bot> &bot) : queue_(), bot_(bot)
{
#ifdef DEBUG
	printf("%s\n", __FUNCTION__);
#endif
}

MessageManager::~MessageManager()
{
#ifdef DEBUG
	printf("~%s\n", __FUNCTION__);
#endif
}

void MessageManager::sendMessage(const std::shared_ptr<Message> &message)
{
	queue_.push(message);
}

void MessageManager::sendMessage(const Message &message)
{
	return this->sendMessage(std::make_shared<Message>(message));
}

void MessageManager::sendMessage(const int64_t receiver,
																 const std::__cxx11::string &message,
																 const TgBot::GenericReply::Ptr &replyMarkup)
{
	return this->sendMessage(std::make_shared<Message>(receiver, message, replyMarkup));
}

#include <set>
#include <vector>
void MessageManager::startSending()
{
	struct MessageLess
	{
		inline bool operator()(const std::shared_ptr<Message> &a,
													 const std::shared_ptr<Message> &b)
		{
			return a->receiver() < b->receiver();
		}
	};
	std::shared_ptr<Message> message;


	std::time_t currentTime = std::time(nullptr);

	for(short nCyclesCount = 0; nCyclesCount < 2 && !queue_.empty();++nCyclesCount)
	{
		std::set<std::shared_ptr<Message>, MessageLess> sendedMessages;
		std::vector<std::shared_ptr<Message>> multiMessages;
		for(short nSendedMessages = 0;nSendedMessages < 30 && !queue_.empty();)
		{
			message = queue_.front();
			queue_.pop();

			if(sendedMessages.find(message) == sendedMessages.end())
			{
				double time = std::difftime(currentTime, message->sendTime());

				if(time < 1.0 && time > 0.0)
				{
					usleep(time * 1000);
					currentTime = std::time(nullptr);
				}
				bot_->getApi().sendMessage(message->receiver(), message->message(), 0, 0,
																	 message->replyMarkup());
				sendedMessages.insert(message);
				++nSendedMessages;
			}
			else
				multiMessages.push_back(message);
		}
		for(auto value : multiMessages)
		{
			queue_.push(value);
		}
		sleep(1);
	}
	updateDelayTime_ = queue_.empty() ? 60 : 0;
}

int32_t MessageManager::getUpdateDelayTime()
{
	return updateDelayTime_;
}

/////////////////////////////////////////////////////////////////////
TgBot::GenericReply::Ptr Message::replyMarkup() const
{
	return replyMarkup_;
}

void Message::setReplyMarkup(const TgBot::GenericReply::Ptr &replyMarkup)
{
	replyMarkup_ = replyMarkup;
}

Message::Message(const int64_t receiver, const std::__cxx11::string &message,
								 TgBot::GenericReply::Ptr replyMarkup) :
	message_(message), receiver_(receiver), replyMarkup_(replyMarkup), sendTime_(std::time(nullptr))
{
#ifdef DEBUG
	printf("%s\n", __FUNCTION__);
#endif
}

Message::~Message()
{
#ifdef DEBUG
	printf("~%s\n", __FUNCTION__);
#endif
}

std::string Message::message() const
{
	return message_;
}

void Message::setMessage(const std::string &message)
{
	message_ = message;
}

int64_t Message::receiver() const
{
	return receiver_;
}

void Message::setReceiver(const int64_t &receiver)
{
	receiver_ = receiver;
}

const std::time_t &Message::sendTime() const
{
	return sendTime_;
}

void Message::setSendTime(const std::time_t &sendtime)
{
	sendTime_ = sendtime;
}
