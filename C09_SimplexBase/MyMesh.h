#ifndef MY_MESH_H_
#define MY_MESH_H_

#include <vector>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\ext.hpp>
#include <glm\gtc\quaternion.hpp>
#include "Definitions.h"

struct vertex
{
	glm::vec3 position;
	glm::vec3 color;
};

class MyMesh {
public:
	typedef enum class TYPES
	{
		TRIANGLE,
		QUAD,
		CUBE,
		INVALID
	} type;
public:
	MyMesh();
	~MyMesh();

	std::vector<vertex> m_vertices;

	void Init();
	void InitShader();
	void InitTriangle();
	void InitQuad();
	void InitCube();

	void Draw();
	void SetScale(float size);

private:
	// Factory Methods
	static MyMesh* MakeTriangle(float size);
	static MyMesh* MakeQuad(float size);
	static MyMesh* MakeCube(float size);

public:
	static MyMesh* Make(MyMesh::type meshType, float size);

private:
	std::vector<unsigned short> m_indices;

	GLuint m_uShaderProgramID = 0;
	GLuint m_uVAO = 0;
	GLuint m_uVBO = 0;
	GLuint m_uVIO = 0;

	float m_scale = 1.0f;
};

#endif // !MY_MESH_H_
