/*************************************************************************
> File Name: Animation.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Animation class for CubbyFlow C# API.
> Created Time: 2018/04/12
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <API/CSharp/Animation/Animation.h>

namespace CubbyFlowSharp
{
	void Animation::Update(Frame& frame) const
	{
		m_pAnimation->Update(*frame.GetInteralPtr());
	}
}