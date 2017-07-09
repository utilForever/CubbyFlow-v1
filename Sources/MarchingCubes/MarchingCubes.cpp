/*************************************************************************
> File Name: MarchingCubes.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Computes marching cubes and extract triangle mesh from grid.
> Created Time: 2017/07/09
> Copyright (c) 2017, Chan-Ho Chris Ohk
*************************************************************************/

// Marching Cubes Example Program
// by Cory Bloyd (corysama@yahoo.com)
//
// A simple, portable and complete implementation of the Marching Cubes
// and Marching Tetrahedrons algorithms in a single source file.
// There are many ways that this code could be made faster, but the
// intent is for the code to be easy to understand.
//
// For a description of the algorithm go to
// http://astronomy.swin.edu.au/pbourke/modelling/polygonise/
//
// This code is public domain.

#include <MarchingCubes/MarchingCubes.h>
#include <MarchingCubes/MarchingCubesTable.h>
#include <MarchingCubes/MarchingSquaresTable.h>

namespace CubbyFlow
{
	using MarchingCubeVertexHashKey = size_t;
	using MarchingCubeVertexID = size_t;
	using MarchingCubeVertexMap = std::unordered_map<MarchingCubeVertexHashKey, MarchingCubeVertexID>;

	inline bool QueryVertexID(
		const MarchingCubeVertexMap& vertexMap,
		MarchingCubeVertexHashKey vKey,
		MarchingCubeVertexID* vID)
	{
		auto vIter = vertexMap.find(vKey);
		if (vIter != vertexMap.end())
		{
			*vID = vIter->second;
			return true;
		}
		
		return false;
	}

	inline Vector3D Grad(
		const ConstArrayAccessor3<double>& grid,
		ssize_t i, ssize_t j, ssize_t k,
		const Vector3D& invGridSize)
	{
		Vector3D ret;
		ssize_t ip = i + 1;
		ssize_t im = i - 1;
		ssize_t jp = j + 1;
		ssize_t jm = j - 1;
		ssize_t kp = k + 1;
		ssize_t km = k - 1;
		Size3 dim = grid.Size();
		ssize_t dimx = static_cast<ssize_t>(dim.x);
		ssize_t dimy = static_cast<ssize_t>(dim.y);
		ssize_t dimz = static_cast<ssize_t>(dim.z);
		
		if (i > dimx - 2)
		{
			ip = i;
		}
		else if (i == 0)
		{
			im = 0;
		}
		if (j > dimy - 2)
		{
			jp = j;
		}
		else if (j == 0)
		{
			jm = 0;
		}
		if (k > dimz - 2)
		{
			kp = k;
		}
		else if (k == 0)
		{
			km = 0;
		}

		ret.x = 0.5f * invGridSize.x * (grid(ip, j, k) - grid(im, j, k));
		ret.y = 0.5f * invGridSize.y * (grid(i, jp, k) - grid(i, jm, k));
		ret.z = 0.5f * invGridSize.z * (grid(i, j, kp) - grid(i, j, km));
		
		return ret;
	}

	inline Vector3D SafeNormalize(const Vector3D& n)
	{
		if (n.LengthSquared() > 0.0)
		{
			return n.Normalized();
		}

		return n;
	}

	// To compute unique edge ID, map vertices+edges into
	// doubled virtual vertex indices.
	//
	// v  edge   v
	// |----*----|    -->    |-----|-----|
	// i        i+1         2i   2i+1  2i+2
	//
	inline size_t GlobalEdgeID(
		size_t i, size_t j, size_t k,
		const Size3& dim, size_t localEdgeID)
	{
		// See edgeConnection in marching_cubes_table.h for the edge ordering.
		static const int edgeOffset3D[12][3] =
		{
			{ 1, 0, 0 }, { 2, 0, 1 }, { 1, 0, 2 }, { 0, 0, 1 },
			{ 1, 2, 0 }, { 2, 2, 1 }, { 1, 2, 2 }, { 0, 2, 1 },
			{ 0, 1, 0 }, { 2, 1, 0 }, { 2, 1, 2 }, { 0, 1, 2 }
		};

		return ((2 * k + edgeOffset3D[localEdgeID][2]) * 2 * dim.y +
			(2 * j + edgeOffset3D[localEdgeID][1])) * 2 * dim.x +
			(2 * i + edgeOffset3D[localEdgeID][0]);
	}

