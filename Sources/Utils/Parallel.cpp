/*************************************************************************
> File Name: Parallel.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Parallel functions for CubbyFlow.
> Created Time: 2017/10/13
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Utils/Parallel.h>

#include <thread>

static unsigned int MAX_NUMBER_OF_THREADS = std::thread::hardware_concurrency();

namespace CubbyFlow
{
	void SetMaxNumberOfThreads(unsigned int numThreads)
	{
		MAX_NUMBER_OF_THREADS = std::max(numThreads, 1u);
	}

	unsigned int GetMaxNumberOfThreads()
	{
		return MAX_NUMBER_OF_THREADS;
	}
}