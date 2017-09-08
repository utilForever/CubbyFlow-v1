/*************************************************************************
> File Name: main.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Convert object file to spatial data format file.
> Created Time: 2017/09/08
> Copyright (c) 2017, Chan-Ho Chris Ohk
*************************************************************************/
#include <Geometry/TriangleMesh3.h>
#include <Geometry/TriangleMeshToSDF.h>
#include <Grid/VertexCenteredScalarGrid3.h>
#include <MarchingCubes/MarchingCubes.h>

#include <getopt.h>

#include <algorithm>
#include <fstream>
#include <string>
#include <vector>

using namespace CubbyFlow;

void PrintUsage()
{
	printf(
		"Usage: Obj2Sdf "
		"-i input_obj -o output_sdf -r resolution -m margin_scale\n"
		"   -i, --input: input obj file name\n"
		"   -o, --output: output sdf file name\n"
		"   -r, --resx: grid resolution in x-axis (default: 100)\n"
		"   -m, --margin: margin scale around the sdf (default: 0.2)\n"
		"   -h, --help: print this message\n");
}

void SaveTriangleMeshData(const TriangleMesh3& data, const std::string& fileName)
{
	std::ofstream file(fileName.c_str());
	if (file)
	{
		data.WriteObj(&file);
		file.close();
	}
}

int main(int argc, char* argv[])
{
	std::string inputFileName;
	std::string outputFileName;
	size_t resolutionX = 100;
	double marginScale = 0.2;

	// Parse options
	static struct option longOptions[] =
	{
		{ "input",   required_argument,  nullptr,  'i' },
		{ "output",  required_argument,  nullptr,  'o' },
		{ "resx",    optional_argument,  nullptr,  'r' },
		{ "margin",  optional_argument,  nullptr,  'm' },
		{ "help",    optional_argument,  nullptr,  'h' },
		{ nullptr,   0,                  nullptr,   0 }
	};

	int opt;
	int long_index = 0;
	while ((opt = getopt_long(argc, argv, "i:o:r:m:h", longOptions, &long_index)) != -1)
	{
		switch (opt)
		{
		case 'i':
			inputFileName = optarg;
			break;
		case 'o':
			outputFileName = optarg;
			break;
		case 'r':
			resolutionX = static_cast<size_t>(atoi(optarg));
			break;
		case 'm':
			marginScale = std::max(atof(optarg), 0.0);
			break;
		case 'h':
			PrintUsage();
			exit(EXIT_SUCCESS);
		default:
			PrintUsage();
			exit(EXIT_FAILURE);
		}
	}

	if (inputFileName.empty())
	{
		PrintUsage();
		exit(EXIT_FAILURE);
	}

	if (outputFileName.empty())
	{
		PrintUsage();
		exit(EXIT_FAILURE);
	}

	TriangleMesh3 triMesh;

	std::ifstream objFile(inputFileName.c_str());
	if (objFile)
	{
		printf("Reading obj file %s\n", inputFileName.c_str());
		triMesh.ReadObj(&objFile);
		objFile.close();
	}
	else
	{
		fprintf(stderr, "Failed to open file %s\n", inputFileName.c_str());
		exit(EXIT_FAILURE);
	}

	BoundingBox3D box = triMesh.BoundingBox();
	Vector3D scale(box.Width(), box.Height(), box.Depth());
	box.lowerCorner -= marginScale * scale;
	box.upperCorner += marginScale * scale;

	size_t resolutionY = static_cast<size_t>(std::ceil(resolutionX * box.Height() / box.Width()));
	size_t resolutionZ = static_cast<size_t>(std::ceil(resolutionX * box.Depth() / box.Width()));

	printf(
		"Vertex-centered grid size: %zu x %zu x %zu\n",
		resolutionX, resolutionY, resolutionZ);

	double dx = box.Width() / resolutionX;

	VertexCenteredScalarGrid3 grid(
		resolutionX, resolutionY, resolutionZ,
		dx, dx, dx,
		box.lowerCorner.x, box.lowerCorner.y, box.lowerCorner.z);

	BoundingBox3D domain = grid.BoundingBox();
	printf(
		"Domain size: [%f, %f, %f] x [%f, %f, %f]\n",
		domain.lowerCorner.x, domain.lowerCorner.y, domain.lowerCorner.z,
		domain.upperCorner.x, domain.upperCorner.y, domain.upperCorner.z);
	printf("Generating SDF...");

	TriangleMeshToSDF(triMesh, &grid);

	printf("done\n");

	std::ofstream sdfFile(outputFileName.c_str(), std::ofstream::binary);
	if (sdfFile)
	{
		printf("Writing to vertex-centered grid %s\n", outputFileName.c_str());

		std::vector<uint8_t> buffer;
		grid.Serialize(&buffer);
		sdfFile.write(reinterpret_cast<char*>(buffer.data()), buffer.size());
		sdfFile.close();
	}
	else
	{
		fprintf(stderr, "Failed to write file %s\n", outputFileName.c_str());
		exit(EXIT_FAILURE);
	}

	TriangleMesh3 triMesh2;
	MarchingCubes(grid.GetConstDataAccessor(), grid.GridSpacing(), grid.Origin(), &triMesh2, 0, DIRECTION_ALL);

	SaveTriangleMeshData(triMesh2, outputFileName + "_previz.obj");

	return EXIT_SUCCESS;
}