	// To compute unique edge ID, map vertices+edges into
	// doubled virtual vertex indices.
	//
	// v  edge   v
	// |----*----|    -->    |-----|-----|
	// i        i+1         2i   2i+1  2i+2
	//
	inline size_t GlobalVertexID(
		size_t i, size_t j, size_t k,
		const Size3& dim, size_t localVertexID)
	{
		// See edgeConnection in marching_cubes_table.h for the edge ordering.
		static const int vertexOffset3D[8][3] =
		{
			{ 0, 0, 0 }, { 2, 0, 0 }, { 2, 0, 2 }, { 0, 0, 2 },
			{ 0, 2, 0 }, { 2, 2, 0 }, { 2, 2, 2 }, { 0, 2, 2 }
		};

		return ((2 * k + vertexOffset3D[localVertexID][2]) * 2 * dim.y +
			(2 * j + vertexOffset3D[localVertexID][1])) * 2 * dim.x +
			(2 * i + vertexOffset3D[localVertexID][0]);
	}

	static void SingleSquare(
		const std::array<double, 4>& data,
		const std::array<size_t, 8>& vertAndEdgeIds,
		const Vector3D& normal,
		const std::array<Vector3D, 4>& corners,
		MarchingCubeVertexMap* vertexMap,
		TriangleMesh3* mesh,
		double isoValue)
	{
		int idxFlags = 0;
		int idxVertexOfTheEdge[2];

		double alpha;
		Vector3D e[4];

		// Which vertices are inside? If i-th vertex is inside,
		// mark '1' at i-th bit. of 'idxFlags'.
		for (size_t iterVertex = 0; iterVertex < 4; ++iterVertex)
		{
			if (data[iterVertex] <= isoValue)
			{
				idxFlags |= 1 << iterVertex;
			}
		}

		// If the rect is entirely outside of the surface,
		// there is no job to be done in this marching-cube cell.
		if (idxFlags == 0)
		{
			return;
		}

		// If there are vertices which is inside the surface...
		// Which edges intersect the surface?
		// If i-th edge intersects the surface, mark '1' at i-th bit of 'idxEdgeFlags'
		int idxEdgeFlags = squareEdgeFlags[idxFlags];

		// Find the point of intersection of the surface with each edge
		for (size_t iterEdge = 0; iterEdge < 4; ++iterEdge)
		{
			// If there is an intersection on this edge
			if (idxEdgeFlags & (1 << iterEdge))
			{
				idxVertexOfTheEdge[0] = edgeConnection2D[iterEdge][0];
				idxVertexOfTheEdge[1] = edgeConnection2D[iterEdge][1];

				// Find the phi = 0 position by iteration
				Vector3D pos0 = corners[idxVertexOfTheEdge[0]];
				Vector3D pos1 = corners[idxVertexOfTheEdge[1]];

				double phi0 = data[idxVertexOfTheEdge[0]] - isoValue;
				double phi1 = data[idxVertexOfTheEdge[1]] - isoValue;

				// I think it needs perturbation a little bit.
				if (std::fabs(phi0) + std::fabs(phi1) > 1e-12)
				{
					alpha = std::fabs(phi0) / (std::fabs(phi0) + std::fabs(phi1));
				}
				else
				{
					alpha = 0.5f;
				}

				if (alpha < 0.000001f)
				{
					alpha = 0.000001f;
				}
				if (alpha > 0.999999f)
				{
					alpha = 0.999999f;
				}

				Vector3D pos = ((1.f - alpha) * pos0 + alpha * pos1);

				// What is the position of this vertex of the edge?
				e[iterEdge] = pos;
			}
		}

		// Make triangular patches.
		for (size_t iterTri = 0; iterTri < 4; ++iterTri)
		{
			// If there isn't any triangle to be built, escape this loop.
			if (triangleConnectionTable2D[idxFlags][3 * iterTri] < 0)
			{
				break;
			}

			Point3UI face;

			for (int j = 0; j < 3; ++j) 
			{
				int idxVertex = triangleConnectionTable2D[idxFlags][3 * iterTri + j];

				MarchingCubeVertexHashKey vKey = vertAndEdgeIds[idxVertex];
				MarchingCubeVertexID vID;
				
				if (QueryVertexID(*vertexMap, vKey, &vID))
				{
					face[j] = vID;
				}
				else {
					// if vertex does not exist...
					face[j] = mesh->NumberOfPoints();
					mesh->AddNormal(normal);
					
					if (idxVertex < 4)
					{
						mesh->AddPoint(corners[idxVertex]);
					}
					else
					{
						mesh->AddPoint(e[idxVertex - 4]);
					}

					// empty texture coordinate...
					mesh->AddUV(Vector2D());
					vertexMap->insert(std::make_pair(vKey, face[j]));
				}
			}

			mesh->AddPointNormalUVTriangle(face, face, face);
		}
	}

