#include <cxxtest/TestSuite.h>

#include "../src/helper/stocks.cpp"


#include <set>
class TestStocks : public CxxTest::TestSuite
{
public:
	void testStocks_RandomTest_5NonMatchingResults()
	{
		std::set<int> randomNums;

		for(short i = 0; i<10;++i)
		{
			int number = random_int(0, 99999999);
			TSM_ASSERT("RandomGenerator can not generate 10 unqiue numbers",
								 randomNums.insert(number).second);
		}
	}
	void testStocks_IsStringEmpty_ReturnTrueIfStringEmpty()
	{
		std::pair<std::string,bool> testBase[] =
		{
			{"\tBiily Jean\t\t\t\t\t", false},
			{"\t\v", true},
			{"", true}
		};
		for(auto &value : testBase)
		{
			TSM_ASSERT((std::string)"is_string_empty(" + value.first + ") returned " +
								 (value.second ? "false" : "true") + " for " + value.first,
								 value.second==is_string_empty(value.first));
		}
	}
	void testStocks_Trim_ReturnTrimmedString()
	{
		std::tuple<std::string, std::string, short> testBase[] =
		{
			std::make_tuple("\t\t\t\t \v Text  ","Text  ", -1),
			std::make_tuple("\t\t\t\t \v Text  ","Text", 0),
			std::make_tuple("\t\t\t\t \v Text  ","\t\t\t\t \v Text", 1),
			std::make_tuple("1234", "1234", 0),
			std::make_tuple("", "", 0)
		};

		for(auto &value : testBase)
		{
			std::string message = std::get<0>(value);
			std::string trimmedMessage = trim(message, std::get<2>(value));
			std::string waitingTrimmedMessage = std::get<1>(value);
			TSM_ASSERT( ("trim(" + message + ") returned " +
								 trimmedMessage + " while waiting " + waitingTrimmedMessage ),
									waitingTrimmedMessage == trimmedMessage
								 );
		}
	}
	void testStocks_StringTok_TokenizeStringInTwoParts()
	{
		std::tuple<std::string, std::string, bool, std::string, std::string> testBase[] =
		{
			std::make_tuple(" \t\t\tFirst Second   ", "Se", true, "First", "cond"),
			std::make_tuple(" First Second ", "Se", false, " First ", "cond "),
			std::make_tuple("\t\t\tFirst Second   ", " ", true, "First", "Second"),
			std::make_tuple("\t\t\tFirst Second   ", " ", false, "\t\t\tFirst", "Second   "),
			std::make_tuple("First Second", "", false, "", ""),
			std::make_tuple("First Second", "", true, "", ""),
		};

		for (auto &value : testBase)
		{
			std::string srcStr = std::get<0>(value);
			std::string token = std::get<1>(value);
			bool toTrim = std::get<2>(value);

			std::pair<std::string, std::string> callResult = string_tok(srcStr, token, toTrim);

			TSM_ASSERT( ("string_tok("+ srcStr + "," + token + "," + (toTrim ? "true" : "false") + ")" +
								 " is returned wrong result(" + callResult.first + "," + callResult.second + ")"),
									callResult.first == std::get<3>(value) && callResult.second == std::get<4>(value));
		}
	}
};

