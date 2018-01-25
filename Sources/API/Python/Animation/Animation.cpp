/*************************************************************************
> File Name: Animation.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Animation functions for CubbyFlow Python API.
> Created Time: 2018/01/20
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <API/Python/Animation/Animation.h>
#include <Core/Animation/Animation.h>
#include <Core/Animation/Frame.h>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddFrame(pybind11::module& m)
{
	pybind11::class_<Frame>(m, "Frame",
		R"pbdoc(
			Representation of an animation frame.

			This struct holds current animation frame index and frame interval in seconds.
		)pbdoc")
	.def("__init__", [](Frame& instance, int index, double timeIntervalInSeconds)
	{
		new (&instance) Frame(index, timeIntervalInSeconds);
	},
		R"pbdoc(
			Constructs Frame
			
			This method constructs Frame with index and time interval (in seconds).

			Parameters
			----------
			- index : The index (default is 0).
			- timeIntervalInSeconds : The time interval in seconds (default is 1.0 / 60.0).
		)pbdoc",
		pybind11::arg("index") = 0,
		pybind11::arg("timeIntervalInSeconds") = 1.0 / 60.0)
	.def_readwrite("index", &Frame::index,
		R"pbdoc(Index of the frame)pbdoc")
	.def_readwrite("timeIntervalInSeconds", &Frame::timeIntervalInSeconds,
		R"pbdoc(Time interval of the frame in seconds)pbdoc")
	.def("timeInSeconds", &Frame::TimeInSeconds,
		R"pbdoc(Elapsed time in seconds)pbdoc")
	.def("Advance", [](Frame& instance, int delta)
	{
		instance.Advance(delta);
	},
		R"pbdoc(
			Advances multiple frames.

			Parameters
			----------
			- delta : Number of frames to advance.
		)pbdoc",
		pybind11::arg("delta") = 1);
}

class PyAnimation : public Animation
{
public:
	using Animation::Animation;

	void OnUpdate(const Frame& frame) override
	{
		PYBIND11_OVERLOAD_PURE(void, Animation, OnUpdate, frame);
	}
};

void AddAnimation(pybind11::module& m)
{
	pybind11::class_<Animation, PyAnimation, AnimationPtr>(m, "Animation",
		R"pbdoc(
			Abstract base class for animation-related class.

			This class represents the animation logic in very abstract level.
			Generally animation is a function of time and/or its previous state.
			This base class provides a virtual function update() which can be
			overriden by its sub-classes to implement their own state update logic.
		)pbdoc")
	.def(pybind11::init<>())
	.def("Update",
		&Animation::Update,
		R"pbdoc(
			Updates animation state for given `frame`.

			Parameters
			----------
			- frame : Number of frames to advance.
		)pbdoc",
		pybind11::arg("frame"));
}