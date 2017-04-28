///*************************************************************************
//> File Name: ParticleSystemData2.cpp
//> Project Name: CubbyFlow
//> Author: Chan-Ho Chris Ohk
//> Purpose: 2-D particle system data.
//> Created Time: 2017/04/28
//> Copyright (c) 2017, Chan-Ho Chris Ohk
//*************************************************************************/
//#include <Particle/ParticleSystemData2.h>
//#include <Particle/Searcher/PointNeighborSearcher2.h>
//
//namespace CubbyFlow
//{
//	static const size_t DEFAULT_HASH_GRID_RESOLUTION = 64;
//
//	ParticleSystemData2::ParticleSystemData2() :
//		ParticleSystemData2(0)
//	{
//		// Do nothing
//	}
//
//	ParticleSystemData2::ParticleSystemData2(size_t numberOfParticles)
//	{
//		m_positionIdx = AddVectorData();
//		m_velocityIdx = AddVectorData();
//		m_forceIdx = AddVectorData();
//
//		// Use PointParallelHashGridSearcher2 by default
//		m_neighborSearcher = std::make_shared<PointParallelHashGridSearcher2>(
//			DEFAULT_HASH_GRID_RESOLUTION,
//			DEFAULT_HASH_GRID_RESOLUTION,
//			2.0 * m_radius);
//
//		Resize(numberOfParticles);
//	}
//
//	ParticleSystemData2::ParticleSystemData2(const ParticleSystemData2& other)
//	{
//		Set(other);
//	}
//
//	ParticleSystemData2::~ParticleSystemData2()
//	{
//		// Do nothing
//	}
//
//	void ParticleSystemData2::Resize(size_t newNumberOfParticles)
//	{
//		m_numberOfParticles = newNumberOfParticles;
//
//		for (auto& attr : m_scalarDataList)
//		{
//			attr.Resize(newNumberOfParticles, 0.0);
//		}
//
//		for (auto& attr : m_vectorDataList)
//		{
//			attr.Resize(newNumberOfParticles, Vector2D());
//		}
//	}
//
//	size_t ParticleSystemData2::NumberOfParticles() const
//	{
//		return m_numberOfParticles;
//	}
//
//	size_t ParticleSystemData2::AddScalarData(double initialVal)
//	{
//		size_t attrIdx = m_scalarDataList.size();
//		m_scalarDataList.emplace_back(NumberOfParticles(), initialVal);
//		return attrIdx;
//	}
//
//	size_t ParticleSystemData2::AddVectorData(const Vector2D& initialVal)
//	{
//		size_t attrIdx = m_vectorDataList.size();
//		m_vectorDataList.emplace_back(NumberOfParticles(), initialVal);
//		return attrIdx;
//	}
//
//	double ParticleSystemData2::Radius() const {
//		return m_radius;
//	}
//
//	void ParticleSystemData2::SetRadius(double newRadius)
//	{
//		m_radius = std::max(newRadius, 0.0);
//	}
//
//	double ParticleSystemData2::Mass() const
//	{
//		return m_mass;
//	}
//
//	void ParticleSystemData2::SetMass(double newMass)
//	{
//		m_mass = std::max(newMass, 0.0);
//	}
//
//	ConstArrayAccessor1<Vector2D> ParticleSystemData2::positions() const {
//		return VectorDataAt(m_positionIdx);
//	}
//
//	ArrayAccessor1<Vector2D> ParticleSystemData2::positions() {
//		return VectorDataAt(m_positionIdx);
//	}
//
//	ConstArrayAccessor1<Vector2D> ParticleSystemData2::velocities() const {
//		return VectorDataAt(m_velocityIdx);
//	}
//
//	ArrayAccessor1<Vector2D> ParticleSystemData2::velocities() {
//		return VectorDataAt(m_velocityIdx);
//	}
//
//	ConstArrayAccessor1<Vector2D> ParticleSystemData2::forces() const {
//		return VectorDataAt(m_forceIdx);
//	}
//
//	ArrayAccessor1<Vector2D> ParticleSystemData2::forces() {
//		return VectorDataAt(m_forceIdx);
//	}
//
//	ConstArrayAccessor1<double> ParticleSystemData2::scalarDataAt(
//		size_t idx) const {
//		return m_scalarDataList[idx].constAccessor();
//	}
//
//	ArrayAccessor1<double> ParticleSystemData2::scalarDataAt(size_t idx) {
//		return m_scalarDataList[idx].accessor();
//	}
//
//	ConstArrayAccessor1<Vector2D> ParticleSystemData2::VectorDataAt(
//		size_t idx) const {
//		return m_vectorDataList[idx].constAccessor();
//	}
//
//	ArrayAccessor1<Vector2D> ParticleSystemData2::VectorDataAt(size_t idx) {
//		return m_vectorDataList[idx].accessor();
//	}
//
//	void ParticleSystemData2::addParticle(
//		const Vector2D& newPosition,
//		const Vector2D& newVelocity,
//		const Vector2D& newForce) {
//		Array1<Vector2D> newPositions = { newPosition };
//		Array1<Vector2D> newVelocities = { newVelocity };
//		Array1<Vector2D> newForces = { newForce };
//
//		addParticles(
//			newPositions.constAccessor(),
//			newVelocities.constAccessor(),
//			newForces.constAccessor());
//	}
//
//	void ParticleSystemData2::addParticles(
//		const ConstArrayAccessor1<Vector2D>& newPositions,
//		const ConstArrayAccessor1<Vector2D>& newVelocities,
//		const ConstArrayAccessor1<Vector2D>& newForces) {
//		JET_THROW_INVALID_ARG_IF(
//			newVelocities.size() > 0
//			&& newVelocities.size() != newPositions.size());
//		JET_THROW_INVALID_ARG_IF(
//			newForces.size() > 0 && newForces.size() != newPositions.size());
//
//		size_t oldNumberOfParticles = NumberOfParticles();
//		size_t newNumberOfParticles = oldNumberOfParticles + newPositions.size();
//
//		resize(newNumberOfParticles);
//
//		auto pos = positions();
//		auto vel = velocities();
//		auto frc = forces();
//
//		parallelFor(kZeroSize, newPositions.size(),
//			[&](size_t i) {
//			pos[i + oldNumberOfParticles] = newPositions[i];
//		});
//
//		if (newVelocities.size() > 0) {
//			parallelFor(kZeroSize, newPositions.size(),
//				[&](size_t i) {
//				vel[i + oldNumberOfParticles] = newVelocities[i];
//			});
//		}
//
//		if (newForces.size() > 0) {
//			parallelFor(kZeroSize, newPositions.size(),
//				[&](size_t i) {
//				frc[i + oldNumberOfParticles] = newForces[i];
//			});
//		}
//	}
//
//	const PointNeighborSearcher2Ptr& ParticleSystemData2::neighborSearcher() const {
//		return m_neighborSearcher;
//	}
//
//	void ParticleSystemData2::setNeighborSearcher(
//		const PointNeighborSearcher2Ptr& newNeighborSearcher) {
//		m_neighborSearcher = newNeighborSearcher;
//	}
//
//	const std::vector<std::vector<size_t>>&
//		ParticleSystemData2::neighborLists() const {
//		return _neighborLists;
//	}
//
//	void ParticleSystemData2::buildNeighborSearcher(double maxSearchRadius) {
//		Timer timer;
//
//		// Use PointParallelHashGridSearcher2 by default
//		m_neighborSearcher = std::make_shared<PointParallelHashGridSearcher2>(
//			DEFAULT_HASH_GRID_RESOLUTION,
//			DEFAULT_HASH_GRID_RESOLUTION,
//			2.0 * maxSearchRadius);
//
//		m_neighborSearcher->build(positions());
//
//		JET_INFO << "Building neighbor searcher took: "
//			<< timer.durationInSeconds()
//			<< " seconds";
//	}
//
//	void ParticleSystemData2::buildNeighborLists(double maxSearchRadius) {
//		Timer timer;
//
//		_neighborLists.resize(NumberOfParticles());
//
//		auto points = positions();
//		for (size_t i = 0; i < NumberOfParticles(); ++i) {
//			Vector2D origin = points[i];
//			_neighborLists[i].clear();
//
//			m_neighborSearcher->forEachNearbyPoint(
//				origin,
//				maxSearchRadius,
//				[&](size_t j, const Vector2D&) {
//				if (i != j) {
//					_neighborLists[i].push_back(j);
//				}
//			});
//		}
//
//		JET_INFO << "Building neighbor list took: "
//			<< timer.durationInSeconds()
//			<< " seconds";
//	}
//
//	void ParticleSystemData2::serialize(std::vector<uint8_t>* buffer) const {
//		flatbuffers::FlatBufferBuilder builder(1024);
//		flatbuffers::Offset<fbs::ParticleSystemData2> fbsParticleSystemData;
//
//		serializeParticleSystemData(&builder, &fbsParticleSystemData);
//
//		builder.Finish(fbsParticleSystemData);
//
//		uint8_t *buf = builder.GetBufferPointer();
//		size_t size = builder.GetSize();
//
//		buffer->resize(size);
//		memcpy(buffer->data(), buf, size);
//	}
//
//	void ParticleSystemData2::deserialize(const std::vector<uint8_t>& buffer) {
//		auto fbsParticleSystemData = fbs::GetParticleSystemData2(buffer.data());
//		deserializeParticleSystemData(fbsParticleSystemData);
//	}
//
//	void ParticleSystemData2::set(const ParticleSystemData2& other) {
//		m_radius = other.m_radius;
//		m_mass = other.m_mass;
//		m_positionIdx = other.m_positionIdx;
//		m_velocityIdx = other.m_velocityIdx;
//		m_forceIdx = other.m_forceIdx;
//		m_numberOfParticles = other.m_numberOfParticles;
//
//		for (auto& attr : other.m_scalarDataList) {
//			m_scalarDataList.emplace_back(attr);
//		}
//
//		for (auto& attr : other.m_vectorDataList) {
//			m_vectorDataList.emplace_back(attr);
//		}
//
//		m_neighborSearcher = other.m_neighborSearcher->clone();
//		_neighborLists = other._neighborLists;
//	}
//
//	ParticleSystemData2& ParticleSystemData2::operator=(
//		const ParticleSystemData2& other) {
//		set(other);
//		return *this;
//	}
//
//	void ParticleSystemData2::serializeParticleSystemData(
//		flatbuffers::FlatBufferBuilder* builder,
//		flatbuffers::Offset<fbs::ParticleSystemData2>* fbsParticleSystemData)
//		const {
//		// Copy data
//		std::vector<flatbuffers::Offset<fbs::ScalarParticleData2>> scalarDataList;
//		for (const auto& scalarData : m_scalarDataList) {
//			auto fbsScalarData = fbs::CreateScalarParticleData2(
//				*builder,
//				builder->CreateVector(scalarData.data(), scalarData.size()));
//			scalarDataList.push_back(fbsScalarData);
//		}
//		auto fbsScalarDataList = builder->CreateVector(scalarDataList);
//
//		std::vector<flatbuffers::Offset<fbs::VectorParticleData2>> vectorDataList;
//		for (const auto& vectorData : m_vectorDataList) {
//			std::vector<fbs::Vector2D> newVectorData;
//			for (const auto& v : vectorData) {
//				newVectorData.push_back(jetToFbs(v));
//			}
//
//			auto fbsVectorData = fbs::CreateVectorParticleData2(
//				*builder,
//				builder->CreateVectorOfStructs(
//					newVectorData.data(), newVectorData.size()));
//			vectorDataList.push_back(fbsVectorData);
//		}
//		auto fbsVectorDataList = builder->CreateVector(vectorDataList);
//
//		// Copy neighbor searcher
//		auto neighborSearcherType
//			= builder->CreateString(m_neighborSearcher->typeName());
//		std::vector<uint8_t> neighborSearcherSerialized;
//		m_neighborSearcher->serialize(&neighborSearcherSerialized);
//		auto fbsNeighborSearcher = fbs::CreatePointNeighborSearcherSerialized2(
//			*builder,
//			neighborSearcherType,
//			builder->CreateVector(
//				neighborSearcherSerialized.data(),
//				neighborSearcherSerialized.size()));
//
//		// Copy neighbor lists
//		std::vector<flatbuffers::Offset<fbs::ParticleNeighborList2>> neighborLists;
//		for (const auto& neighbors : _neighborLists) {
//			std::vector<uint64_t> neighbors64(neighbors.begin(), neighbors.end());
//			flatbuffers::Offset<fbs::ParticleNeighborList2> fbsNeighborList
//				= fbs::CreateParticleNeighborList2(
//					*builder,
//					builder->CreateVector(neighbors64.data(), neighbors64.size()));
//			neighborLists.push_back(fbsNeighborList);
//		}
//
//		auto fbsNeighborLists = builder->CreateVector(neighborLists);
//
//		// Copy the searcher
//		*fbsParticleSystemData = fbs::CreateParticleSystemData2(
//			*builder,
//			m_radius,
//			m_mass,
//			m_positionIdx,
//			m_velocityIdx,
//			m_forceIdx,
//			fbsScalarDataList,
//			fbsVectorDataList,
//			fbsNeighborSearcher,
//			fbsNeighborLists);
//	}
//
//	void ParticleSystemData2::deserializeParticleSystemData(
//		const fbs::ParticleSystemData2* fbsParticleSystemData) {
//		m_scalarDataList.clear();
//		m_vectorDataList.clear();
//
//		// Copy scalars
//		m_radius = fbsParticleSystemData->Radius();
//		m_mass = fbsParticleSystemData->Mass();
//		m_positionIdx = static_cast<size_t>(fbsParticleSystemData->positionIdx());
//		m_velocityIdx = static_cast<size_t>(fbsParticleSystemData->velocityIdx());
//		m_forceIdx = static_cast<size_t>(fbsParticleSystemData->forceIdx());
//
//		// Copy data
//		auto fbsScalarDataList = fbsParticleSystemData->scalarDataList();
//		for (const auto& fbsScalarData : (*fbsScalarDataList)) {
//			auto data = fbsScalarData->data();
//
//			m_scalarDataList.push_back(ScalarData(data->size()));
//
//			auto& newData = *(m_scalarDataList.rbegin());
//
//			for (uint32_t i = 0; i < data->size(); ++i) {
//				newData[i] = data->Get(i);
//			}
//		}
//
//		auto fbsVectorDataList = fbsParticleSystemData->vectorDataList();
//		for (const auto& fbsVectorData : (*fbsVectorDataList)) {
//			auto data = fbsVectorData->data();
//
//			m_vectorDataList.push_back(VectorData(data->size()));
//			auto& newData = *(m_vectorDataList.rbegin());
//			for (uint32_t i = 0; i < data->size(); ++i) {
//				newData[i] = fbsToJet(*data->Get(i));
//			}
//		}
//
//		m_numberOfParticles = m_vectorDataList[0].size();
//
//		// Copy neighbor searcher
//		auto fbsNeighborSearcher = fbsParticleSystemData->neighborSearcher();
//		m_neighborSearcher
//			= Factory::buildPointNeighborSearcher2(
//				fbsNeighborSearcher->type()->c_str());
//		std::vector<uint8_t> neighborSearcherSerialized(
//			fbsNeighborSearcher->data()->begin(),
//			fbsNeighborSearcher->data()->end());
//		m_neighborSearcher->deserialize(neighborSearcherSerialized);
//
//		// Copy neighbor list
//		auto fbsNeighborLists = fbsParticleSystemData->neighborLists();
//		_neighborLists.resize(fbsNeighborLists->size());
//		for (uint32_t i = 0; i < fbsNeighborLists->size(); ++i) {
//			auto fbsNeighborList = fbsNeighborLists->Get(i);
//			_neighborLists[i].resize(fbsNeighborList->data()->size());
//			std::transform(
//				fbsNeighborList->data()->begin(),
//				fbsNeighborList->data()->end(),
//				_neighborLists[i].begin(),
//				[](uint64_t val) {
//				return static_cast<size_t>(val);
//			});
//		}
//	}
//}