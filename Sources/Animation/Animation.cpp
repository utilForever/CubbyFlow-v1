/*************************************************************************
> File Name: Animation.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Abstract base class for animation-related class.
> Created Time: 2017/04/19
> Copyright (c) 2017, Chan-Ho Chris Ohk
*************************************************************************/
#include <Animation/Animation.h>

namespace CubbyFlow
{
	Animation::Animation()
	{
		// Do nothing
	}

	Animation::~Animation()
	{
		// Do nothing
	}

	// TODO: Implement Logger class
	void Animation::Update(const Frame& frame)
	{
		Timer timer;

		//JET_INFO << "Begin updating frame: " << frame.index
		//	<< " timeIntervalInSeconds: " << frame.timeIntervalInSeconds
		//	<< " (1/" << 1.0 / frame.timeIntervalInSeconds
		//	<< ") seconds";

		OnUpdate(frame);

		//JET_INFO << "End updating frame (took " << timer.durationInSeconds()
		//	<< " seconds)";
	}
}