/*************************************************************************
> File Name: main.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Hybrid Liquid Simulator
> Created Time: 2017/09/13
> Copyright (c) 2017, Chan-Ho Chris Ohk
*************************************************************************/
#include <Array/ArrayUtils.h>
#include <BoundingBox/BoundingBox3.h>
#include <Collider/RigidBodyCollider3.h>
#include <Emitter/ParticleEmitterSet3.h>
#include <Emitter/VolumeParticleEmitter3.h>
#include <Geometry/Box3.h>
#include <Geometry/Cylinder3.h>
#include <Geometry/Plane3.h>
#include <Geometry/Sphere3.h>
#include <Particle/ParticleSystemData3.h>
#include <PointGenerator/GridPointGenerator3.h>
#include <Solver/FLIP/FLIPSolver3.h>
#include <Solver/PIC/PICSolver3.h>
#include <Surface/Implicit/ImplicitSurfaceSet3.h>
#include <Utils/Logger.h>

#include <pystring/pystring.h>

#ifdef CUBBYFLOW_WINDOWS
#include <direct.h>
#else
#include <sys/stat.h>
#endif

#include <getopt.h>

#include <algorithm>
#include <fstream>
#include <string>
#include <vector>

#define APP_NAME "HybridLiquidSim"

using namespace CubbyFlow;

void SaveParticleAsPos(
	const ParticleSystemData3Ptr& particles,
	const std::string& rootDir,
	unsigned int frameCnt)
{
	Array1<Vector3D> positions(particles->NumberOfParticles());
	CopyRange1(particles->GetPositions(), particles->NumberOfParticles(), &positions);
	char baseName[256];
	snprintf(baseName, sizeof(baseName), "frame_%06d.pos", frameCnt);
	std::string fileName = pystring::os::path::join(rootDir, baseName);
	std::ofstream file(fileName.c_str(), std::ios::binary);
	if (file)
	{
		printf("Writing %s...\n", fileName.c_str());
		std::vector<uint8_t> buffer;
		Serialize(positions, &buffer);
		file.write(reinterpret_cast<char*>(buffer.data()), buffer.size());
		file.close();
	}
}

void SaveParticleAsXYZ(
	const ParticleSystemData3Ptr& particles,
	const std::string& rootDir,
	unsigned int frameCnt)
{
	Array1<Vector3D> positions(particles->NumberOfParticles());
	CopyRange1(particles->GetPositions(), particles->NumberOfParticles(), &positions);
	char baseName[256];
	snprintf(baseName, sizeof(baseName), "frame_%06d.xyz", frameCnt);
	std::string fileName = pystring::os::path::join(rootDir, baseName);
	std::ofstream file(fileName.c_str());
	if (file)
	{
		printf("Writing %s...\n", fileName.c_str());
		for (const auto& pt : positions)
		{
			file << pt.x << ' ' << pt.y << ' ' << pt.z << std::endl;
		}
		file.close();
	}
}

void PrintUsage()
{
	printf(
		"Usage: " APP_NAME " "
		"-r resolution -l length -f frames -e example_num\n"
		"   -r, --resx: grid resolution in x-axis (default is 50)\n"
		"   -f, --frames: total number of frames (default is 100)\n"
		"   -p, --fps: frames per second (default is 60.0)\n"
		"   -l, --log: log file name (default is " APP_NAME ".log)\n"
		"   -o, --output: output directory name "
		"(default is " APP_NAME "_output)\n"
		"   -e, --example: example number (between 1 and 4, default is 1)\n"
		"   -m, --format: particle output format (xyz or pos. default is xyz)\n"
		"   -h, --help: print this message\n");
}

void PrintInfo(const PICSolver3Ptr& solver)
{
	auto grids = solver->GetGridSystemData();
	Size3 resolution = grids->GetResolution();
	BoundingBox3D domain = grids->GetBoundingBox();
	Vector3D gridSpacing = grids->GetGridSpacing();

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
}

void RunSimulation(
	const std::string& rootDir,
	const PICSolver3Ptr& solver,
	size_t numberOfFrames,
	const std::string& format,
	double fps)
{
	auto particles = solver->GetParticleSystemData();

	for (Frame frame(0, 1.0 / fps); frame.index < numberOfFrames; ++frame)
	{
		solver->Update(frame);
		if (format == "xyz")
		{
			SaveParticleAsXYZ(particles, rootDir, frame.index);
		}
		else if (format == "pos")
		{
			SaveParticleAsPos(particles, rootDir, frame.index);
		}
	}
}

