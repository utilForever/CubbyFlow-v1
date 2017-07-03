/*************************************************************************
> File Name: main.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Convert particles to object file.
> Created Time: 2017/07/03
> Copyright (c) 2017, Chan-Ho Chris Ohk
*************************************************************************/
#include <Array/Array1.h>
#include <BoundingBox/BoundingBox3.h>
#include <Geometry/TriangleMesh3.h>
#include <Size/Size3.h>
#include <SPH/SPHSystemData3.h>
#include <Utils/Serialization.h>

#include <pystring/pystring.h>

#include <getopt.h>

#include <algorithm>
#include <fstream>
#include <string>
#include <vector>

using namespace CubbyFlow;

void PrintUsage()
{
    printf(
        "Usage: particles2obj "
        "-i input_pos -o output_obj "
        "-r resx, resy, resz "
        "-g dx, dy, dz "
        "-n ox, oy, oz "
        "-k kernel_radius\n"
        "   -i, --input: input particle position file name\n"
        "   -o, --output: output obj file name\n"
        "   -r, --resolution: grid resolution in CSV format "
            "(default: 100, 100, 100)\n"
		"   -g, --grid_spacing: grid spacing in CSV format "
            "(default: 0.01, 0.01, 0.01)\n"
        "   -n, --origin: domain origin in CSV format (default: 0, 0, 0)\n"
        "   -k, --kernel: interpolation kernel radius (default: 0.2)\n"
        "   -h, --help: print this message\n");
}

void PrintInfo(const Size3& resolution, const BoundingBox3D& domain, const Vector3D& gridSpacing, size_t numberOfParticles)
{
    printf(
        "Resolution: %zu x %zu x %zu\n",
        resolution.x, resolution.y, resolution.z);
    printf(
        "Domain: [%f, %f, %f] x [%f, %f, %f]\n",
        domain.lowerCorner.x, domain.lowerCorner.y, domain.lowerCorner.z,
        domain.upperCorner.x, domain.upperCorner.y, domain.upperCorner.z);
    printf(
        "Grid spacing: [%f, %f, %f]\n",
        gridSpacing.x, gridSpacing.y, gridSpacing.z);
    printf("Number of particles: %zu\n", numberOfParticles);
}

void TriangulateAndSave(const ScalarGrid3& sdf, const std::string& objFileName)
{
	TriangleMesh3 mesh;
    MarchingCubes(
        sdf.ConstDataAccessor(),
        sdf.GridSpacing(),
        sdf.DataOrigin(),
        &mesh,
        0.0,
		DIRECTION_ALL);

    std::ofstream file(objFileName.c_str());
    if (file)
	{
        printf("Writing %s...\n", objFileName.c_str());
        mesh.WriteObj(&file);
        file.close();
    }
	else
	{
        printf("Cannot write file %s.\n", objFileName.c_str());
        exit(EXIT_FAILURE);
    }
}

void ParticlesToObj(
    const Array1<Vector3D>& positions,
    const Size3& resolution,
    const Vector3D& gridSpacing,
    const Vector3D& origin,
    double kernelRadius,
    const std::string& objFileName)
{
    SPHSystemData3 sphParticles;
    sphParticles.AddParticles(positions);
    sphParticles.SetRelativeKernelRadius(2.0);
    sphParticles.SetTargetSpacing(kernelRadius / 2.0);
    sphParticles.BuildNeighborSearcher();
    sphParticles.UpdateDensities();

    VertexCenteredScalarGrid3 sdf(resolution, gridSpacing, origin);
    PrintInfo(resolution, sdf.GetBoundingBox(), gridSpacing, sphParticles.NumberOfParticles());

    Array1<double> constData(sphParticles.NumberOfParticles(), 1.0);
    sdf.Fill([&](const Vector3D& pt)
	{
        double d = sphParticles.Interpolate(pt, constData);
        return 0.5 - d;
    });

    TriangulateAndSave(sdf, objFileName);
}

int main(int argc, char* argv[])
{
    std::string inputFileName;
    std::string outputFileName;
    Size3 resolution(100, 100, 100);
    Vector3D gridSpacing(0.01, 0.01, 0.01);
    Vector3D origin;
    double kernelRadius = 0.2;

    // Parse options
    static struct option longOptions[] =
	{
		{"input",       required_argument,  nullptr,  'i' },
		{"output",      required_argument,  nullptr,  'o' },
		{"resolution",  optional_argument,  nullptr,  'r' },
		{"gridspacing", optional_argument,  nullptr,  'g' },
		{"origin",      optional_argument,  nullptr,  'n' },
		{"kernel",      optional_argument,  nullptr,  'k' },
		{"help",        optional_argument,  nullptr,  'h' },
		{nullptr,       0,                  nullptr,   0  }
    };

    int opt;
    int long_index = 0;
    while ((opt = getopt_long(argc, argv, "i:o:r:g:n:k:h", longOptions, &long_index)) != -1)
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
        	{
                std::vector<std::string> tokens;
                pystring::split(optarg, tokens, ",");

                if (tokens.size() == 1)
				{
                    resolution.x = resolution.y = resolution.z = static_cast<size_t>(atoi(optarg));
				}
            	else if (tokens.size() == 3)
				{
                    resolution.x = static_cast<size_t>(atoi(tokens[0].c_str()));
                    resolution.y = static_cast<size_t>(atoi(tokens[1].c_str()));
                    resolution.z = static_cast<size_t>(atoi(tokens[2].c_str()));
                }
                break;
            }
            case 'g':
        	{
                std::vector<std::string> tokens;
                pystring::split(optarg, tokens, ",");

                if (tokens.size() == 1)
				{
                    gridSpacing.x = gridSpacing.y = gridSpacing.z = atof(optarg);
                }
            	else if (tokens.size() == 3)
				{
                    gridSpacing.x = atof(tokens[0].c_str());
                    gridSpacing.y = atof(tokens[1].c_str());
                    gridSpacing.z = atof(tokens[2].c_str());
                }
                break;
            }
            case 'n':
        	{
                std::vector<std::string> tokens;
                pystring::split(optarg, tokens, ",");

                if (tokens.size() == 1)
				{
                    origin.x = origin.y = origin.z = atof(optarg);
                }
            	else if (tokens.size() == 3)
				{
                    origin.x = atof(tokens[0].c_str());
                    origin.y = atof(tokens[1].c_str());
                    origin.z = atof(tokens[2].c_str());
                }
                break;
            }
            case 'k':
                kernelRadius = atof(optarg);
                break;
            case 'h':
                PrintUsage();
                exit(EXIT_SUCCESS);
            default:
                PrintUsage();
                exit(EXIT_FAILURE);
        }
    }

    if (inputFileName.empty() || outputFileName.empty())
	{
        PrintUsage();
        exit(EXIT_FAILURE);
    }

    // Read particle positions
    Array1<Vector3D> positions;
    std::ifstream positionFile(inputFileName.c_str(), std::ifstream::binary);
    if (positionFile)
	{
        std::vector<uint8_t> buffer((std::istreambuf_iterator<char>(positionFile)), (std::istreambuf_iterator<char>()));
        Deserialize(buffer, &positions);
        positionFile.close();
    }
	else
	{
        printf("Cannot read file %s.\n", inputFileName.c_str());
        exit(EXIT_FAILURE);
    }

    // Run marching cube and save it to the disk
    ParticlesToObj(positions, resolution, gridSpacing, origin, kernelRadius, outputFileName);

    return EXIT_SUCCESS;
}