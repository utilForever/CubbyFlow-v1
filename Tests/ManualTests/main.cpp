/*************************************************************************
> File Name: main.cpp
> Created Time: 2017/04/24
> Copyright (c) 2017, Chan-Ho Chris Ohk
*************************************************************************/
#include "manual_tests.h"
#include <Utils/Logger.h>
#include <gtest/gtest.h>
#include <fstream>

int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	CreateDirectory(CUBBYFLOW_TEST_OUTPUT_DIR);

	std::ofstream logFile("manual_tests.log");
	if (logFile)
	{
		CubbyFlow::Logging::SetAllStream(&logFile);
	}

	int ret = RUN_ALL_TESTS();
	return ret;
}
