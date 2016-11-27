#include "sqlwrapper.h"

#include "sql_exception.h"

#include <unordered_set>
#include <cstring>
void SqlWrapper::loadDatabase()
{
	pqxx::work worker(db_connection_);
	pqxx::result roomsResult = worker.exec("SELECT * FROM public.bot_rooms");
	for (
			 pqxx::result::const_iterator roomsRow = roomsResult.begin();
			 roomsRow != roomsResult.end();
			 ++roomsRow
			 )
	{
		int64_t virtualRoomId = roomsRow["id"].as<int64_t>();
		pqxx::result usersResult = worker.exec(
																 "SELECT * FROM public.bot_users WHERE virtual_room ="+
																 std::to_string(virtualRoomId)+";");
		std::shared_ptr<VirtualRoom> room;
		if(virtualRoomId != 0) room.reset(new VirtualRoom());
		else room.reset();
		for (
				 pqxx::result::const_iterator usersRow = usersResult.begin();
				 usersRow != usersResult.end();
				 ++usersRow
				 )
		{
			/*
			 * language text NOT NULL DEFAULT 'en'::text, "
					"nickname text NOT NULL DEFAULT 'Anonymous'::text, "
					"chat_id bigint NOT NULL DEFAULT 0, "
					"virtual_id bigint NOT NULL DEFAULT 0, "
					"virtual_room bigint NOT NULL DEFAULT 0, "
					"CONSTRAINT \"bot_users_fkey\" FOREIGN KEY (virtual_room) "
					"REFERENCES public.bot_rooms (id) MATCH SIMPLE "
					"ON UPDATE NO ACTION ON DELETE NO ACTION, "
					"warns integer NOT NULL DEFAULT 0, "
					"waiting_status integer NOT NULL DEFAULT 0, "
					"searching_status integer NOT NULL DEFAULT 0 "
			 */
			std::shared_ptr<BotUser> loadingUser = std::make_shared<BotUser>();
			loadingUser->setLanguage(usersRow["language"].as<std::string>());
			loadingUser->setId(usersRow["chat_id"].as<int64_t>());
			loadingUser->setNickname(usersRow["nickname"].as<std::string>());
			loadingUser->setVirtualId(usersRow["virtual_id"].as<int64_t>());
			loadingUser->setWarns(usersRow["warns"].as<short>());
			loadingUser->setWaitingStatus((INPUT_WAITING)usersRow["waiting_status"].as<short>());
			loadingUser->setSearchingStatus((PARTNER_SEARCHING)usersRow["searching_status"].as<short>());
			if (virtualRoomId != 0)
			{
				room->push_back(loadingUser);
			}
			loadingUser->setRoom(room);
			users_->operator[](loadingUser->id()) = loadingUser;

			switch(loadingUser->waitingStatus())
			{
				case INPUT_WAITING_LANGUAGE:
					actions_->changeLanguage(loadingUser);
					break;
				case INPUT_WAITING_NICKNAME:
					actions_->changeNickName(loadingUser);
					break;
				default:
					break;
			}

			switch(loadingUser->searchingStatus())
			{
				case PARTNER_SEARCHING_STARTED:
					loadingUser->setSearchingStatus(PARTNER_SEARCHING_NONE);
					actions_->startPartnerSearching(loadingUser);
					break;
				default:
					break;
			}
			loadingUser->setChangedFields(0);
		}
	}
}

SqlWrapper::SqlWrapper(std::shared_ptr<UsersCollection> &users, std::shared_ptr<UserActions> &actions) :
	db_connection_("dbname=test_db user=user password=1 host=localhost"),
	users_(users),
	actions_(actions)
{
	if(!db_connection_.is_open())
		throw sql_exception("SQLWrapper: Cannot create DB connection..");


	try
	{
		pqxx::work worker(db_connection_);
		worker.exec(
					"CREATE SEQUENCE public.bot_rooms_id_seq "
					"INCREMENT 1 "
					"MINVALUE 1 "
					"MAXVALUE 9223372036854775807 "
					"START 1 "
					"CACHE 1; "
					"ALTER TABLE public.bot_rooms_id_seq "
					"OWNER TO \"user\";"
					);
		worker.commit();
	}
	catch(pqxx::sql_error &v)
	{
		if(strstr(v.what(), "already exists") == nullptr) throw ;
	}

	try
	{
		pqxx::work worker(db_connection_);
		worker.exec(
					"CREATE SEQUENCE public.bot_users_id_seq "
					"INCREMENT 1 "
					"MINVALUE 1 "
					"MAXVALUE 9223372036854775807 "
					"START 1 "
					"CACHE 1; "
					"ALTER TABLE public.bot_users_id_seq "
					"OWNER TO \"user\";"
					);
		worker.commit();
	}
	catch (pqxx::sql_error &v)
	{
		if(strstr(v.what(), "already exists") == nullptr) throw;
	}
	{

		pqxx::work worker(db_connection_);

		worker.exec(
					"CREATE TABLE IF NOT EXISTS public.bot_rooms "
					"( "
					"id bigint NOT NULL DEFAULT nextval('bot_rooms_id_seq'::regclass), "
					"CONSTRAINT bot_rooms_pkey PRIMARY KEY (id) "
					") "
					"WITH ( "
					"OIDS=FALSE "
					"); "
					"ALTER TABLE public.bot_rooms "
					"OWNER TO \"user\";"
					);
		worker.exec(
					"CREATE TABLE IF NOT EXISTS public.bot_users "
					"( "
					"id bigint NOT NULL DEFAULT nextval('bot_users_id_seq'::regclass), "
					"CONSTRAINT bot_users_pkey PRIMARY KEY (id), "
					"language text NOT NULL DEFAULT 'en'::text, "
					"nickname text NOT NULL DEFAULT 'Anonymous'::text, "
					"chat_id bigint NOT NULL DEFAULT 0, "
					"virtual_id bigint NOT NULL DEFAULT 0, "
					"virtual_room bigint NOT NULL DEFAULT 0, "
					"CONSTRAINT \"bot_users_fkey\" FOREIGN KEY (virtual_room) "
					"REFERENCES public.bot_rooms (id) MATCH SIMPLE "
					"ON UPDATE NO ACTION ON DELETE NO ACTION, "
					"warns integer NOT NULL DEFAULT 0, "
					"waiting_status integer NOT NULL DEFAULT 0, "
					"searching_status integer NOT NULL DEFAULT 0 "
					") "
					"WITH ( "
					"OIDS=FALSE "
					"); "
					"ALTER TABLE public.bot_users "
					"OWNER TO \"user\"; ");

		worker.commit();
	}
	try
	{
		pqxx::work worker(db_connection_);
		worker.exec("INSERT INTO public.bot_rooms VALUES ('0');");
		worker.commit();
	}
	catch(pqxx::unique_violation&) {}

	try
	{
		pqxx::work secondWorker(db_connection_);
		secondWorker.exec(
					"CREATE INDEX bot_users_id_fkey "
					"ON public.bot_users "
					"USING btree "
					"(virtual_room);"
					);
		secondWorker.commit();
	}
	catch (pqxx::sql_error &v)
	{
		if(strstr(v.what(), "already exists") == nullptr) throw;
	}
	loadDatabase();
#ifdef DEBUG
	printf("%s\n", __FUNCTION__);
#endif
}

