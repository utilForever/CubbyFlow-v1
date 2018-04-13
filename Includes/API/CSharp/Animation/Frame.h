/*************************************************************************
> File Name: Frame.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Frame class for CubbyFlow C# API.
> Created Time: 2018/04/11
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_CSHARP_FRAME_H
#define CUBBYFLOW_CSHARP_FRAME_H

#include <Core/Animation/Frame.h>

namespace CubbyFlowSharp
{
	//!
	//! \brief Representation of an animation frame.
	//!
	//! This struct holds current animation frame index and frame interval in
	//! seconds.
	//!
	struct Frame
	{
		//! Constructs Frame instance with 1/60 seconds time interval.
		Frame();

		//! Constructs Frame instance with given time interval.
		Frame(int newIndex, double newTimeIntervalInSeconds);

		//! Returns internal pointer.
		::CubbyFlow::Frame* GetInteralPtr();

		//! Returns the elapsed time in seconds.
		double TimeInSeconds();

		//! Advances multiple frames.
		//! \param delta Number of frames to advance.
		void Advance(int delta);

	protected:
		::CubbyFlow::Frame* m_pFrame;
	};
}

#endif