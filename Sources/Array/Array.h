/*************************************************************************
> File Name: Array.h
> Project Name: Cubby
> Author: Chan-Ho Chris Ohk
> Purpose: Generic N-dimensional array class interface.
> Created Time: 2017/01/23
> Copyright (c) 2017, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_ARRAY_H
#define CUBBYFLOW_ARRAY_H

namespace CubbyFlow
{
	template <typename T, size_t N>
	class Array final
	{
	public:
		static_assert(N < 1 || N > 3, "Not implemented - N should be either 1, 2, or 3.");
	};
}

#endif