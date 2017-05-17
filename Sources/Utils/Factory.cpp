/*************************************************************************
> File Name: Factory.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Factory methods for CubbyFlow.
> Created Time: 2017/05/01
> Copyright (c) 2017, Chan-Ho Chris Ohk
*************************************************************************/
#include <Searcher/PointParallelHashGridSearcher2.h>
#include <Searcher/PointParallelHashGridSearcher3.h>
#include <Utils/Factory.h>

#include <memory>
#include <unordered_map>

namespace CubbyFlow
{
	std::unordered_map<std::string, PointNeighborSearcherBuilder2Ptr> sPointNeighborSearcher2Builders;
	std::unordered_map<std::string, PointNeighborSearcherBuilder3Ptr> sPointNeighborSearcher3Builders;

#define REGISTER_BUILDER(map, ClassName) \
	map.emplace(#ClassName, std::make_shared<ClassName::Builder>());

#define REGISTER_POINT_NEIGHBOR_SEARCHER2_BUILDER(ClassName) \
    REGISTER_BUILDER(sPointNeighborSearcher2Builders, ClassName)
#define REGISTER_POINT_NEIGHBOR_SEARCHER3_BUILDER(ClassName) \
    REGISTER_BUILDER(sPointNeighborSearcher3Builders, ClassName)

	class Registry
	{
	public:
		Registry()
		{
			REGISTER_POINT_NEIGHBOR_SEARCHER2_BUILDER(PointParallelHashGridSearcher2)

			REGISTER_POINT_NEIGHBOR_SEARCHER3_BUILDER(PointParallelHashGridSearcher3)
		}
	};

	static Registry sRegistry;

	PointNeighborSearcher2Ptr Factory::BuildPointNeighborSearcher2(const std::string& name)
	{
		auto result = sPointNeighborSearcher2Builders.find(name);
		if (result != sPointNeighborSearcher2Builders.end())
		{
			auto builder = result->second;
			return builder->BuildPointNeighborSearcher();
		}
		
		return nullptr;
	}

	PointNeighborSearcher3Ptr Factory::BuildPointNeighborSearcher3(const std::string& name)
	{
		auto result = sPointNeighborSearcher3Builders.find(name);
		if (result != sPointNeighborSearcher3Builders.end())
		{
			auto builder = result->second;
			return builder->BuildPointNeighborSearcher();
		}

		return nullptr;
	}
}