#include "botuser.h"
#include "../helper/stocks.h"

BotUser::BotUser(int64_t id) :
	id_(id), nickname_(), room_(nullptr),
	warns_(0), accessLevel_(0),
	virtualId_(random_int(1, 99999999)),
	waitingStatus_(INPUT_WAITING_NONE),
	searchingStatus_(PARTNER_SEARCHING_NONE),
	lastMessageTime_(0),
	timers_(0)
{
#ifdef DEBUG
	printf("%s\n", __FUNCTION__);
#endif
}

BotUser::~BotUser()
{
#ifdef DEBUG
	printf("~%s\n", __FUNCTION__);
#endif
}

const std::shared_ptr<VirtualRoom> BotUser::room() const
{
	return room_;
}

void BotUser::setRoom(std::shared_ptr<VirtualRoom> &room)
{
	room_ = room;
}

short BotUser::warns() const
{
    return warns_;
}

void BotUser::setWarns(short warns)
{
		addChangedBit(CHANGED_WARNS);
    warns_ = warns;
}

short BotUser::accessLevel() const
{
    return accessLevel_;
}

void BotUser::setAccessLevel(short accessLevel)
{
		addChangedBit(CHANGED_ACCESS_LEVEL);
    accessLevel_ = accessLevel;
}

int64_t BotUser::id() const
{
	return id_;
}

void BotUser::setId(const int64_t &id)
{
	addChangedBit(CHANGED_ID);
	id_ = id;
}

int BotUser::virtualId() const
{
	return virtualId_;
}

void BotUser::setVirtualId(int virtualId)
{
	addChangedBit(CHANGED_VIRTUAL_ID);
	virtualId_ = virtualId;
}

std::string BotUser::nickname() const
{
	return nickname_;
}

void BotUser::setNickname(const std::string &nickname)
{
	addChangedBit(CHANGED_NICKNAME);
	nickname_ = nickname;
}

bool BotUser::operator <(const std::shared_ptr<BotUser> &user)
{
	return user->id() < this->id();
}

INPUT_WAITING BotUser::waitingStatus() const
{
	return waitingStatus_;
}

void BotUser::setWaitingStatus(const INPUT_WAITING &status)
{
	addChangedBit(CHANGED_WAITING_STATUS);
	waitingStatus_ = status;
}

const std::string &BotUser::language() const
{
	return language_;
}

void BotUser::setLanguage(const std::string &language)
{
	addChangedBit(CHANGED_LANGUAGE);
	language_ = language;
}

void BotUser::pushTimer(std::shared_ptr<Timer> &timer)
{
	timers_.push_back(timer);
}

timers_const_iterator BotUser::eraseTimer(timers_const_iterator &position)
{
	return timers_.erase(position);
}

const std::vector<std::shared_ptr<Timer> > &BotUser::timers() const
{
	return timers_;
}

void BotUser::setTimers(const std::vector<std::shared_ptr<Timer> > &timers)
{
	timers_ = timers;
}

PARTNER_SEARCHING BotUser::searchingStatus() const
{
	return searchingStatus_;
}

void BotUser::setSearchingStatus(const PARTNER_SEARCHING &searchingStatus)
{
	addChangedBit(CHANGED_SEARCHING_STATUS);
	searchingStatus_ = searchingStatus;
}

bool BotUser::operator==(const std::shared_ptr<BotUser> &user)
{
	return this->id() == user->id();
}

bool BotUser::operator!=(const std::shared_ptr<BotUser> &user)
{
	return this->id() != user->id();
}

std::time_t BotUser::lastMessageTime() const
{
	return lastMessageTime_;
}

void BotUser::setLastMessageTime(const std::time_t &lastMessageTime)
{
	lastMessageTime_ = lastMessageTime;
}

int64_t BotUser::changedFields() const
{
	return changedFields_;
}

void BotUser::setChangedFields(const int64_t &changedFields)
{
	changedFields_ = changedFields;
}

void BotUser::addChangedBit(CHANGED bitNumber)
{
	changedFields_ |= 1 << bitNumber;
}

