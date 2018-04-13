/*************************************************************************
> File Name: Animation.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Animation class for CubbyFlow C# API.
> Created Time: 2018/04/12
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_CSHARP_ANIMATION_H
#define CUBBYFLOW_CSHARP_ANIMATION_H

#include <Core/Animation/Animation.h>
#include <API/CSharp/Animation/Frame.h>

namespace CubbyFlowSharp
{
	//!
	//! \brief Abstract base class for animation-related class.
	//!
	//! This class represents the animation logic in very abstract level.
	//! Generally animation is a function of time and/or its previous state.
	//! This base class provides a virtual function update() which can be
	//! overriden by its sub-classes to implement their own state update logic.
	//!
	class Animation
	{
	public:
		void Update(Frame& frame) const;

	protected:
		::CubbyFlow::Animation* m_pAnimation;
	};
}

#endif