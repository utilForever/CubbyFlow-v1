/*************************************************************************
> File Name: ParticleEmitter3.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Abstract base class for 3-D particle emitter.
> Created Time: 2017/05/20
> Copyright (c) 2017, Chan-Ho Chris Ohk
*************************************************************************/
#include <Particle/ParticleEmitter3.h>

namespace CubbyFlow
{
	ParticleEmitter3::ParticleEmitter3()
	{
		// Do nothing
	}

	ParticleEmitter3::~ParticleEmitter3()
	{
		// Do nothing
	}

	const ParticleSystemData3Ptr& ParticleEmitter3::Target() const
	{
		return m_particles;
	}

	void ParticleEmitter3::SetTarget(const ParticleSystemData3Ptr& particles)
	{
		m_particles = particles;

		OnSetTarget(particles);
	}

	void ParticleEmitter3::Update(double currentTimeInSeconds, double timeIntervalInSeconds)
	{
		if (m_onBeginUpdateCallback)
		{
			m_onBeginUpdateCallback(this, currentTimeInSeconds, timeIntervalInSeconds);
		}

		OnUpdate(currentTimeInSeconds, timeIntervalInSeconds);
	}

	// TODO: particles is unused variable.
	void ParticleEmitter3::OnSetTarget(const ParticleSystemData3Ptr& particles)
	{
		// Do nothing
	}

	void ParticleEmitter3::SetOnBeginUpdateCallback(const OnBeginUpdateCallback& callback)
	{
		m_onBeginUpdateCallback = callback;
	}
}