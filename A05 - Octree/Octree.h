#ifndef __OCTREE_H_
#define __OCTREE_H_

#include "MyEntityManager.h";
#include "Simplex/Physics/Octant.h"

namespace Simplex
{
	class Octree
	{
		static uint octantCount;
		static uint maxLevel;
		static uint maxEntityCount;

		uint id = 0;
		uint level = 0;
		uint numChildren = 0;

		float sizeFloat = 0;

		MeshManager* meshManager = nullptr;
		MyEntityManager* entityManager = nullptr;

		vector3 centerVec = vector3(0, 0, 0);
		vector3 minVec = vector3(0, 0, 0);
		vector3 maxVec = vector3(0, 0, 0);

		Octree* parent = nullptr;
		Octree* root = nullptr;

		Octree* children[8];
		std::vector<Octree*> rootChildren;

		std::vector<uint> entityList;

	public:
		Octree(uint maxLevel = 2, uint maxEntityCount = 5);
		Octree(vector3 center, float size);
		Octree(Octree const& other);
		~Octree();

		bool ContainsMoreThan(uint numEntities);
		bool IsColliding(uint index);
		bool IsLeaf();

		float GetSize();

		Octree* GetChild(uint index);
		Octree* GetParent();
		Octree& operator=(Octree const& other);

		uint GetOctantCount();

		void AssignIDtoEntity();

		void ClearEntityList();
		void ConstructTree(uint maxLevel);
		
		void Display(uint index, vector3 color = C_YELLOW);
		void Display(vector3 color = C_YELLOW);
		void DisplayLeaves(vector3 color = C_YELLOW);
		void DisplaySelf(vector3 color = C_YELLOW);

		void KillChildren();
		void Subdivide();

		vector3 GetCenterGlobal();
		vector3 GetMinGlobal();
		vector3 GetMaxGlobal();

	private:
		void ConstructList();
		void Init();
		void Release();
	};
}
#endif