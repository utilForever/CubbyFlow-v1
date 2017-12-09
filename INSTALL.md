This document explains how to build, test, and install the SDK.

## Building the Code

To get started, clone the code from the repository and also download dependent libraries by running

```
git clone https://github.com/utilForever/CubbyFlow.git
cd CubbyFlow.git
git submodule init
git submodule update
```

To build the code, a compiler that supports C++17 is required. Platform-specific build instructions are described below.

### Building from macOS

CubbyFlow supports OS X 10.12.6 Sierra or higher. Also, Xcode 9 or higher and the command line tools are required for building CubbyFlow. Once ready, install [Homebrew](http://brew.sh) and run the following command line to setup [CMake](https://cmake.org/):

```
brew install cmake python
```

> Note that we want `brew` version of Python which is recommended. You can still use macOS's default Python.

Once CMake is installed, build the code by running

```
mkdir build
cd build
cmake ..
make
```

> Of course, use `make -j<num_threads>` flag to boost up the build performance by using multithreads.

This will build entire codebase. To run the unit test, execute

```
bin/UnitTests
```

It should show all the tests are passing.

### Building from Ubuntu

CubbyFlow supports Ubuntu 17.04 or higher. Using `apt-get`, install required tools and libraries by running,

```
sudo apt-get install build-essential python-dev python-pip cmake
```

This will install GNU compilers, python, and CMake. Once installed, build the code by running

```
mkdir build
cd build
cmake ..
make
```

> Again, use `make -j<num_threads>` flag to boost up the build performance by using multithreads.

This will build entire codebase. To run the unit test, execute

```
bin/UnitTests
```

It should show all the tests are passing.

### Building from Windows

To build the code on Windows, CMake, Python, and Visual Studio 2017 (or higher) is required. Windows' version of CMake is available from [this website](https://cmake.org/), Python installer can be downloaded from [here](https://python.org/). For Python, version 2.7.9 or later is recommended. To install Visual Studio, the community edition of the tool can be downloaded from [Visual Studio Community 2017](https://www.Visualstudio.com/en-us/products/Visual-studio-community-vs.aspx).

Once everything is installed, run the following commands:

```
md build
cd build
cmake .. -G"Visual Studio 15 2017 Win64"
```

This will generate 64-bit version of VS 2017 solution and projects. Once executed, you can find `CubbyFlow.sln` solution file in the `build` directory. Open the solution file and hit `Ctrl + Shift + B` to build the entire solution. Set `UnitTests` as a start-up project and hit `Ctrl + F5` to run the test.

Alternatively, you can use MSBuild to build the solution from the command prompt. In such case, simply run:

```
MSBuild CubbyFlow.sln /p:Configuration=Release
```

This will build the whole solution in release mode. Once built, run the following command to execute unit tests:

```
bin\Release\UnitTests.exe
```

### Running Tests

There are two different tests in the codebase including the unit test and manual test. For the detailed instruction on how to run those tests, please checkout the documentation page from [the project website](https://utilforever.github.io/CubbyFlow/Documentation/).

### Installing C++ SDK

For macOS and Ubuntu platforms, the library can be installed by running

```
cmake .. -DCMAKE_INSTALL_PREFIX=_INSTALL_PATH_
make
make install
```

This will install the header files and the static library `libCubbyFlow.a` under `_INSTALL_PATH_`.

For Windows, run:

```
cmake .. -G"Visual Studio 15 2017 Win64" -DCMAKE_INSTALL_PREFIX=_INSTALL_PATH_
```

Then, build `INSTALL` project under `CubbyFlow.sln`. This will install the header files and the static library `CubbyFlow.lib` under `_INSTALL_PATH_`.