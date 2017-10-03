#include "pch.h"

#include <Solver/FLIP/FLIPSolver3.h>

using namespace CubbyFlow;

TEST(FLIPSolver3, Empty)
{
	FLIPSolver3 solver;

	for (Frame frame; frame.index < 2; ++frame)
	{
		solver.Update(frame);
	}
}