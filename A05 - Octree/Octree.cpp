#include "Octree.h"
using namespace Simplex;

uint Octree::octantCount = 0;
uint Octree::maxLevel = 3;
uint Octree::maxEntityCount = 5;

Octree::Octree(uint maxLevel, uint maxEntityCount)
{
	Init();

	octantCount = 0;
	Octree::maxLevel = maxLevel;
	Octree::maxEntityCount = maxEntityCount;
	id = octantCount;

	root = this;
	rootChildren.clear();

	std::vector<vector3> minMaxVectors;

	uint numObjects = entityManager->GetEntityCount();
	for (uint i = 0; i < numObjects; i++)
	{
		MyEntity* entity = entityManager->GetEntity(i);
		MyRigidBody* rigidBody = entity->GetRigidBody();
		minMaxVectors.push_back(rigidBody->GetMinGlobal());
		minMaxVectors.push_back(rigidBody->GetMaxGlobal());
	}
	MyRigidBody* rigidBody = new MyRigidBody(minMaxVectors);

	vector3 halfWidth = rigidBody->GetHalfWidth();
	float maxFloat = halfWidth.x;
	for (uint i = 1; i < 3; i++)
	{
		if (maxFloat < halfWidth[i])
			maxFloat = halfWidth[i];
	}
	vector3 center = rigidBody->GetCenterLocal();
	minMaxVectors.clear();
	SafeDelete(rigidBody);

	sizeFloat = maxFloat * 2.0f;
	centerVec = center;
	minVec = centerVec - (vector3(maxFloat));
	maxVec = centerVec + (vector3(maxFloat));

	octantCount++;

	ConstructTree(maxLevel);
}

Octree::Octree(vector3 center, float size)
{
	Init();
	centerVec = center;
	sizeFloat = size;

	minVec = centerVec - (vector3(size) / 2.0f);
	maxVec = centerVec + (vector3(size) / 2.0f);

	octantCount++;
}

Octree::Octree(Octree const& other)
{
	numChildren = other.numChildren;

	centerVec = other.centerVec;
	minVec = other.minVec;
	maxVec = other.maxVec;

	sizeFloat = other.sizeFloat;

	id = other.id;
	level = other.level;

	parent = other.parent;
	root = other.root;
	rootChildren = other.rootChildren;

	meshManager = MeshManager::GetInstance();
	entityManager = MyEntityManager::GetInstance();

	for (uint i = 0; i < 8; i++)
	{
		children[i] = other.children[i];
	}
	//octantCount++;
}

Octree::~Octree()
{
	Release();
}

bool Octree::ContainsMoreThan(uint numEntities)
{
	uint count = 0;
	uint numObjects = entityManager->GetEntityCount();
	for (uint c = 0; c < numObjects; c++)
	{
		if (IsColliding(c))
			count++;
		if (count > numEntities)
			return true;
	}
	return false;
}

bool Octree::IsColliding(uint index)
{
	uint numObjects = entityManager->GetEntityCount();

	if (index >= numObjects)
		return false;

	MyEntity* entity = entityManager->GetEntity(index);
	MyRigidBody* rigidBody = entity->GetRigidBody();
	vector3 minG = rigidBody->GetMinGlobal();
	vector3 maxG = rigidBody->GetMaxGlobal();

	if (maxVec.x < minG.x)
		return false;
	if (minVec.x > maxG.x)
		return false;

	if (maxVec.y < minG.y)
		return false;
	if (minVec.y > maxG.y)
		return false;

	if (maxVec.z < minG.z)
		return false;
	if (minVec.z > maxG.z)
		return false;

	return true;
}

bool Octree::IsLeaf()
{
	return (numChildren == 0);
}

float Octree::GetSize()
{
	return 0.0f;
}

Octree* Octree::GetChild(uint index)
{
	return index >= numChildren ? nullptr : children[index];
}

Octree* Octree::GetParent()
{
	return parent;
}

Octree& Octree::operator=(Octree const& other)
{
	Octree tree = *this;
	if (this != &other)
	{
		Release();
		Init();
		tree = Octree(other);
	}
	return tree;
}

uint Octree::GetOctantCount()
{
	return octantCount;
}

void Octree::AssignIDtoEntity()
{
	for (uint i = 0; i < numChildren; i++)
	{
		children[i]->AssignIDtoEntity();
	}

	if (numChildren == 0)
	{
		uint numEntities = entityManager->GetEntityCount();
		for (uint i = 0; i < numEntities; i++)
		{
			entityList.push_back(i);
			entityManager->AddDimension(i, id);
		}
	}
}