// Water-drop example (FLIP)
void RunExample1(
	const std::string& rootDir,
	size_t resolutionX,
	unsigned int numberOfFrames,
	const std::string& format,
	double fps)
{
	// Build solver
	auto solver = FLIPSolver3::Builder()
		.WithResolution({ resolutionX, 2 * resolutionX, resolutionX })
		.WithDomainSizeX(1.0)
		.MakeShared();

	auto grids = solver->GetGridSystemData();
	auto particles = solver->GetParticleSystemData();

	Vector3D gridSpacing = grids->GetGridSpacing();
	double dx = gridSpacing.x;
	BoundingBox3D domain = grids->GetBoundingBox();

	// Build emitter
	auto plane = Plane3::Builder()
		.WithNormal({ 0, 1, 0 })
		.WithPoint({ 0, 0.25 * domain.Height(), 0 })
		.MakeShared();

	auto sphere = Sphere3::Builder()
		.WithCenter(domain.MidPoint())
		.WithRadius(0.15 * domain.Width())
		.MakeShared();

	auto emitter1 = VolumeParticleEmitter3::Builder()
		.WithSurface(plane)
		.WithSpacing(0.5 * dx)
		.WithMaxRegion(domain)
		.WithIsOneShot(true)
		.MakeShared();
	emitter1->SetPointGenerator(std::make_shared<GridPointGenerator3>());

	auto emitter2 = VolumeParticleEmitter3::Builder()
		.WithSurface(sphere)
		.WithSpacing(0.5 * dx)
		.WithMaxRegion(domain)
		.WithIsOneShot(true)
		.MakeShared();
	emitter2->SetPointGenerator(std::make_shared<GridPointGenerator3>());

	auto emitterSet = ParticleEmitterSet3::Builder()
		.WithEmitters({ emitter1, emitter2 })
		.MakeShared();

	solver->SetParticleEmitter(emitterSet);

	// Print simulation info
	printf("Running example 1 (water-drop With FLIP)\n");
	PrintInfo(solver);

	// Run simulation
	RunSimulation(rootDir, solver, numberOfFrames, format, fps);
}

// Water-drop example (PIC)
void RunExample2(
	const std::string& rootDir,
	size_t resolutionX,
	unsigned int numberOfFrames,
	const std::string& format,
	double fps)
{
	// Build solver
	auto solver = PICSolver3::Builder()
		.WithResolution({ resolutionX, 2 * resolutionX, resolutionX })
		.WithDomainSizeX(1.0)
		.MakeShared();

	auto grids = solver->GetGridSystemData();
	auto particles = solver->GetParticleSystemData();

	Vector3D gridSpacing = grids->GetGridSpacing();
	double dx = gridSpacing.x;
	BoundingBox3D domain = grids->GetBoundingBox();

	// Build emitter
	auto plane = Plane3::Builder()
		.WithNormal({ 0, 1, 0 })
		.WithPoint({ 0, 0.25 * domain.Height(), 0 })
		.MakeShared();

	auto sphere = Sphere3::Builder()
		.WithCenter(domain.MidPoint())
		.WithRadius(0.15 * domain.Width())
		.MakeShared();

	auto emitter1 = VolumeParticleEmitter3::Builder()
		.WithSurface(plane)
		.WithSpacing(0.5 * dx)
		.WithMaxRegion(domain)
		.WithIsOneShot(true)
		.MakeShared();
	emitter1->SetPointGenerator(std::make_shared<GridPointGenerator3>());

	auto emitter2 = VolumeParticleEmitter3::Builder()
		.WithSurface(sphere)
		.WithSpacing(0.5 * dx)
		.WithMaxRegion(domain)
		.WithIsOneShot(true)
		.MakeShared();
	emitter2->SetPointGenerator(std::make_shared<GridPointGenerator3>());

	auto emitterSet = ParticleEmitterSet3::Builder()
		.WithEmitters({ emitter1, emitter2 })
		.MakeShared();

	solver->SetParticleEmitter(emitterSet);

	// Print simulation info
	printf("Running example 1 (water-drop With PIC)\n");
	PrintInfo(solver);

	// Run simulation
	RunSimulation(rootDir, solver, numberOfFrames, format, fps);
}

