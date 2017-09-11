# CubbyFlow

<img src="https://github.com/utilForever/CubbyFlow/blob/master/Logo.png" align="center" />

CubbyFlow is voxel-based fluid simulation engine for computer games based on [Jet framework](https://github.com/doyubkim/fluid-engine-dev) that was created by [Doyub Kim](https://twitter.com/doyub).
The code is built on C++17 and can be compiled with commonly available compilers such as VC++. Currently tested platforms are Windows (Visual Studio 2017).

[![License](https://img.shields.io/badge/Licence-MIT-blue.svg)](https://github.com/utilForever/CubbyFlow/blob/master/LICENSE)

## Key Features

- Basic math and geometry operations and data structures
- Spatial query accelerators
- SPH and PCISPH fluid simulators
- Stable fluids-based smoke simulator
- Level set-based liquid simulator
- PIC, FLIP, and APIC fluid simulators
- Upwind, ENO, and FMM level set solvers
- Jacobi, Gauss-Seidel, SOR, MG, CG, ICCG, and MGPCG linear system solvers
- Converters between signed distance function and triangular mesh
- MPS and MPM fluid simulators
- IVOCK fluid simulators
- Multiphase fluid simulators
    - Particle-based multiphase flow
    - Grid-based two-phase flow
    - Grid-based multiphase flow
    - Grid-based bubbly flow
- Voxel surface reconstruction
- Compatible for DirectX, OpenGL, Metal, and Vulkan
- C++, Python, Go, and Rust API

Every simulator has both 2-D and 3-D implementations.

## How to Build

To learn how to build, test, and install the SDK, please check out [INSTALL.md](https://github.com/utilForever/CubbyFlow/blob/master/INSTALL.md).

## Documentation

All the documentations for the framework can be found from [the project website](https://utilforever.github.io/CubbyFlow/) incluing the API reference.

## Examples

Here are some of the example simulations generated using CubbyFlow framework. Corresponding example codes can be found under [Examples](https://github.com/utilForever/CubbyFlow/tree/master/Examples). All images are rendered using [Mitsuba renderer](https://www.mitsuba-renderer.org/) and the Mitsuba scene files can be found from [the demo directory](https://github.com/utilForever/CubbyFlow/tree/master/Demos). Find out more demos from [the project website](https://utilforever.github.io/CubbyFlow/Examples).

#### PCISPH Simulation Example

![PCISPH_dam_breaking](https://github.com/utilForever/CubbyFlow/blob/master/Medias/PCISPH_dam_breaking.png "PCISPH Example")

#### Level Set Simulation Example

![Level-set_dam_breaking](https://github.com/utilForever/CubbyFlow/blob/master/Medias/LevelSet_dam_breaking.png "Level Set Example")

#### Level Set Example with Different Viscosity (high / low)

![level_set_bunny_drop_high_viscosity](https://github.com/utilForever/CubbyFlow/blob/master/Medias/level_set_bunny_drop_high_viscosity.png "Level Set Bunny Drop - High Viscosity")
![level_set_bunny_drop_low_viscosity](https://github.com/utilForever/CubbyFlow/blob/master/Medias/level_set_bunny_drop_low_viscosity.png "Level Set Bunny Drop - Low Viscosity")

#### Smoke Simulation with Different Advection Methods (Linear / Cubic-Spline)

![rising_smoke_linear](https://github.com/utilForever/CubbyFlow/blob/master/Medias/rising_smoke_linear.png "Rising Smoke - Linear")
![rising_smoke_cubic](https://github.com/utilForever/CubbyFlow/blob/master/Medias/rising_smoke_cubic.png "Rising Smoke - Cubic")

## How To Contribute

Contributions are always welcome, either reporting issues/bugs or forking the repository and then issuing pull requests when you have completed some additional coding that you feel will be beneficial to the main project. If you are interested in contributing in a more dedicated capacity, then please contact me.

## Contact

You can contact me via e-mail (utilForever at gmail.com). I am always happy to answer questions or help with any issues you might have, and please be sure to share any additional work or your creations with me, I love seeing what other people are making.

## License

<img align="right" src="http://opensource.org/trademarks/opensource/OSI-Approved-License-100x137.png">

The class is licensed under the [MIT License](http://opensource.org/licenses/MIT):

Copyright &copy; 2017 [Chris Ohk](http://www.github.com/utilForever), [Dongmin Kim](https://github.com/dmk98), and [YoungJoong Kim](https://github.com/revsic).

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.