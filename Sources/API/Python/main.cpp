/*************************************************************************
> File Name: main.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Main entry of CubbyFlow Python API.
> Created Time: 2018/01/22
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <API/Python/Array/ArrayAccessor.h>
#include <API/Python/Animation/Animation.h>
#include <API/Python/Animation/PhysicsAnimation.h>
#include <API/Python/BoundingBox/BoundingBox.h>
#include <API/Python/Geometry/Box.h>
#include <API/Python/Geometry/Cylinder.h>
#include <API/Python/Geometry/Plane.h>
#include <API/Python/Geometry/Sphere.h>
#include <API/Python/Geometry/Triangle.h>
#include <API/Python/Geometry/TriangleMesh.h>
#include <API/Python/Math/Quaternion.h>
#include <API/Python/Point/Point.h>
#include <API/Python/Ray/Ray.h>
#include <API/Python/Size/Size.h>
#include <API/Python/Surface/Surface.h>
#include <API/Python/Surface/SurfaceSet.h>
#include <API/Python/Surface/SurfaceToImplicit.h>
#include <API/Python/Transform/Transform.h>
#include <API/Python/Utils/Constants.h>
#include <API/Python/Utils/Logging.h>
#include <API/Python/Utils/Serializable.h>
#include <API/Python/Vector/Vector.h>

#include <pybind11/pybind11.h>

PYBIND11_MODULE(pyCubbyFlow, m)
{
	m.doc() =
		R"pbdoc(
			Voxel-based fluid simulation engine for computer games
		)pbdoc";

	// Constants
	AddConstants(m);

	// Serialization
	AddSerializable(m);

	// Trivial basic types
	AddBoundingBox2D(m);
	AddBoundingBox2F(m);
	AddBoundingBox3D(m);
	AddBoundingBox3F(m);
	AddFrame(m);
	AddPoint2UI(m);
	AddPoint3UI(m);
	AddQuaternionD(m);
	AddQuaternionF(m);
	AddRay2D(m);
	AddRay2F(m);
	AddRay3D(m);
	AddRay3F(m);
	AddSize2(m);
	AddSize3(m);
	AddTransform2(m);
	AddTransform3(m);
	AddVector2D(m);
	AddVector2F(m);
	AddVector3D(m);
	AddVector3F(m);

	// Containers/helpers
	AddArrayAccessor1(m);
	AddArrayAccessor2(m);
	AddArrayAccessor3(m);

	// Trivial APIs
	AddLogging(m);

	// Surfaces
	AddSurface2(m);
	AddSurface3(m);
	AddSurfaceSet2(m);
	AddSurfaceSet3(m);
	AddSurfaceToImplicit2(m);
	AddSurfaceToImplicit3(m);

	// Geometries
	AddBox2(m);
	AddBox3(m);
	AddCylinder3(m);
	AddPlane2(m);
	AddPlane3(m);
	AddSphere2(m);
	AddSphere3(m);
	AddTriangle3(m);
	AddTriangleMesh3(m);
	
	// Animations
	AddAnimation(m);
	AddPhysicsAnimation(m);

#ifdef VERSION_INFO
	m.attr("__version__") = pybind11::str(VERSION_INFO);
#else
	m.attr("__version__") = pybind11::str("Dev");
#endif
}