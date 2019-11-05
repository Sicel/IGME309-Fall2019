#include "MyEntityManager.h"
using namespace Simplex;
//  MyEntityManager
MyEntityManager* MyEntityManager::m_pInstance = nullptr;
void MyEntityManager::Init(void)
{
	m_uEntityCount = 0;
	m_entityList.clear();
}
void MyEntityManager::Release(void)
{
	for (int i = 0; i < m_uEntityCount; i++)
	{
		SafeDelete(m_entityList[i]);
	}
	m_uEntityCount = 0;
	m_entityList.clear();
}
MyEntityManager* MyEntityManager::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new MyEntityManager();
	}
	return m_pInstance;
}
void MyEntityManager::ReleaseInstance()
{
	if (m_pInstance != nullptr)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}
int Simplex::MyEntityManager::GetEntityIndex(String a_sUniqueID)
{
	for (int i = 0; i < m_uEntityCount; i++)
	{
		if (m_entityList[i]->GetUniqueID() == a_sUniqueID)
		{
			return i;
		}
	};

	return -1;
}
//Accessors
Model* Simplex::MyEntityManager::GetModel(uint a_uIndex)
{
	if (a_uIndex >= m_uEntityCount)
		return nullptr;

	return m_entityList[a_uIndex]->GetModel();
}
Model* Simplex::MyEntityManager::GetModel(String a_sUniqueID)
{
	Model* model = nullptr;
	for (int i = 0; i < m_uEntityCount; i++)
	{
		if (m_entityList[i]->GetUniqueID() == a_sUniqueID)
		{
			model = m_entityList[i]->GetModel();
		}
	};

	return model;
}
RigidBody* Simplex::MyEntityManager::GetRigidBody(uint a_uIndex)
{
	if (a_uIndex >= m_uEntityCount)
		return nullptr;

	return m_entityList[a_uIndex]->GetRigidBody();
}
RigidBody* Simplex::MyEntityManager::GetRigidBody(String a_sUniqueID)
{
	RigidBody* model = nullptr;
	for (int i = 0; i < m_uEntityCount; i++)
	{
		if (m_entityList[i]->GetUniqueID() == a_sUniqueID)
		{
			model = m_entityList[i]->GetRigidBody();
		}
	};

	return model;
}
matrix4 Simplex::MyEntityManager::GetModelMatrix(uint a_uIndex)
{
	if (a_uIndex >= m_uEntityCount)
		return IDENTITY_M4;

	return m_entityList[a_uIndex]->GetModelMatrix();
}
matrix4 Simplex::MyEntityManager::GetModelMatrix(String a_sUniqueID)
{
	for (int i = 0; i < m_uEntityCount; i++)
	{
		if (m_entityList[i]->GetUniqueID() == a_sUniqueID)
		{
			return m_entityList[i]->GetModelMatrix();
		}
	};

	return IDENTITY_M4;
}
void Simplex::MyEntityManager::SetModelMatrix(matrix4 a_m4ToWorld, String a_sUniqueID)
{
	for (int i = 0; i < m_uEntityCount; i++)
	{
		if (m_entityList[i]->GetUniqueID() == a_sUniqueID)
		{
			m_entityList[i]->SetModelMatrix(a_m4ToWorld);
			return;
		}
	};
}
void Simplex::MyEntityManager::SetModelMatrix(matrix4 a_m4ToWorld, uint a_uIndex)
{
	if (a_uIndex >= m_uEntityCount)
		return;

	m_entityList[a_uIndex]->SetModelMatrix(a_m4ToWorld);
}
//The big 3
MyEntityManager::MyEntityManager(){Init();}
MyEntityManager::MyEntityManager(MyEntityManager const& other){ }
MyEntityManager& MyEntityManager::operator=(MyEntityManager const& other) { return *this; }
MyEntityManager::~MyEntityManager(){Release();};
// other methods
void Simplex::MyEntityManager::Update(void)
{
	for (int i = 0; i < m_uEntityCount - 1; i++)
	{
		for (int j = i + 1; j < m_uEntityCount; j++)
		{
			m_entityList[i]->IsColliding(m_entityList[j]);
		}
	}
}
void Simplex::MyEntityManager::AddEntity(String a_sFileName, String a_sUniqueID)
{
	m_entityList.push_back(new MyEntity(a_sFileName, a_sUniqueID));
	m_uEntityCount++;
}
void Simplex::MyEntityManager::RemoveEntity(uint a_uIndex)
{
	if (a_uIndex >= m_uEntityCount)
		return;

	m_entityList.erase(m_entityList.begin() + a_uIndex);
	m_uEntityCount--;
}
void Simplex::MyEntityManager::RemoveEntity(String a_sUniqueID)
{
	for (int i = 0; i < m_entityList.size(); i++)
	{
		MyEntity* entity = m_entityList[i];
		if (entity->GetUniqueID() == a_sUniqueID)
		{
			m_entityList.erase(m_entityList.begin() + i);
			m_uEntityCount--;
			return;
		}
	}
}
String Simplex::MyEntityManager::GetUniqueID(uint a_uIndex)
{
	if (a_uIndex >= m_uEntityCount)
		return "Index out of range";

	return m_entityList[a_uIndex]->GetUniqueID();
}
MyEntity* Simplex::MyEntityManager::GetEntity(uint a_uIndex)
{
	if (a_uIndex >= m_uEntityCount)
		return nullptr;

	return m_entityList[a_uIndex];
}
void Simplex::MyEntityManager::AddEntityToRenderList(uint a_uIndex, bool a_bRigidBody)
{
	if (a_uIndex >= m_uEntityCount)
	{
		for (int i = 0; i < m_uEntityCount; i++)
			m_entityList[i]->AddToRenderList(a_bRigidBody);
	}
	else
	{
		m_entityList[a_uIndex]->AddToRenderList(a_bRigidBody);
	}
}
void Simplex::MyEntityManager::AddEntityToRenderList(String a_sUniqueID, bool a_bRigidBody)
{
	for (int i = 0; i < m_entityList.size(); i++)
	{
		MyEntity* entity = m_entityList[i];
		if (entity->GetUniqueID() == a_sUniqueID)
		{
			m_entityList[i]->AddToRenderList(a_bRigidBody);
			return;
		}
	}
}