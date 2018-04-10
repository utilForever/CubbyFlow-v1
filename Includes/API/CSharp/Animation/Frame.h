/*************************************************************************
> File Name: Frame.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Frame functions for CubbyFlow C# API.
> Created Time: 2018/04/11
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_CSHARP_FRAME_H
#define CUBBYFLOW_CSHARP_FRAME_H

#include <Core/Animation/Frame.h>

namespace CubbyFlowSharp
{
	public ref class Frame
	{
	public:
		Frame();
		Frame(int newIndex, double newTimeIntervalInSeconds);

	protected:
		::CubbyFlow::Frame* m_pFrame;
	};
}

#endif