#include "Particle.h"
using namespace Simplex;

Particle::Particle(float size) : MyMesh() { 
	m_fSize = size;
	this->GenerateSphere(m_fSize, 5, C_BLUE_CORNFLOWER);
}

Particle::Particle(Particle& other) : MyMesh(other) { }

Particle& Simplex::Particle::operator=(Particle& other)
{
	if (this != &other)
	{
		Release();
		Init();
		Particle temp(other);
		Swap(temp);
	}
	return *this;
}

Particle::~Particle()
{
	Release();
}

vector3 Simplex::Particle::GetVelocity()
{
	return m_v3Velocity;
}

vector3 Simplex::Particle::GetMomentum()
{
	return m_v3Momentum;
}

vector3 Simplex::Particle::GetPosition()
{
	return m_v3Position;
}

float Particle::GetRadius() { return m_fSize; }
float Particle::MaxY() { return m_v3Position.y + m_fSize; }
float Particle::MaxZ() { return m_v3Position.z + m_fSize; }
float Particle::MaxX() { return m_v3Position.x + m_fSize; }
float Particle::MinY() { return m_v3Position.y - m_fSize; }
float Particle::MinZ() { return m_v3Position.z - m_fSize; }
float Particle::MinX() { return m_v3Position.x - m_fSize; }

void Simplex::Particle::SetVelocity(vector3 nVelocity) { m_v3Velocity = nVelocity; }

void Simplex::Particle::SetPosition(vector3 nPosition) { m_v3Position = nPosition; }

void Simplex::Particle::CalcMomentum()
{
	m_v3Momentum = m_fMass * m_v3Velocity;
}

void Particle::Update(matrix4 projection, matrix4 view)
{
	m_v3Position += m_v3Velocity;
	matrix4 model = glm::translate(m_v3Position);
	this->Render(projection, view, model);
}

void Particle::Collision(Particle* other)
{
	// Avoids checking self
	if (this == other)
		return;

	// distance between two particles
	float distance = glm::distance(GetPosition(), other->GetPosition());

	// Avoids going any further if they're not close enough
	if (distance > GetRadius() + other->GetRadius())
		return;

	// Swaps velocities
	vector3 otherVel = other->GetVelocity();
	other->m_v3Velocity = m_v3Velocity;
	m_v3Velocity = otherVel;
}

void Particle::MirrorX() { m_v3Velocity = vector3(-m_v3Velocity.x, m_v3Velocity.y, m_v3Velocity.z); }
void Particle::MirrorY() { m_v3Velocity = vector3(m_v3Velocity.x, -m_v3Velocity.y, m_v3Velocity.z); }
void Particle::MirrorZ() { m_v3Velocity = vector3(m_v3Velocity.x, m_v3Velocity.y, -m_v3Velocity.z); }
