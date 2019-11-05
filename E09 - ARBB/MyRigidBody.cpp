#include "MyRigidBody.h"
using namespace Simplex;
//Allocation
void MyRigidBody::Init(void)
{
	m_pMeshMngr = MeshManager::GetInstance();
	m_bVisibleBS = false;
	m_bVisibleOBB = true;
	m_bVisibleARBB = true;

	m_fRadius = 0.0f;

	m_v3ColorColliding = C_RED;
	m_v3ColorNotColliding = C_WHITE;

	m_v3Center = ZERO_V3;
	m_v3CenterG = ZERO_V3;

	m_v3MinL = ZERO_V3;
	m_v3MaxL = ZERO_V3;

	m_v3MinG = ZERO_V3;
	m_v3MaxG = ZERO_V3;

	m_v3HalfWidth = ZERO_V3;
	m_v3ARBBSize = ZERO_V3;

	m_m4ToWorld = IDENTITY_M4;
}
void MyRigidBody::Swap(MyRigidBody& other)
{
	std::swap(m_pMeshMngr, other.m_pMeshMngr);
	std::swap(m_bVisibleBS, other.m_bVisibleBS);
	std::swap(m_bVisibleOBB, other.m_bVisibleOBB);
	std::swap(m_bVisibleARBB, other.m_bVisibleARBB);

	std::swap(m_fRadius, other.m_fRadius);

	std::swap(m_v3ColorColliding, other.m_v3ColorColliding);
	std::swap(m_v3ColorNotColliding, other.m_v3ColorNotColliding);

	std::swap(m_v3Center, other.m_v3Center);
	std::swap(m_v3MinL, other.m_v3MinL);
	std::swap(m_v3MaxL, other.m_v3MaxL);

	std::swap(m_v3MinG, other.m_v3MinG);
	std::swap(m_v3MaxG, other.m_v3MaxG);

	std::swap(m_v3HalfWidth, other.m_v3HalfWidth);
	std::swap(m_v3ARBBSize, other.m_v3ARBBSize);

	std::swap(m_m4ToWorld, other.m_m4ToWorld);

	std::swap(m_CollidingRBSet, other.m_CollidingRBSet);
}
void MyRigidBody::Release(void)
{
	m_pMeshMngr = nullptr;
	ClearCollidingList();
}
//Accessors
bool MyRigidBody::GetVisibleBS(void) { return m_bVisibleBS; }
void MyRigidBody::SetVisibleBS(bool a_bVisible) { m_bVisibleBS = a_bVisible; }
bool MyRigidBody::GetVisibleOBB(void) { return m_bVisibleOBB; }
void MyRigidBody::SetVisibleOBB(bool a_bVisible) { m_bVisibleOBB = a_bVisible; }
bool MyRigidBody::GetVisibleARBB(void) { return m_bVisibleARBB; }
void MyRigidBody::SetVisibleARBB(bool a_bVisible) { m_bVisibleARBB = a_bVisible; }
float MyRigidBody::GetRadius(void) { return m_fRadius; }
vector3 MyRigidBody::GetColorColliding(void) { return m_v3ColorColliding; }
vector3 MyRigidBody::GetColorNotColliding(void) { return m_v3ColorNotColliding; }
void MyRigidBody::SetColorColliding(vector3 a_v3Color) { m_v3ColorColliding = a_v3Color; }
void MyRigidBody::SetColorNotColliding(vector3 a_v3Color) { m_v3ColorNotColliding = a_v3Color; }
vector3 MyRigidBody::GetCenterLocal(void) { return m_v3Center; }
vector3 MyRigidBody::GetMinLocal(void) { return m_v3MinL; }
vector3 MyRigidBody::GetMaxLocal(void) { return m_v3MaxL; }
vector3 MyRigidBody::GetCenterGlobal(void){	return vector3(m_m4ToWorld * vector4(m_v3Center, 1.0f)); }
vector3 MyRigidBody::GetMinGlobal(void) { return m_v3MinG; }
vector3 MyRigidBody::GetMaxGlobal(void) { return m_v3MaxG; }
vector3 MyRigidBody::GetHalfWidth(void) { return m_v3HalfWidth; }
matrix4 MyRigidBody::GetModelMatrix(void) { return m_m4ToWorld; }

enum BOX_POINT
{
	TLF,
	TRF,
	TLB,
	TRB,
	BLF,
	BRF,
	BLB,
	BRB,
	INVALID
};

