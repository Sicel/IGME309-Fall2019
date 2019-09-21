#include "AppClass.h"

#include "MyMesh.h"
void Application::InitVariables(void)
{
	//Change this to your name and email
	m_sProgrammer = "Cristian Cordoba - cxc9597@rit.edu";

	//m_pTriangle = MyMesh::Make(MyMesh::type::TRIANGLE, 0.5f);
	//m_pQuad = MyMesh::Make(MyMesh::type::QUAD, 0.5f);
	m_pCube = MyMesh::Make(MyMesh::type::CUBE, 0.5f);

	srand(time(NULL));
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();
	
	if(m_pTriangle != nullptr)
		m_pTriangle->Draw();

	if(m_pQuad != nullptr)
		m_pQuad->Draw();

	if(m_pCube != nullptr)
		m_pCube->Draw();

	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();
	
	//render list call
	//m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();
	
	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	//release GUI
	ShutdownGUI();
}