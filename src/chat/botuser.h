#ifndef USER_H
#define USER_H
#include <stdint.h>

#include <memory>
#include <string>

#include <vector>

#include "../timer/timer.h"

class VirtualRoom;
enum INPUT_WAITING
{
	INPUT_WAITING_NONE = 0,
	INPUT_WAITING_NICKNAME = 1,
	INPUT_WAITING_LANGUAGE = 2,
};

enum PARTNER_SEARCHING
{
	PARTNER_SEARCHING_NONE = 0,
	PARTNER_SEARCHING_STARTED = 1,
	PARTNER_SEARCHING_FOUND = 2,
};

enum CHANGED
{
	CHANGED_ID = 0,
	CHANGED_NICKNAME = 1,
	CHANGED_ROOM = 2,
	CHANGED_WARNS = 3,
	CHANGED_ACCESS_LEVEL = 4,
	CHANGED_VIRTUAL_ID = 5,
	CHANGED_LANGUAGE = 6,
	CHANGED_WAITING_STATUS = 7,
	CHANGED_SEARCHING_STATUS = 8,
};

typedef std::vector<std::shared_ptr<Timer>>::const_iterator timers_const_iterator;

class BotUser
{
public:
	explicit BotUser(int64_t id = 0);
	~BotUser();
	const std::shared_ptr<VirtualRoom> room() const;

	void setRoom(std::shared_ptr<VirtualRoom> &room);


	short warns() const;
	void setWarns(short warns);

	short accessLevel() const;
	void setAccessLevel(short accessLevel);

	int64_t id() const;
	void setId(const int64_t &id);

	int virtualId() const;
	void setVirtualId(int virtualId);

	std::string nickname() const;
	void setNickname(const std::string &nickname);

	bool operator <(const std::shared_ptr<BotUser> &user);
	INPUT_WAITING waitingStatus() const;
	void setWaitingStatus(const INPUT_WAITING &waitingStatus);

	const std::string &language() const;
	void setLanguage(const std::string &language);

	void pushTimer(std::shared_ptr<Timer> &timer);

	timers_const_iterator eraseTimer(timers_const_iterator &position);

	const std::vector<std::shared_ptr<Timer> > &timers() const;
	void setTimers(const std::vector<std::shared_ptr<Timer> > &timers);

	PARTNER_SEARCHING searchingStatus() const;
	void setSearchingStatus(const PARTNER_SEARCHING &searchingStatus);

	bool operator==(const std::shared_ptr<BotUser> &user);
	bool operator!=(const std::shared_ptr<BotUser> &user);

	std::time_t lastMessageTime() const;
	void setLastMessageTime(const std::time_t &lastMessageTime);

	int64_t changedFields() const;
	void setChangedFields(const int64_t &changedFields);

private:
	int64_t id_;
	std::string nickname_;
	std::shared_ptr<VirtualRoom> room_;
	short warns_;
	short accessLevel_;
	int virtualId_;
	std::string language_ = "en";
	INPUT_WAITING waitingStatus_ = INPUT_WAITING_NONE;

	PARTNER_SEARCHING searchingStatus_;

	std::time_t lastMessageTime_;
	std::vector<std::shared_ptr<Timer>> timers_;

	int64_t changedFields_ = 0;

	void addChangedBit(CHANGED bitNumber);
};

struct BotUsersCompare
{
	inline bool operator()(const std::shared_ptr<BotUser> &a, const std::shared_ptr<BotUser> &b) const
	{
			 return a->id() < b->id();
	}
};


#endif // USER_H