void MyRigidBody::SetModelMatrix(matrix4 a_m4ModelMatrix)
{
	//to save some calculations if the model matrix is the same there is nothing to do here
	if (a_m4ModelMatrix == m_m4ToWorld)
		return;

	m_m4ToWorld = a_m4ModelMatrix;
	m_v3CenterG = GetCenterGlobal();
	
	//your code goes here---------------------
	m_v3MinG = vector3(m_m4ToWorld * vector4(m_v3MinL, 1.0f));
	m_v3MaxG = vector3(m_m4ToWorld * vector4(m_v3MaxL, 1.0f));

	vector3 vHVOriented = matrix3(m_m4ToWorld) * m_v3HalfWidth;

	vector3 m_v3MaxLR = vector3(std::numeric_limits<float>().min());
	vector3 m_v3MinLR = vector3(std::numeric_limits<float>().max());

	for (BOX_POINT i = TLF; i < INVALID; i = (BOX_POINT)((int)(i) + 1))
	{
		vector3 ptBox = vector3(0.0f);
		switch (i) {
			case TLF:
				ptBox += vector3(-m_v3HalfWidth.x, m_v3HalfWidth.y, -m_v3HalfWidth.z);
				break;
			case TRF:
				ptBox += vector3(m_v3HalfWidth.x, m_v3HalfWidth.y, -m_v3HalfWidth.z);
				break;
			case TLB:
				ptBox += vector3(-m_v3HalfWidth.x, m_v3HalfWidth.y, m_v3HalfWidth.z);
				break;
			case TRB:
				ptBox += vector3(m_v3HalfWidth.x, m_v3HalfWidth.y, m_v3HalfWidth.z);
				break;
			case BLF:
				ptBox += vector3(-m_v3HalfWidth.x, -m_v3HalfWidth.y, -m_v3HalfWidth.z);
				break;
			case BRF:
				ptBox += vector3(m_v3HalfWidth.x, -m_v3HalfWidth.y, -m_v3HalfWidth.z);
				break;
			case BLB:
				ptBox += vector3(-m_v3HalfWidth.x, -m_v3HalfWidth.y, m_v3HalfWidth.z);
				break;
			case BRB:
				ptBox += vector3(m_v3HalfWidth.x, -m_v3HalfWidth.y, m_v3HalfWidth.z);
				break;
		}

		ptBox = matrix3(m_m4ToWorld) * ptBox;

		if (m_v3MaxLR.x < ptBox.x) m_v3MaxLR.x = ptBox.x;
		else if (m_v3MinLR.x > ptBox.x) m_v3MinLR.x = ptBox.x;

		if (m_v3MaxLR.y < ptBox.y) m_v3MaxLR.y = ptBox.y;
		else if (m_v3MinLR.y > ptBox.y) m_v3MinLR.y = ptBox.y;

		if (m_v3MaxLR.z < ptBox.z) m_v3MaxLR.z = ptBox.z;
		else if (m_v3MinLR.z > ptBox.z) m_v3MinLR.z = ptBox.z;
	}
	//----------------------------------------

	//we calculate the distance between min and max vectors
	//m_v3ARBBSize = m_v3MaxG - m_v3MinG;

	m_v3ARBBSize = (m_v3MaxLR - m_v3MinLR) / 2.0f;
	m_v3CenterR = m_v3CenterG;

	// Update max/min with global center
	m_v3MaxGR = m_v3MaxLR + m_v3CenterG;
	m_v3MinGR = m_v3MinLR + m_v3CenterG;
}
//The big 3
MyRigidBody::MyRigidBody(std::vector<vector3> a_pointList)
{
	Init();
	//Count the points of the incoming list
	uint uVertexCount = a_pointList.size();

	//If there are none just return, we have no information to create the BS from
	if (uVertexCount == 0)
		return;

	//Max and min as the first vector of the list
	m_v3MaxL = m_v3MinL = a_pointList[0];

	//Get the max and min out of the list
	for (uint i = 1; i < uVertexCount; ++i)
	{
		if (m_v3MaxL.x < a_pointList[i].x) m_v3MaxL.x = a_pointList[i].x;
		else if (m_v3MinL.x > a_pointList[i].x) m_v3MinL.x = a_pointList[i].x;
	
		if (m_v3MaxL.y < a_pointList[i].y) m_v3MaxL.y = a_pointList[i].y;
		else if (m_v3MinL.y > a_pointList[i].y) m_v3MinL.y = a_pointList[i].y;
	
		if (m_v3MaxL.z < a_pointList[i].z) m_v3MaxL.z = a_pointList[i].z;
		else if (m_v3MinL.z > a_pointList[i].z) m_v3MinL.z = a_pointList[i].z;
	}

	//with model matrix being the identity, local and global are the same
	m_v3MinG = m_v3MinL;
	m_v3MaxG = m_v3MaxL;

	//we calculate the distance between min and max vectors
	m_v3HalfWidth = (m_v3MaxL - m_v3MinL) / 2.0f;

	//with the max and the min we calculate the center
	//m_v3Center = (m_v3MaxL + m_v3MinL) / 2.0f;
	m_v3Center = m_v3MinL + m_v3HalfWidth;

	//Get the distance between the center and either the min or the max
	//m_fRadius = glm::distance(m_v3Center, m_v3MinL);
	//m_fRadius = 0.0f;
	//m_fRadius2 = 0.0f;
	//for (const vector3& pt : a_pointList)
	//{
	//	float temp;
	//	if ((temp = glm::distance2(pt, m_v3Center)) > m_fRadius2)
	//		m_fRadius2 = temp;
	//}

	//m_fRadius = sqrt(m_fRadius2);
	m_fRadius = glm::length(m_v3HalfWidth);
	m_fRadius2 = m_fRadius * m_fRadius;
}
MyRigidBody::MyRigidBody(MyRigidBody const& other)
{
	m_pMeshMngr = other.m_pMeshMngr;

	m_bVisibleBS = other.m_bVisibleBS;
	m_bVisibleOBB = other.m_bVisibleOBB;
	m_bVisibleARBB = other.m_bVisibleARBB;

	m_fRadius = other.m_fRadius;

	m_v3ColorColliding = other.m_v3ColorColliding;
	m_v3ColorNotColliding = other.m_v3ColorNotColliding;

	m_v3Center = other.m_v3Center;
	m_v3MinL = other.m_v3MinL;
	m_v3MaxL = other.m_v3MaxL;

	m_v3MinG = other.m_v3MinG;
	m_v3MaxG = other.m_v3MaxG;

	m_v3HalfWidth = other.m_v3HalfWidth;
	m_v3ARBBSize = other.m_v3ARBBSize;

	m_m4ToWorld = other.m_m4ToWorld;

	m_CollidingRBSet = other.m_CollidingRBSet;
}
MyRigidBody& MyRigidBody::operator=(MyRigidBody const& other)
{
	if (this != &other)
	{
		Release();
		Init();
		MyRigidBody temp(other);
		Swap(temp);
	}
	return *this;
}
MyRigidBody::~MyRigidBody() { Release(); };