// Dam-breaking example (FLIP)
void RunExample3(
	const std::string& rootDir,
	size_t resolutionX,
	unsigned int numberOfFrames,
	const std::string& format,
	double fps)
{
	// Build solver
	Size3 resolution{ 3 * resolutionX, 2 * resolutionX, (3 * resolutionX) / 2 };
	auto solver = FLIPSolver3::Builder()
		.WithResolution(resolution)
		.WithDomainSizeX(3.0)
		.MakeShared();

	auto grids = solver->GetGridSystemData();
	double dx = grids->GetGridSpacing().x;
	BoundingBox3D domain = grids->GetBoundingBox();
	double lz = domain.Depth();

	// Build emitter
	auto box1 = Box3::Builder()
		.WithLowerCorner({ 0, 0, 0 })
		.WithUpperCorner({ 0.5 + 0.001, 0.75 + 0.001, 0.75 * lz + 0.001 })
		.MakeShared();

	auto box2 = Box3::Builder()
		.WithLowerCorner({ 2.5 - 0.001, 0, 0.25 * lz - 0.001 })
		.WithUpperCorner({ 3.5 + 0.001, 0.75 + 0.001, 1.5 * lz + 0.001 })
		.MakeShared();

	auto boxSet = ImplicitSurfaceSet3::Builder()
		.WithExplicitSurfaces({ box1, box2 })
		.MakeShared();

	auto emitter = VolumeParticleEmitter3::Builder()
		.WithSurface(boxSet)
		.WithMaxRegion(domain)
		.WithSpacing(0.5 * dx)
		.MakeShared();

	emitter->SetPointGenerator(std::make_shared<GridPointGenerator3>());
	solver->SetParticleEmitter(emitter);

	// Build collider
	auto cyl1 = Cylinder3::Builder()
		.WithCenter({ 1, 0.375, 0.375 })
		.WithRadius(0.1)
		.WithHeight(0.75)
		.MakeShared();

	auto cyl2 = Cylinder3::Builder()
		.WithCenter({ 1.5, 0.375, 0.75 })
		.WithRadius(0.1)
		.WithHeight(0.75)
		.MakeShared();

	auto cyl3 = Cylinder3::Builder()
		.WithCenter({ 2, 0.375, 1.125 })
		.WithRadius(0.1)
		.WithHeight(0.75)
		.MakeShared();

	auto cylSet = ImplicitSurfaceSet3::Builder()
		.WithExplicitSurfaces({ cyl1, cyl2, cyl3 })
		.MakeShared();

	auto collider = RigidBodyCollider3::Builder()
		.WithSurface(cylSet)
		.MakeShared();

	solver->SetCollider(collider);

	// Print simulation info
	printf("Running example 3 (dam-breaking With FLIP)\n");
	PrintInfo(solver);

	// Run simulation
	RunSimulation(rootDir, solver, numberOfFrames, format, fps);
}

