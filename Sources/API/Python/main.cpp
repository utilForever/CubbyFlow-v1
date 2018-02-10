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
#include <API/Python/Collider/Collider.h>
#include <API/Python/Collider/RigidbodyCollider.h>
#include <API/Python/Field/Field.h>
#include <API/Python/Field/ScalarField.h>
#include <API/Python/Field/VectorField.h>
#include <API/Python/Field/ConstantScalarField.h>
#include <API/Python/Field/ConstantVectorField.h>
#include <API/Python/Field/CustomScalarField.h>
#include <API/Python/Field/CustomVectorField.h>
#include <API/Python/Geometry/Box.h>
#include <API/Python/Geometry/Cylinder.h>
#include <API/Python/Geometry/Plane.h>
#include <API/Python/Geometry/Sphere.h>
#include <API/Python/Geometry/Triangle.h>
#include <API/Python/Geometry/TriangleMesh.h>
#include <API/Python/Grid/Grid.h>
#include <API/Python/Grid/GridSystemData.h>
#include <API/Python/Grid/ScalarGrid.h>
#include <API/Python/Grid/VectorGrid.h>
#include <API/Python/Grid/CollocatedVectorGrid.h>
#include <API/Python/Grid/CellCenteredScalarGrid.h>
#include <API/Python/Grid/CellCenteredVectorGrid.h>
#include <API/Python/Grid/VertexCenteredScalarGrid.h>
#include <API/Python/Grid/VertexCenteredVectorGrid.h>
#include <API/Python/Grid/FaceCenteredGrid.h>
#include <API/Python/Math/Quaternion.h>
#include <API/Python/Particle/ParticleSystemData.h>
#include <API/Python/Point/Point.h>
#include <API/Python/Ray/Ray.h>
#include <API/Python/SemiLagrangian/SemiLagrangian.h>
#include <API/Python/SemiLagrangian/CubicSemiLagrangian.h>
#include <API/Python/Size/Size.h>
#include <API/Python/SPH/SPHSystemData.h>
#include <API/Python/Surface/Surface.h>
#include <API/Python/Surface/SurfaceSet.h>
#include <API/Python/Surface/ImplicitSurface.h>
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

	// Animations
	AddAnimation(m);
	AddPhysicsAnimation(m);

	// Colliders
	AddCollider2(m);
	AddCollider3(m);
	AddRigidBodyCollider2(m);
	AddRigidBodyCollider3(m);

	// Fields
	AddField2(m);
	AddField3(m);
	AddScalarField2(m);
	AddScalarField3(m);
	AddVectorField2(m);
	AddVectorField3(m);
	AddConstantScalarField2(m);
	AddConstantScalarField3(m);
	AddConstantVectorField2(m);
	AddConstantVectorField3(m);
	AddCustomScalarField2(m);
	AddCustomScalarField3(m);
	AddCustomVectorField2(m);
	AddCustomVectorField3(m);

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

	// Grids
	AddGrid2(m);
	AddGrid3(m);
	AddScalarGrid2(m);
	AddScalarGrid3(m);
	AddVectorGrid2(m);
	AddVectorGrid3(m);
	AddCollocatedVectorGrid2(m);
	AddCollocatedVectorGrid3(m);
	AddCellCenteredScalarGrid2(m);
	AddCellCenteredScalarGrid3(m);
	AddCellCenteredVectorGrid2(m);
	AddCellCenteredVectorGrid3(m);
	AddVertexCenteredScalarGrid2(m);
	AddVertexCenteredScalarGrid3(m);
	AddVertexCenteredVectorGrid2(m);
	AddVertexCenteredVectorGrid3(m);
	AddFaceCenteredGrid2(m);
	AddFaceCenteredGrid3(m);

	// Semi-lagrangians
	AddSemiLagrangian2(m);
	AddSemiLagrangian3(m);
	AddCubicSemiLagrangian2(m);
	AddCubicSemiLagrangian3(m);

	// Surfaces
	AddSurface2(m);
	AddSurface3(m);
	AddSurfaceSet2(m);
	AddSurfaceSet3(m);
	AddImplicitSurface2(m);
	AddImplicitSurface3(m);
	AddSurfaceToImplicit2(m);
	AddSurfaceToImplicit3(m);

	// Data models
	AddGridSystemData2(m);
	AddGridSystemData3(m);
	AddParticleSystemData2(m);
	AddParticleSystemData3(m);
	AddSPHSystemData2(m);
	AddSPHSystemData3(m);

#ifdef VERSION_INFO
	m.attr("__version__") = pybind11::str(VERSION_INFO);
#else
	m.attr("__version__") = pybind11::str("Dev");
#endif
}