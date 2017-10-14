#include "pch.h"
#include "UnitTestsUtils.h"

#include <Vector/Vector2.h>
#include <Vector/Vector3.h>

namespace CubbyFlow
{
	const Vector2D* GetSamplePoints2()
	{
		return SAMPLE_POINTS2;
	}

	size_t GetNumberOfSamplePoints2()
	{
		return sizeof(SAMPLE_POINTS2) / sizeof(SAMPLE_POINTS2[0]);
	}

	const Vector3D* GetSamplePoints3()
	{
		return SAMPLE_POINTS3;
	}

	size_t GetNumberOfSamplePoints3()
	{
		return sizeof(SAMPLE_POINTS3) / sizeof(SAMPLE_POINTS3[0]);
	}

	const Vector2D* GetSampleDirs2()
	{
		return SAMPLE_DIRS2;
	}

	size_t GetNumberOfSampleDirs2()
	{
		return sizeof(SAMPLE_DIRS2) / sizeof(SAMPLE_DIRS2[0]);
	}

	const Vector3D* GetSampleDirs3()
	{
		return SAMPLE_DIRS3;
	}

	size_t GetNumberOfSampleDirs3()
	{
		return sizeof(SAMPLE_DIRS3) / sizeof(SAMPLE_DIRS3[0]);
	}
}