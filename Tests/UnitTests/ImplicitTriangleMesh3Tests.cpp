#include "pch.h"
#include "UnitTestsUtils.h"

#include <Geometry/Box3.h>
#include <Geometry/ImplicitTriangleMesh3.h>
#include <Surface/Implicit/SurfaceToImplicit3.h>

#include <fstream>

using namespace CubbyFlow;

TEST(ImplicitTriangleMesh3, SignedDistance)
{
	auto box = Box3::Builder()
		.WithLowerCorner({ 0, 0, 0 })
		.WithUpperCorner({ 1, 1, 1 })
		.MakeShared();
	SurfaceToImplicit3 refSurf(box);

	std::ifstream objFile("Resources/cube.obj");
	auto mesh = TriangleMesh3::Builder().MakeShared();
	mesh->ReadObj(&objFile);

	auto imesh = ImplicitTriangleMesh3::Builder()
		.WithTriangleMesh(mesh)
		.WithResolutionX(20)
		.MakeShared();

	for (auto sample : SAMPLE_POINTS3)
	{
		auto refAns = refSurf.SignedDistance(sample);
		auto actAns = imesh->SignedDistance(sample);

		EXPECT_NEAR(refAns, actAns, 1.0 / 20);
	}
}