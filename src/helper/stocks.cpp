#include "stocks.h"

#include <boost/random.hpp>
#include <ctime>

int random_int(int start, int end, bool generateNewSeed)
{
	static boost::random::mt19937 generator;
	static bool seedWasGenerated = false;

	if(generateNewSeed) seedWasGenerated = false;
	if(!seedWasGenerated)
	{
		generator.seed(std::time(nullptr));
		seedWasGenerated = true;
	}
	boost::uniform_int<> uni_dist(start,end);
	boost::variate_generator<boost::random::mt19937&, boost::uniform_int<> > uni(generator, uni_dist);
	return uni();
}

bool is_string_empty(const std::string &str)
{
	for(char character : str)
	{
		if(!isspace(character))
			return false;
	}
	return true;
}

std::__cxx11::string trim(const std::string &srcStr,const short type)
{
	std::string str(srcStr);
	if(type <= 0)
	{
		for(auto it = str.begin(); it!=str.end();) // TODO: Optimize regular cycle
		{
			if(!isspace(*it))
				break;
			it = str.erase(it);
		}
	}

	typedef std::string::reverse_iterator riter;
	if(type >= 0)
	{
		for(auto it = str.rbegin(); it!=str.rend();) // TODO: Optimize reverse cycle
		{
			if(!isspace(*it))
				break;
			it = riter( str.erase(std::next(it).base()) );
		}
	}
	return str;
}

std::pair<std::string, std::string> string_tok(const std::string &str, const std::string &delimiter, bool toTrim)
{
	std::pair<std::string, std::string> returnValue;
	int delimiterPos = str.find_first_of(delimiter);
	if(delimiterPos > 0)
	{
		returnValue.first = str.substr(0, delimiterPos);
		returnValue.second = str.substr(delimiterPos+delimiter.size());
		if (toTrim)
		{
			returnValue.first = trim(returnValue.first, 0);
			returnValue.second = trim(returnValue.second, 0);
		}
	}
	return returnValue;
}

void msleep(uint delay)
{
	usleep(1000 * delay);
}
