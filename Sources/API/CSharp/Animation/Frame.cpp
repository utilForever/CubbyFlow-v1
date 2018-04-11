/*************************************************************************
> File Name: Frame.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Frame class for CubbyFlow C# API.
> Created Time: 2018/04/11
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <API/CSharp/Animation/Frame.h>

namespace CubbyFlowSharp
{
	Frame::Frame() : m_pFrame(new ::CubbyFlow::Frame)
	{
		// Do nothing
	}

	Frame::Frame(int newIndex, double newTimeIntervalInSeconds) :
		m_pFrame(new ::CubbyFlow::Frame(newIndex, newTimeIntervalInSeconds))
	{
		// Do nothing
	}

	double Frame::TimeInSeconds()
	{
		return index * timeIntervalInSeconds;
	}

	void Frame::Advance(int delta)
	{
		index += delta;
	}
} 