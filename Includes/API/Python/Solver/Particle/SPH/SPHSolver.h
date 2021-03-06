/*************************************************************************
> File Name: SPHSolver.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: SPHSolver functions for CubbyFlow Python API.
> Created Time: 2018/02/18
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_PYTHON_SPH_SOLVER_H
#define CUBBYFLOW_PYTHON_SPH_SOLVER_H

#include <pybind11/pybind11.h>

void AddSPHSolver2(pybind11::module& m);
void AddSPHSolver3(pybind11::module& m);

#endif