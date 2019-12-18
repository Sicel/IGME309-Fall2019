#pragma once
#include "MyMesh.h"
#include "MyRigidBody.h"
namespace Simplex
{
	class Particle :
		public MyMesh
	{
		vector3 m_v3Velocity = vector3(1);
		vector3 m_v3Momentum = vector3();
		vector3 m_v3Position = vector3();

		float m_fSize = 1; // Radius
		float m_fMass = 1;

	public:
		// Creates Particles of specified size
		Particle(float size);
		Particle(Particle& other);
		Particle& operator=(Particle& other);
		~Particle();

		// Getters
		vector3 GetVelocity();
		vector3 GetMomentum();
		vector3 GetPosition();
		float GetRadius();

		// Bounds of the particle
		float MaxY();
		float MaxZ();
		float MaxX();
		float MinY();
		float MinZ();
		float MinX();

		// Setters
		void SetVelocity(vector3 nVelocity);
		void SetPosition(vector3 nPosition);

		void CalcMomentum();

		// Updates particle position
		void Update(matrix4 projection, matrix4 view);

		// Particle collisions
		void Collision(Particle* other);

		void MirrorX(); // Reverse x velocity
		void MirrorY(); // Reverse y velocity
		void MirrorZ(); // Reverse z velocity
	};
}