	static void SingleCube(
		const std::array<double, 8>& data,
		const std::array<size_t, 12>& edgeIds,
		const std::array<Vector3D, 8>& normals,
		const BoundingBox3D& bound,
		MarchingCubeVertexMap* vertexMap,
		TriangleMesh3* mesh,
		double isoValue)
	{
		int itrVertex, itrEdge, itrTri;
		int idxFlagSize = 0, idxEdgeFlags = 0;
		int idxVertexOfTheEdge[2];

		Vector3D pos, pos0, pos1, normal, normal0, normal1;
		double phi0, phi1;
		double alpha;
		Vector3D e[12], n[12];

		// Which vertices are inside? If i-th vertex is inside, mark '1' at i-th
		// bit. of 'idxFlagSize'.
		for (itrVertex = 0; itrVertex < 8; itrVertex++) {
			if (data[itrVertex] <= isoValue) {
				idxFlagSize |= 1 << itrVertex;
			}
		}

		// If the cube is entirely inside or outside of the surface, there is no job
		// to be done in t1his marching-cube cell.
		if (idxFlagSize == 0 || idxFlagSize == 255) {
			return;
		}

		// If there are vertices which is inside the surface...
		// Which edges intersect the surface? If i-th edge intersects the surface,
		// mark '1' at i-th bit of 'itrEdgeFlags'
		idxEdgeFlags = cubeEdgeFlags[idxFlagSize];

		// Find the point of intersection of the surface with each edge
		for (itrEdge = 0; itrEdge < 12; itrEdge++) {
			// If there is an intersection on this edge
			if (idxEdgeFlags & (1 << itrEdge)) {
				idxVertexOfTheEdge[0] = edgeConnection[itrEdge][0];
				idxVertexOfTheEdge[1] = edgeConnection[itrEdge][1];

				// cube vertex ordering to x-major ordering
				static int indexMap[8] = { 0, 1, 5, 4, 2, 3, 7, 6 };

				// Find the phi = 0 position
				pos0 = bound.corner(indexMap[idxVertexOfTheEdge[0]]);
				pos1 = bound.corner(indexMap[idxVertexOfTheEdge[1]]);

				normal0 = normals[idxVertexOfTheEdge[0]];
				normal1 = normals[idxVertexOfTheEdge[1]];

				phi0 = data[idxVertexOfTheEdge[0]] - isoValue;
				phi1 = data[idxVertexOfTheEdge[1]] - isoValue;

				alpha = distanceToZeroLevelSet(phi0, phi1);

				if (alpha < 0.000001) {
					alpha = 0.000001;
				}
				if (alpha > 0.999999) {
					alpha = 0.999999;
				}

				pos = (1.0 - alpha) * pos0 + alpha * pos1;
				normal = (1.0 - alpha) * normal0 + alpha * normal1;

				e[itrEdge] = pos;
				n[itrEdge] = normal;
			}
		}

		// Make triangles
		for (itrTri = 0; itrTri < 5; ++itrTri) {
			// If there isn't any triangle to be made, escape this loop.
			if (triangleConnectionTable3D[idxFlagSize][3 * itrTri] < 0) {
				break;
			}

			Point3UI face;

			for (int j = 0; j < 3; j++) {
				int k = 3 * itrTri + j;
				MarchingCubeVertexHashKey vKey
					= edgeIds[triangleConnectionTable3D[idxFlagSize][k]];
				MarchingCubeVertexID vId;
				if (QueryVertexID(*vertexMap, vKey, &vId)) {
					face[j] = vId;
				}
				else {
					// If vertex does not exist from the map
					face[j] = mesh->numberOfPoints();
					mesh->addNormal(
						SafeNormalize(
							n[triangleConnectionTable3D[idxFlagSize][k]]));
					mesh->addPoint(
						e[triangleConnectionTable3D[idxFlagSize][k]]);
					mesh->addUv(Vector2D());
					vertexMap->insert(std::make_pair(vKey, face[j]));
				}
			}
			mesh->addPointNormalUvTriangle(face, face, face);
		}
	}

