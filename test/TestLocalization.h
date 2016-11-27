#ifndef TESTLOCALIZATION_H
#define TESTLOCALIZATION_H

#include <cxxtest/TestSuite.h>

#include "../src/helper/localization.cpp"
#include "../src/helper/stocks.cpp"


#include <fstream>
#include <string>

class TestLocalization : public CxxTest::TestSuite
{
	Localization* localization_ = nullptr;
public:
	void setUp()
	{
		std::ofstream f("test_dictionary.txt", std::ios::out | std::ios::trunc);

		f << "[ru]" << std::endl;
		f << "    FIRST_LINE  =   FIRST_LINE_VAL \tUE \t " << std::endl;
		localization_ = new Localization("test_dictionary.txt");
	}
	void tearDown()
	{
		delete localization_;
		std::remove("test_dictionary.txt");
	}

	void testLocalization_Tr_ReturnFirstLineVal()
	{
		TS_ASSERT(localization_->tr("ru","FIRST_LINE") == "FIRST_LINE_VAL \tUE");
	}
};

#endif // TESTLOCALIZATION_H
