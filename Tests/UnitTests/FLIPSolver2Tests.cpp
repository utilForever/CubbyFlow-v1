#include "pch.h"

#include <Solver/FLIP/FLIPSolver2.h>

using namespace CubbyFlow;

TEST(FLIPSolver2, Empty)
{
	FLIPSolver2 solver;

	for (Frame frame; frame.index < 2; ++frame)
	{
		solver.Update(frame);
	}
}