	void marchingCubes(
		const ConstArrayAccessor3<double>& grid,
		const Vector3D& gridSize,
		const Vector3D& origin,
		TriangleMesh3* mesh,
		double isoValue,
		int bndFlag) {
		MarchingCubeVertexMap vertexMap;

		const Size3 dim = grid.size();
		const Vector3D invGridSize = 1.0 / gridSize;

		auto pos = [origin, gridSize](ssize_t i, ssize_t j, ssize_t k) {
			return origin + gridSize * Vector3D({ i, j, k });
		};

		ssize_t dimx = static_cast<ssize_t>(dim.x);
		ssize_t dimy = static_cast<ssize_t>(dim.y);
		ssize_t dimz = static_cast<ssize_t>(dim.z);

		for (ssize_t k = 0; k < dimz - 1; ++k) {
			for (ssize_t j = 0; j < dimy - 1; ++j) {
				for (ssize_t i = 0; i < dimx - 1; ++i) {
					std::array<double, 8> data;
					std::array<size_t, 12> edgeIds;
					std::array<Vector3D, 8>  normals;
					BoundingBox3D bound;

					data[0] = grid(i, j, k);
					data[1] = grid(i + 1, j, k);
					data[4] = grid(i, j + 1, k);
					data[5] = grid(i + 1, j + 1, k);
					data[3] = grid(i, j, k + 1);
					data[2] = grid(i + 1, j, k + 1);
					data[7] = grid(i, j + 1, k + 1);
					data[6] = grid(i + 1, j + 1, k + 1);

					normals[0] = Grad(grid, i, j, k, invGridSize);
					normals[1] = Grad(grid, i + 1, j, k, invGridSize);
					normals[4] = Grad(grid, i, j + 1, k, invGridSize);
					normals[5] = Grad(grid, i + 1, j + 1, k, invGridSize);
					normals[3] = Grad(grid, i, j, k + 1, invGridSize);
					normals[2] = Grad(grid, i + 1, j, k + 1, invGridSize);
					normals[7] = Grad(grid, i, j + 1, k + 1, invGridSize);
					normals[6] = Grad(grid, i + 1, j + 1, k + 1, invGridSize);

					for (int e = 0; e < 12; e++) {
						edgeIds[e] = GlobalEdgeID(i, j, k, dim, e);
					}

					bound.lowerCorner = pos(i, j, k);
					bound.upperCorner = pos(i + 1, j + 1, k + 1);

					SingleCube(
						data,
						edgeIds,
						normals,
						bound,
						&vertexMap,
						mesh,
						isoValue);
				}  // i
			}  // j
		}  // k

		   // Construct boundaries parallel to x-y plane
		vertexMap.clear();
		if (bndFlag
			& (kDirectionBack | kDirectionFront)) {
			for (ssize_t j = 0; j < dimy - 1; ++j) {
				for (ssize_t i = 0; i < dimx - 1; ++i) {
					ssize_t k = 0;

					std::array<double, 4> data;
					std::array<size_t, 8> vertexAndEdgeIds;
					std::array<Vector3D, 4> corners;
					Vector3D normal;
					BoundingBox2D bound;

					data[0] = grid(i + 1, j, k);
					data[1] = grid(i, j, k);
					data[2] = grid(i, j + 1, k);
					data[3] = grid(i + 1, j + 1, k);

					if (bndFlag & kDirectionBack) {
						normal = Vector3D(0, 0, -1);

						vertexAndEdgeIds[0] = GlobalVertexID(i, j, k, dim, 1);
						vertexAndEdgeIds[1] = GlobalVertexID(i, j, k, dim, 0);
						vertexAndEdgeIds[2] = GlobalVertexID(i, j, k, dim, 4);
						vertexAndEdgeIds[3] = GlobalVertexID(i, j, k, dim, 5);
						vertexAndEdgeIds[4] = GlobalEdgeID(i, j, k, dim, 0);
						vertexAndEdgeIds[5] = GlobalEdgeID(i, j, k, dim, 8);
						vertexAndEdgeIds[6] = GlobalEdgeID(i, j, k, dim, 4);
						vertexAndEdgeIds[7] = GlobalEdgeID(i, j, k, dim, 9);

						corners[0] = pos(i + 1, j, k);
						corners[1] = pos(i, j, k);
						corners[2] = pos(i, j + 1, k);
						corners[3] = pos(i + 1, j + 1, k);

						SingleSquare(
							data,
							vertexAndEdgeIds,
							normal,
							corners,
							&vertexMap,
							mesh,
							isoValue);
					}

					k = dimz - 2;
					data[0] = grid(i, j, k + 1);
					data[1] = grid(i + 1, j, k + 1);
					data[2] = grid(i + 1, j + 1, k + 1);
					data[3] = grid(i, j + 1, k + 1);

					if (bndFlag & kDirectionFront) {
						normal = Vector3D(0, 0, 1);

						vertexAndEdgeIds[0] = GlobalVertexID(i, j, k, dim, 3);
						vertexAndEdgeIds[1] = GlobalVertexID(i, j, k, dim, 2);
						vertexAndEdgeIds[2] = GlobalVertexID(i, j, k, dim, 6);
						vertexAndEdgeIds[3] = GlobalVertexID(i, j, k, dim, 7);
						vertexAndEdgeIds[4] = GlobalEdgeID(i, j, k, dim, 2);
						vertexAndEdgeIds[5] = GlobalEdgeID(i, j, k, dim, 10);
						vertexAndEdgeIds[6] = GlobalEdgeID(i, j, k, dim, 6);
						vertexAndEdgeIds[7] = GlobalEdgeID(i, j, k, dim, 11);

						corners[0] = pos(i, j, k + 1);
						corners[1] = pos(i + 1, j, k + 1);
						corners[2] = pos(i + 1, j + 1, k + 1);
						corners[3] = pos(i, j + 1, k + 1);

						SingleSquare(
							data,
							vertexAndEdgeIds,
							normal,
							corners,
							&vertexMap,
							mesh,
							isoValue);
					}
				}  // i
			}  // j
		}

		// Construct boundaries parallel to y-z plane
		vertexMap.clear();
		if (bndFlag
			& (kDirectionLeft | kDirectionRight)) {
			for (ssize_t k = 0; k < dimz - 1; ++k) {
				for (ssize_t j = 0; j < dimy - 1; ++j) {
					ssize_t i = 0;

					std::array<double, 4> data;
					std::array<size_t, 8> vertexAndEdgeIds;
					std::array<Vector3D, 4> corners;
					Vector3D normal;
					BoundingBox2D bound;

					data[0] = grid(i, j, k);
					data[1] = grid(i, j, k + 1);
					data[2] = grid(i, j + 1, k + 1);
					data[3] = grid(i, j + 1, k);

					if (bndFlag & kDirectionLeft) {
						normal = Vector3D(-1, 0, 0);

						vertexAndEdgeIds[0] = GlobalVertexID(i, j, k, dim, 0);
						vertexAndEdgeIds[1] = GlobalVertexID(i, j, k, dim, 3);
						vertexAndEdgeIds[2] = GlobalVertexID(i, j, k, dim, 7);
						vertexAndEdgeIds[3] = GlobalVertexID(i, j, k, dim, 4);
						vertexAndEdgeIds[4] = GlobalEdgeID(i, j, k, dim, 3);
						vertexAndEdgeIds[5] = GlobalEdgeID(i, j, k, dim, 11);
						vertexAndEdgeIds[6] = GlobalEdgeID(i, j, k, dim, 7);
						vertexAndEdgeIds[7] = GlobalEdgeID(i, j, k, dim, 8);

						corners[0] = pos(i, j, k);
						corners[1] = pos(i, j, k + 1);
						corners[2] = pos(i, j + 1, k + 1);
						corners[3] = pos(i, j + 1, k);

						SingleSquare(
							data,
							vertexAndEdgeIds,
							normal,
							corners,
							&vertexMap,
							mesh,
							isoValue);
					}

					i = dimx - 2;
					data[0] = grid(i + 1, j, k + 1);
					data[1] = grid(i + 1, j, k);
					data[2] = grid(i + 1, j + 1, k);
					data[3] = grid(i + 1, j + 1, k + 1);

					if (bndFlag & kDirectionRight) {
						normal = Vector3D(1, 0, 0);

						vertexAndEdgeIds[0] = GlobalVertexID(i, j, k, dim, 2);
						vertexAndEdgeIds[1] = GlobalVertexID(i, j, k, dim, 1);
						vertexAndEdgeIds[2] = GlobalVertexID(i, j, k, dim, 5);
						vertexAndEdgeIds[3] = GlobalVertexID(i, j, k, dim, 6);
						vertexAndEdgeIds[4] = GlobalEdgeID(i, j, k, dim, 1);
						vertexAndEdgeIds[5] = GlobalEdgeID(i, j, k, dim, 7);
						vertexAndEdgeIds[6] = GlobalEdgeID(i, j, k, dim, 5);
						vertexAndEdgeIds[7] = GlobalEdgeID(i, j, k, dim, 10);

						corners[0] = pos(i + 1, j, k + 1);
						corners[1] = pos(i + 1, j, k);
						corners[2] = pos(i + 1, j + 1, k);
						corners[3] = pos(i + 1, j + 1, k + 1);

						SingleSquare(
							data,
							vertexAndEdgeIds,
							normal,
							corners,
							&vertexMap,
							mesh,
							isoValue);
					}
				}  // j
			}  // k
		}

		// Construct boundaries parallel to x-z plane
		vertexMap.clear();
		if (bndFlag
			& (kDirectionDown | kDirectionUp)) {
			for (ssize_t k = 0; k < dimz - 1; ++k) {
				for (ssize_t i = 0; i < dimx - 1; ++i) {
					ssize_t j = 0;

					std::array<double, 4> data;
					std::array<size_t, 8> vertexAndEdgeIds;
					std::array<Vector3D, 4> corners;
					Vector3D normal;
					BoundingBox2D bound;

					data[0] = grid(i, j, k);
					data[1] = grid(i + 1, j, k);
					data[2] = grid(i + 1, j, k + 1);
					data[3] = grid(i, j, k + 1);

					if (bndFlag & kDirectionDown) {
						normal = Vector3D(0, -1, 0);

						vertexAndEdgeIds[0] = GlobalVertexID(i, j, k, dim, 0);
						vertexAndEdgeIds[1] = GlobalVertexID(i, j, k, dim, 1);
						vertexAndEdgeIds[2] = GlobalVertexID(i, j, k, dim, 2);
						vertexAndEdgeIds[3] = GlobalVertexID(i, j, k, dim, 3);
						vertexAndEdgeIds[4] = GlobalEdgeID(i, j, k, dim, 0);
						vertexAndEdgeIds[5] = GlobalEdgeID(i, j, k, dim, 1);
						vertexAndEdgeIds[6] = GlobalEdgeID(i, j, k, dim, 2);
						vertexAndEdgeIds[7] = GlobalEdgeID(i, j, k, dim, 3);

						corners[0] = pos(i, j, k);
						corners[1] = pos(i + 1, j, k);
						corners[2] = pos(i + 1, j, k + 1);
						corners[3] = pos(i, j, k + 1);

						SingleSquare(
							data,
							vertexAndEdgeIds,
							normal,
							corners,
							&vertexMap,
							mesh,
							isoValue);
					}

					j = dimy - 2;
					data[0] = grid(i + 1, j + 1, k);
					data[1] = grid(i, j + 1, k);
					data[2] = grid(i, j + 1, k + 1);
					data[3] = grid(i + 1, j + 1, k + 1);

					if (bndFlag & kDirectionUp) {
						normal = Vector3D(0, 1, 0);

						vertexAndEdgeIds[0] = GlobalVertexID(i, j, k, dim, 5);
						vertexAndEdgeIds[1] = GlobalVertexID(i, j, k, dim, 4);
						vertexAndEdgeIds[2] = GlobalVertexID(i, j, k, dim, 7);
						vertexAndEdgeIds[3] = GlobalVertexID(i, j, k, dim, 6);
						vertexAndEdgeIds[4] = GlobalEdgeID(i, j, k, dim, 4);
						vertexAndEdgeIds[5] = GlobalEdgeID(i, j, k, dim, 7);
						vertexAndEdgeIds[6] = GlobalEdgeID(i, j, k, dim, 6);
						vertexAndEdgeIds[7] = GlobalEdgeID(i, j, k, dim, 5);

						corners[0] = pos(i + 1, j + 1, k);
						corners[1] = pos(i, j + 1, k);
						corners[2] = pos(i, j + 1, k + 1);
						corners[3] = pos(i + 1, j + 1, k + 1);

						SingleSquare(
							data,
							vertexAndEdgeIds,
							normal,
							corners,
							&vertexMap,
							mesh,
							isoValue);
					}
				}  // i
			}  // k
		}
	}
}