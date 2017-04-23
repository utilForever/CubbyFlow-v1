/*************************************************************************
> File Name: animation_tests.cpp
> Created Time: 2017/04/24
> Copyright (c) 2017, Chan-Ho Chris Ohk
*************************************************************************/
#include "manual_tests.h"
#include <Animation/Animation.h>

namespace CubbyFlow
{
	class SineAnimation : public Animation
	{
	public:
		double x = 0.0;

	protected:
		void OnUpdate(const Frame& frame) override
		{
			x = std::sin(10.0 * frame.TimeInSeconds());
		}
	};

	class SineWithDecayAnimation final : public Animation
	{
	public:
		double x = 0.0;

	protected:
		void OnUpdate(const Frame& frame) override
		{
			double decay = exp(-frame.TimeInSeconds());
			x = std::sin(10.0 * frame.TimeInSeconds()) * decay;
		}
	};
}