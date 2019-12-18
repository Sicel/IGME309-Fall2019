#include "AppClass.h"
using namespace Simplex;
void Application::InitVariables(void)
{
	//Set the position and target of the camera
	m_pCameraMngr->SetPositionTargetAndUpward(	vector3(0.0f, 3.0f, 13.0f), //Position
											vector3(0.0f, 3.0f, 12.0f),	//Target
											AXIS_Y );					//Up

	//Set the position of the light
	m_pLightMngr->SetPosition(vector3(10.0f));

	m_v3BoxBounds = vector3(20, 20, 20); // Bounds of box

	// Spawns particles at random location with different velocities
	for (int i = 0; i < 5; i++)
	{
		std::srand(i);
		m_vParticles.push_back(new Particle(0.5f));
		float randX = std::rand() / (RAND_MAX / (0.5f * m_v3BoxBounds.x));
		float randY = std::rand() / (RAND_MAX / (0.5f * m_v3BoxBounds.y));
		float randZ = std::rand() / (-RAND_MAX / (0.5f * m_v3BoxBounds.z));
		m_vParticles[i]->SetPosition(vector3(randX, randY, randZ));
		m_vParticles[i]->SetVelocity(vector3(randX / 10, -randY / 10, randZ / 10));
	}
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();

	// wall bouncing
	for (int i = 0; i < m_vParticles.size(); i++)
	{
		if (m_vParticles[i]->MaxX() >= m_v3BoxBounds.x / 2 || m_vParticles[i]->MinX() <= -m_v3BoxBounds.x / 2)
			m_vParticles[i]->MirrorX();
	
		if (m_vParticles[i]->MaxY() >= m_v3BoxBounds.y / 2 || m_vParticles[i]->MinY() <= -m_v3BoxBounds.y / 2)
			m_vParticles[i]->MirrorY();
	
		if (m_vParticles[i]->MaxZ() >= m_v3BoxBounds.z / 2 || m_vParticles[i]->MinZ() <= -m_v3BoxBounds.z / 2)
			m_vParticles[i]->MirrorZ();
	}

	// particle collision
	for (int i = 0; i < m_vParticles.size(); i++)
	{
		for (int j = i + 1; j < m_vParticles.size(); j++)
		{
			m_vParticles[i]->Collision(m_vParticles[j]);
		}
	}
}
void Application::Display(void)
{
	//Clear the screen
	ClearScreen();

	//Add grid to the scene
	//m_pMeshMngr->AddGridToRenderList();

	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();
	matrix4 m4View = m_pCameraMngr->GetViewMatrix();

	// renders particles
	for (int i = 0; i < m_vParticles.size(); i++)
	{
		m_vParticles[i]->Update(m4Projection, m4View);
	}

	//Add skybox
	m_pMeshMngr->AddSkyboxToRenderList();

	m_pMeshMngr->AddWireCubeToRenderList(glm::scale(m_v3BoxBounds), C_RED);

	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();
		
	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	//release the model
	SafeDelete(m_pCreeper);

	//release the rigid body for the model
	SafeDelete(m_pCreeperRB);

	//release the model
	SafeDelete(m_pSteve);

	//release the rigid body for the model
	SafeDelete(m_pSteveRB);
	
	//release GUI
	ShutdownGUI();
}