//--- other Methods
void MyRigidBody::AddCollisionWith(MyRigidBody* other)
{
	/*
		check if the object is already in the colliding set, if
		the object is already there return with no changes
	*/
	auto element = m_CollidingRBSet.find(other);
	if (element != m_CollidingRBSet.end())
		return;
	// we couldn't find the object so add it
	m_CollidingRBSet.insert(other);
}
void MyRigidBody::RemoveCollisionWith(MyRigidBody* other)
{
	m_CollidingRBSet.erase(other);
}
void MyRigidBody::ClearCollidingList(void)
{
	m_CollidingRBSet.clear();
}
bool MyRigidBody::IsColliding(MyRigidBody* const other)
{
	//check if spheres are colliding
	bool bColliding = false;
	if (glm::distance(m_v3CenterG, other->m_v3CenterG) < (m_fRadius + other->m_fRadius))
		bColliding = true;

	//bColliding = (glm::distance(GetCenterGlobal(), other->GetCenterGlobal()) < m_fRadius + other->m_fRadius);

	//if they are check the Axis Aligned Bounding Box
	if (bColliding) //they are colliding with bounding sphere
	{
		if (this->m_v3MaxGR.x < other->m_v3MinGR.x) //this to the right of other
			bColliding = false;
		if (this->m_v3MinGR.x > other->m_v3MaxGR.x) //this to the left of other
			bColliding = false;

		if (this->m_v3MaxGR.y < other->m_v3MinGR.y) //this below of other
			bColliding = false;
		if (this->m_v3MinGR.y > other->m_v3MaxGR.y) //this above of other
			bColliding = false;

		if (this->m_v3MaxGR.z < other->m_v3MinGR.z) //this behind of other
			bColliding = false;
		if (this->m_v3MinGR.z > other->m_v3MaxGR.z) //this in front of other
			bColliding = false;

		if (bColliding) //they are colliding with bounding box also
		{
			this->AddCollisionWith(other);
			other->AddCollisionWith(this);
		}
		else //they are not colliding with bounding box
		{
			this->RemoveCollisionWith(other);
			other->RemoveCollisionWith(this);
		}
	}
	else //they are not colliding with bounding sphere
	{
		this->RemoveCollisionWith(other);
		other->RemoveCollisionWith(this);
	}
	return bColliding;
}

void MyRigidBody::AddToRenderList(void)
{
	if (m_bVisibleBS)
	{
		if (m_CollidingRBSet.size() > 0)
			m_pMeshMngr->AddWireSphereToRenderList(glm::translate(m_m4ToWorld, m_v3Center) * glm::scale(vector3(m_fRadius)), C_BLUE_CORNFLOWER);
		else
			m_pMeshMngr->AddWireSphereToRenderList(glm::translate(m_m4ToWorld, m_v3Center) * glm::scale(vector3(m_fRadius)), C_BLUE_CORNFLOWER);
	}
	if (m_bVisibleOBB)
	{
		if (m_CollidingRBSet.size() > 0)
			m_pMeshMngr->AddWireCubeToRenderList(glm::translate(m_m4ToWorld, m_v3Center) * glm::scale(m_v3HalfWidth * 2.0f), m_v3ColorColliding);
		else
			m_pMeshMngr->AddWireCubeToRenderList(glm::translate(m_m4ToWorld, m_v3Center) * glm::scale(m_v3HalfWidth * 2.0f), m_v3ColorNotColliding);
	}
	if (m_bVisibleARBB)
	{
		if (m_CollidingRBSet.size() > 0)
			m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3CenterR) * glm::scale(m_v3ARBBSize * 2.0f), C_YELLOW);
		else
			m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3CenterR) * glm::scale(m_v3ARBBSize * 2.0f), C_YELLOW);
	}
}