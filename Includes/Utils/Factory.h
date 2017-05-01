/*************************************************************************
> File Name: Factory.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Factory methods for CubbyFlow.
> Created Time: 2017/05/01
> Copyright (c) 2017, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_FACTORY_H
#define CUBBYFLOW_FACTORY_H

#include <Searcher/PointNeighborSearcher2.h>

namespace CubbyFlow
{
	class Factory
	{
	public:
		static PointNeighborSearcher2Ptr BuildPointNeighborSearcher2(const std::string& name);
	};
}

#endif