SqlWrapper::~SqlWrapper()
{
#ifdef DEBUG
	printf("~%s\n", __FUNCTION__);
#endif
}

void SqlWrapper::updateUser(std::shared_ptr<BotUser> &user)
{
	int64_t iBitFields = user->changedFields();
	if(iBitFields != 0)
	{

		std::string query = "UPDATE public.bot_users SET ";
		{
			pqxx::work worker(db_connection_);
			if(iBitFields & (1<<CHANGED_LANGUAGE))
				query+="language="+worker.quote(user->language())+"::text ,";

			if(iBitFields & (1<<CHANGED_NICKNAME))
				query+="nickname="+worker.quote(user->nickname())+"::text ,";

			if(iBitFields & (1<<CHANGED_ID))
				query+= "virtual_id="+std::to_string(user->virtualId()) + " ,";

			if(iBitFields & (1<<CHANGED_WARNS))
				query+= "warns="+std::to_string(user->warns()) + ",";

			if(iBitFields & (1<<CHANGED_WAITING_STATUS))
				query+= "waiting_status="+std::to_string(user->waitingStatus()) + " ,";

			if(iBitFields & (1<<CHANGED_SEARCHING_STATUS))
				query+= "searching_status="+std::to_string(user->searchingStatus())+" ,";

			query = query.erase(query.size() - 1);
			query.append(" WHERE chat_id="+std::to_string(user->id())+";");
			printf("%s\n", query.c_str());
			worker.exec(query);
			worker.commit();
		}
		if (iBitFields & (1<<CHANGED_ROOM))
		{
			pqxx::work worker(db_connection_);
			std::string chatId = std::to_string(user->id());
			int64_t oldRoomId = -1;
			{
				pqxx::result result = worker.exec("SELECT virtual_room FROM public.bot_users WHERE chat_id ="+chatId);
				if(!result.empty())
				{
					auto firstRow = result.front();
					oldRoomId = firstRow["virtual_room"].as<int64_t>();
				}
			}
			int64_t newRoomId = 0;
			if(user->room())
			{
				if(user->room()->sqlId())
					newRoomId = user->room()->sqlId();
				else
				{
					worker.exec("INSERT INTO public.bot_rooms (id) VALUES (nextval('bot_rooms_id_seq'::regclass));");
					pqxx::result result =  worker.exec("SELECT currval('bot_rooms_id_seq'::regclass);");
					newRoomId = result.front()[0].as<int64_t>();
					user->room()->setSqlId(newRoomId);
				}
			}
			worker.exec("UPDATE public.bot_users SET virtual_room="+std::to_string(newRoomId)+" WHERE chat_id="+chatId+";");

			if(oldRoomId > 0)
			{
				std::string oldRoomIdString = std::to_string(oldRoomId);
				pqxx::result result = worker.exec("SELECT virtual_room FROM public.bot_users WHERE virtual_room="+oldRoomIdString+" and chat_id!="+chatId+";");
				if(result.empty())
				{
					worker.exec("DELETE FROM public.bot_rooms WHERE id!=0 and id="+std::to_string(oldRoomId));
				}
			}
			worker.commit();
		}

	}
}
void SqlWrapper::insertUser(std::shared_ptr<BotUser> &user)
{
	pqxx::work worker(db_connection_);

	std::string idString = std::to_string(user->id());
	worker.exec("DELETE FROM public.bot_users WHERE chat_id="+idString);
	worker.exec("INSERT INTO public.bot_users (chat_id) VALUES (" + idString + ")");
	worker.commit();
	int64_t fields;
	memset(&fields, 255, sizeof(fields));
	user->setChangedFields(fields);
}
