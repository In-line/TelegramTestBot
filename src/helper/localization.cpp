#include "localization.h"

#include <fstream>
#include <vector>

#include <tgbot/tools/StringTools.h>

#include "stocks.h"

#include <algorithm>

const std::vector<std::__cxx11::string> &Localization::availableLanguages() const
{
	return availableLanguages_;
}

Localization::Localization(const std::__cxx11::string &path) :
	keys_(),
	availableLanguages_()
{

	std::ifstream file(path);
	if(file.is_open())
	{
		std::string line;
		std::string key;
		while( getline(file, line) )
		{
			if(is_string_empty(line))
				continue;

			line = trim(line);

			if(StringTools::startsWith(line, "[") && StringTools::endsWith(line, "]"))
			{
				key = line.substr(1, line.size() - 2);
				if(key != "all")
						availableLanguages_.push_back(key);
			}
			else
			{
				if(!key.empty())
				{
					std::pair<std::string, std::string> arguments = string_tok(line,"=");
					if(!arguments.first.empty() && !arguments.second.empty())
					{
						keys_[key][arguments.first] = arguments.second;
					}
				}
			}
		}
	}
	else
		throw std::runtime_error("[Localization] Cannot open file: " + path);
#ifdef DEBUG
	printf("%s\n", __FUNCTION__);
#endif
}

Localization::~Localization()
{
#ifdef DEBUG
	printf("~%s\n", __FUNCTION__);
#endif
}

void Localization::addLang(const std::string &key, const std::string &name, const std::string &value)
{
	keys_[key][name] = value;
}

bool Localization::removeLang(const std::__cxx11::string &key, const std::__cxx11::string &name)
{
	try
	{
		keys_.at(key).erase(name);
		return true;
	}
	catch(...)
	{
		return false;
	}
}

const std::__cxx11::string &Localization::operator()(const std::__cxx11::string &key, const std::__cxx11::string &name)
{
	try
	{
		return keys_.at(key).at(name);
	}
	catch(std::exception &e)
	{
		printf("%s | [%s][%s] \n", e.what(), key.c_str(), name.c_str());
		return keys_["all"]["LANG_KEY_NOT_FOUND"];
	}
}

const std::__cxx11::string &Localization::tr(const std::__cxx11::string &key, const std::__cxx11::string &name)
{
	return this->operator ()(key, name);
}