// Dam-breaking example (PIC)
void RunExample4(
	const std::string& rootDir,
	size_t resolutionX,
	unsigned int numberOfFrames,
	const std::string& format,
	double fps)
{
	// Build solver
	Size3 resolution{ 3 * resolutionX, 2 * resolutionX, (3 * resolutionX) / 2 };
	auto solver = PICSolver3::Builder()
		.WithResolution(resolution)
		.WithDomainSizeX(3.0)
		.MakeShared();

	auto grids = solver->GetGridSystemData();
	double dx = grids->GetGridSpacing().x;
	BoundingBox3D domain = grids->GetBoundingBox();
	double lz = domain.Depth();

	// Build emitter
	auto box1 = Box3::Builder()
		.WithLowerCorner({ 0, 0, 0 })
		.WithUpperCorner({ 0.5 + 0.001, 0.75 + 0.001, 0.75 * lz + 0.001 })
		.MakeShared();

	auto box2 = Box3::Builder()
		.WithLowerCorner({ 2.5 - 0.001, 0, 0.25 * lz - 0.001 })
		.WithUpperCorner({ 3.5 + 0.001, 0.75 + 0.001, 1.5 * lz + 0.001 })
		.MakeShared();

	auto boxSet = ImplicitSurfaceSet3::Builder()
		.WithExplicitSurfaces({ box1, box2 })
		.MakeShared();

	auto emitter = VolumeParticleEmitter3::Builder()
		.WithSurface(boxSet)
		.WithMaxRegion(domain)
		.WithSpacing(0.5 * dx)
		.MakeShared();

	emitter->SetPointGenerator(std::make_shared<GridPointGenerator3>());
	solver->SetParticleEmitter(emitter);

	// Build collider
	auto cyl1 = Cylinder3::Builder()
		.WithCenter({ 1, 0.375, 0.375 })
		.WithRadius(0.1)
		.WithHeight(0.75)
		.MakeShared();

	auto cyl2 = Cylinder3::Builder()
		.WithCenter({ 1.5, 0.375, 0.75 })
		.WithRadius(0.1)
		.WithHeight(0.75)
		.MakeShared();

	auto cyl3 = Cylinder3::Builder()
		.WithCenter({ 2, 0.375, 1.125 })
		.WithRadius(0.1)
		.WithHeight(0.75)
		.MakeShared();

	auto cylSet = ImplicitSurfaceSet3::Builder()
		.WithExplicitSurfaces({ cyl1, cyl2, cyl3 })
		.MakeShared();

	auto collider = RigidBodyCollider3::Builder()
		.WithSurface(cylSet)
		.MakeShared();

	solver->SetCollider(collider);

	// Print simulation info
	printf("Running example 4 (dam-breaking With PIC)\n");
	PrintInfo(solver);

	// Run simulation
	RunSimulation(rootDir, solver, numberOfFrames, format, fps);
}

int main(int argc, char* argv[])
{
	size_t resolutionX = 50;
	unsigned int numberOfFrames = 100;
	double fps = 60.0;
	int exampleNum = 1;
	std::string logFileName = APP_NAME ".log";
	std::string outputDir = APP_NAME "_output";
	std::string format = "xyz";

	// Parse options
	static struct option longOptions[] =
	{
		{ "resx",      optional_argument, nullptr , 'r' },
		{ "frames",    optional_argument, nullptr, 'f' },
		{ "fps",       optional_argument, nullptr, 'p' },
		{ "example",   optional_argument, nullptr, 'e' },
		{ "log",       optional_argument, nullptr, 'l' },
		{ "outputDir", optional_argument, nullptr, 'o' },
		{ "format",    optional_argument, nullptr, 'm' },
		{ "help",      optional_argument, nullptr, 'h' },
		{ nullptr,     0,                 nullptr,  0 }
	};

	int opt;
	int long_index = 0;
	while ((opt = getopt_long(argc, argv, "r:f:p:e:l:o:m:h", longOptions, &long_index)) != -1)
	{
		switch (opt)
		{
		case 'r':
			resolutionX = static_cast<size_t>(atoi(optarg));
			break;
		case 'f':
			numberOfFrames = static_cast<size_t>(atoi(optarg));
			break;
		case 'p':
			fps = atof(optarg);
			break;
		case 'e':
			exampleNum = atoi(optarg);
			break;
		case 'l':
			logFileName = optarg;
			break;
		case 'o':
			outputDir = optarg;
			break;
		case 'm':
			format = optarg;
			if (format != "pos" && format != "xyz")
			{
				PrintUsage();
				exit(EXIT_FAILURE);
			}
			break;
		case 'h':
			PrintUsage();
			exit(EXIT_SUCCESS);
		default:
			PrintUsage();
			exit(EXIT_FAILURE);
		}
	}

#ifdef CUBBYFLOW_WINDOWS
	_mkdir(outputDir.c_str());
#else
	mkdir(outputDir.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
#endif

	std::ofstream logFile(logFileName.c_str());
	if (logFile)
	{
		Logging::SetAllStream(&logFile);
	}

	switch (exampleNum)
	{
	case 1:
		RunExample1(outputDir, resolutionX, numberOfFrames, format, fps);
		break;
	case 2:
		RunExample2(outputDir, resolutionX, numberOfFrames, format, fps);
		break;
	case 3:
		RunExample3(outputDir, resolutionX, numberOfFrames, format, fps);
		break;
	case 4:
		RunExample4(outputDir, resolutionX, numberOfFrames, format, fps);
		break;
	default:
		PrintUsage();
		exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}