void Octree::ClearEntityList()
{
	for (uint i = 0; i < numChildren; i++)
	{
		children[i]->ClearEntityList();
	}
	entityList.clear();
}

void Octree::ConstructTree(uint maxLevel)
{
	if (level != 0)
		return;

	Octree::maxLevel = maxLevel;

	octantCount = 1;

	entityList.clear();

	KillChildren();
	rootChildren.clear();

	if (ContainsMoreThan(maxEntityCount))
		Subdivide();

	AssignIDtoEntity();

	ConstructList();
}

void Octree::Display(uint index, vector3 color)
{
	if (index > numChildren)
	{
		Display(color);
		return;
	}
	
	if (id == index)
	{
		DisplaySelf(color);
		return;
	}

	for (uint i = 0; i < numChildren; i++)
	{
		children[i]->Display(index);
	}
}

void Octree::Display(vector3 color)
{
	for (uint i = 0; i < numChildren; i++)
	{
		children[i]->Display(color);
	}
	DisplaySelf(color);
}

void Octree::DisplaySelf(vector3 color)
{
	meshManager->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, centerVec) *
		glm::scale(vector3(sizeFloat)), color, RENDER_WIRE);
}

void Octree::DisplayLeaves(vector3 color)
{
	uint numLeaves = rootChildren.size();
	for (uint i = 0; i < numLeaves; i++)
	{
		rootChildren[i]->DisplayLeaves(color);
	}
	DisplaySelf(color);
}

void Octree::KillChildren()
{
	for (uint i = 0; i < numChildren; i++)
	{
		children[i]->KillChildren();
		delete children[i];
		children[i] = nullptr;
	}
	numChildren = 0;
}

void Octree::Subdivide()
{
	if (level >= maxLevel)
		return;

	if (numChildren != 0)
		return;

	numChildren = 8;

	float size = sizeFloat / 4.0f;
	float sizeDoubled = size * 2.0f;
	vector3 center;

	center = centerVec;
	center.x -= size;
	center.y -= size;
	center.z -= size;

	// 0
	children[0] = new Octree(center, sizeDoubled);

	// 1
	center.x += sizeDoubled;
	children[1] = new Octree(center, sizeDoubled);

	// 2
	center.z += sizeDoubled;
	children[2] = new Octree(center, sizeDoubled);

	// 3
	center.x -= sizeDoubled;
	children[3] = new Octree(center, sizeDoubled);

	// 4
	center.y += sizeDoubled;
	children[4] = new Octree(center, sizeDoubled);

	// 5
	center.z -= sizeDoubled;
	children[5] = new Octree(center, sizeDoubled);

	// 6
	center.x += sizeDoubled;
	children[6] = new Octree(center, sizeDoubled);

	// 7
	center.z += sizeDoubled;
	children[7] = new Octree(center, sizeDoubled);

	for (uint i = 0; i < 8; i++)
	{
		children[i]->parent = this;
		children[i]->root = root;
		children[i]->level = level + 1;
		if (children[i]->ContainsMoreThan(maxEntityCount))
			children[i]->Subdivide();
	}
}

vector3 Simplex::Octree::GetCenterGlobal()
{
	return centerVec;
}

vector3 Simplex::Octree::GetMinGlobal()
{
	return minVec;
}

vector3 Simplex::Octree::GetMaxGlobal()
{
	return maxVec;
}

void Octree::ConstructList()
{
	for (uint i = 0; i < numChildren; i++)
	{
		children[i]->ConstructList();
	}

	if (entityList.size() > 0)
		root->rootChildren.push_back(this);
}

void Octree::Init()
{
	numChildren = 0;
	sizeFloat = 0;
	id = octantCount;
	level = 0;

	meshManager = MeshManager::GetInstance();
	entityManager = MyEntityManager::GetInstance();

	root = nullptr;
	parent = nullptr;

	for (uint i = 0; i < 8; i++)
	{
		children[i] = nullptr;
	}

	entityList = std::vector<uint>();

	centerVec = vector3(0, 0, 0);
	minVec = vector3(0, 0, 0);
	maxVec = vector3(0, 0, 0);
}

void Octree::Release()
{
	if (level == 0)
		KillChildren();

	numChildren = 0;
	sizeFloat = 0;
	entityList.clear();
	rootChildren.clear();
}
