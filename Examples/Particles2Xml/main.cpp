/*************************************************************************
> File Name: main.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Convert particles to XML file.
> Created Time: 2017/09/09
> Copyright (c) 2017, Chan-Ho Chris Ohk
*************************************************************************/
#include <Array/Array1.h>
#include <Vector/Vector3.h>
#include <Utils/Serialization.h>

#include <getopt.h>

#include <fstream>
#include <string>
#include <vector>

using namespace CubbyFlow;

void PrintUsage()
{
	printf(
		"Usage: Particles2Xml "
		"-i input_pos -o output_xml \n"
		"   -i, --input: input particle position file name\n"
		"   -o, --output: output obj file name\n"
		"   -h, --help: print this message\n");
}

void PrintInfo(size_t numberOfParticles)
{
	printf("Number of particles: %zu\n", numberOfParticles);
}

void ParticlesToXML(
	const Array1<Vector3D>& positions,
	const std::string& xmlFileName)
{
	PrintInfo(positions.size());

	std::ofstream file(xmlFileName.c_str());
	if (file)
	{
		printf("Writing %s...\n", xmlFileName.c_str());

		file << "<scene version=\"0.5.0\">";

		for (const auto& pos : positions)
		{
			file << "<shape type=\"instance\">";
			file << "<ref id=\"spheres\"/>";
			file << "<transform name=\"toWorld\">";

			char buffer[64];
			snprintf(
				buffer,
				sizeof(buffer),
				"<translate x=\"%f\" y=\"%f\" z=\"%f\"/>",
				pos.x,
				pos.y,
				pos.z);
			file << buffer;

			file << "</transform>";
			file << "</shape>";
		}

		file << "</scene>";

		file.close();
	}
	else
	{
		printf("Cannot write file %s.\n", xmlFileName.c_str());
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char* argv[])
{
	std::string inputFileName;
	std::string outputFileName;

	// Parse options
	static struct option longOptions[] =
	{
		{ "input",       required_argument,  nullptr,  'i' },
		{ "output",      required_argument,  nullptr,  'o' },
		{ "help",        optional_argument,  nullptr,  'h' },
		{ nullptr,       0,                  nullptr,   0 }
	};

	int opt;
	int long_index = 0;
	while ((opt = getopt_long(argc, argv, "i:o:h", longOptions, &long_index)) != -1)
	{
		switch (opt)
		{
		case 'i':
			inputFileName = optarg;
			break;
		case 'o':
			outputFileName = optarg;
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
		std::vector<uint8_t> buffer(
			(std::istreambuf_iterator<char>(positionFile)),
			(std::istreambuf_iterator<char>()));
		Deserialize(buffer, &positions);
		positionFile.close();
	}
	else
	{
		printf("Cannot read file %s.\n", inputFileName.c_str());
		exit(EXIT_FAILURE);
	}

	// Run marching cube and save it to the disk
	ParticlesToXML(positions, outputFileName);

	return EXIT_SUCCESS;
}
