#include "MyMesh.h"

MyMesh::MyMesh()
{

}

MyMesh::~MyMesh()
{
	glDeleteShader(m_uShaderProgramID);
	glDeleteBuffers(1, &m_uVBO);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &m_uVAO);

}


void MyMesh::Init()
{
	glGenVertexArrays(1, &m_uVAO);
	glGenBuffers(1, &m_uVBO);

	glBindVertexArray(m_uVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_uVBO);

	glGenBuffers(1, &m_uVIO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uVIO);

	// Set up shader
	InitShader();

	glBindVertexArray(0);
}

void MyMesh::InitShader()
{
	m_uShaderProgramID = LoadShaders("Shaders//BasicColor.vs", "Shaders//BasicColor.fs");
	glUseProgram(m_uShaderProgramID);
}

void MyMesh::InitTriangle()
{
	glBindVertexArray(m_uVAO);
	// Consists of three vertices

	float trisize = 0.5f;

	// Vetex 1 (top vertex)
	vertex vertexTopPoint;
	vertexTopPoint.position = glm::vec3(-trisize, -trisize, 0.0f);
	vertexTopPoint.color = glm::vec3(1.0f, 0.0f, 0.0f);
	m_vertices.push_back(vertexTopPoint);

	// Vetex 2 (left bottom vertex)
	vertex vertexLeftBottom;
	vertexLeftBottom.position = glm::vec3(trisize, -trisize, 0.0f);
	vertexLeftBottom.color = glm::vec3(0.0f, 1.0f, 0.0f);
	m_vertices.push_back(vertexLeftBottom);

	// Vetex 3 (right bottom vertex)
	vertex vertexRightBottom;
	vertexRightBottom.position = glm::vec3(0, trisize, 0.0f);
	vertexRightBottom.color = glm::vec3(0.0f, 0.0f, 1.0f);
	m_vertices.push_back(vertexRightBottom);

	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(vertex), &m_vertices[0], GL_STATIC_DRAW);

	// Fill Index buffer
	m_indices.push_back((unsigned short)0);
	m_indices.push_back((unsigned short)1);
	m_indices.push_back((unsigned short)2);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(glm::u16vec2), &m_indices[0], GL_STATIC_DRAW);

	// Count the attributes
	int attribCount = 2;

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)0);
	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)(sizeof(glm::vec3)));

	glBindVertexArray(0);
}

void MyMesh::InitCube()
{
	glBindVertexArray(m_uVAO);
	// Consists of three vertices

	float size = 0.5f;
	unsigned short vertCount = 0;

	// Vetex 1 (Top Left Front)
	vertex vertexTopLeftFront;
	vertexTopLeftFront.position = glm::vec3(-size, size, size);
	vertexTopLeftFront.color = glm::vec3(1.0f, 0.0f, 0.0f);
	m_vertices.push_back(vertexTopLeftFront);
	unsigned short vTLF = vertCount++;

	// Vetex 2 (Top Right Front)
	vertex vertexTopRightFront;
	vertexTopRightFront.position = glm::vec3(size, size, size);
	vertexTopRightFront.color = glm::vec3(0.0f, 1.0f, 0.0f);
	m_vertices.push_back(vertexTopRightFront);
	unsigned short vTRF = vertCount++;

	// Vetex 3 (Bottom Left Front)
	vertex vertexBottomLeftFront;
	vertexBottomLeftFront.position = glm::vec3(-size, -size, size);
	vertexBottomLeftFront.color = glm::vec3(0.0f, 0.0f, 1.0f);
	m_vertices.push_back(vertexBottomLeftFront);
	unsigned short vBLF = vertCount++;

	// Vetex 4 (Bottom Right Front)
	vertex vertexBottomRightFront;
	vertexBottomRightFront.position = glm::vec3(size, -size, size);
	vertexBottomRightFront.color = glm::vec3(0.0f, 0.0f, 1.0f);
	m_vertices.push_back(vertexBottomRightFront);
	unsigned short vBRF = vertCount++;

	// Vetex 5 (Top Left Back)
	vertex vertexTopLeftBack;
	vertexTopLeftBack.position = glm::vec3(-size, size, -size);
	vertexTopLeftBack.color = glm::vec3(1.0f, 0.0f, 0.0f);
	m_vertices.push_back(vertexTopLeftBack);
	unsigned short vTLB = vertCount++;

	// Vetex 6 (Top Right Back)
	vertex vertexTopRightBack;
	vertexTopRightBack.position = glm::vec3(size, size, -size);
	vertexTopRightBack.color = glm::vec3(0.0f, 1.0f, 0.0f);
	m_vertices.push_back(vertexTopRightBack);
	unsigned short vTRB = vertCount++;

	// Vetex 7 (Bottom Left Back)
	vertex vertexBottomLeftBack;
	vertexBottomLeftBack.position = glm::vec3(-size, -size, -size);
	vertexBottomLeftBack.color = glm::vec3(0.0f, 0.0f, 1.0f);
	m_vertices.push_back(vertexBottomLeftBack);
	unsigned short vBLB = vertCount++;

	// Vetex 8 (Bottom Right Back)
	vertex vertexBottomRightBack;
	vertexBottomRightBack.position = glm::vec3(size, -size, -size);
	vertexBottomRightBack.color = glm::vec3(0.0f, 0.0f, 1.0f);
	m_vertices.push_back(vertexBottomRightBack);
	unsigned short vBRB = vertCount++;

	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(vertex), &m_vertices[0], GL_STATIC_DRAW);

	// Winding order - draws points counter-clockwise so that they don't face away
	// Front face
	m_indices.push_back(vTRF);
	m_indices.push_back(vTLF);
	m_indices.push_back(vBLF);

	m_indices.push_back(vBRF);
	m_indices.push_back(vTRF);
	m_indices.push_back(vBLF);

	// Back face
	m_indices.push_back(vTLB);
	m_indices.push_back(vTRB);
	m_indices.push_back(vBLB);

	m_indices.push_back(vTRB);
	m_indices.push_back(vBRB);
	m_indices.push_back(vBLB);

	
	// Left face
	m_indices.push_back(vTLF);
	m_indices.push_back(vTLB);
	m_indices.push_back(vBLB);

	m_indices.push_back(vTLF);
	m_indices.push_back(vBLB);
	m_indices.push_back(vBLF);

	// Right face
	m_indices.push_back(vTRB);
	m_indices.push_back(vTRF);
	m_indices.push_back(vBRB);

	m_indices.push_back(vBRB);
	m_indices.push_back(vTRF);
	m_indices.push_back(vBRF);

	// Top face
	m_indices.push_back(vTRB);
	m_indices.push_back(vTLB);
	m_indices.push_back(vTLF);

	m_indices.push_back(vTRB);
	m_indices.push_back(vTLF);
	m_indices.push_back(vTRF);
	
	// Bottom face
	m_indices.push_back(vBLB);
	m_indices.push_back(vBRB);
	m_indices.push_back(vBLF);

	m_indices.push_back(vBLF);
	m_indices.push_back(vBRB);
	m_indices.push_back(vBRF);
	

	// Fill Index buffer

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(glm::u16vec2), &m_indices[0], GL_STATIC_DRAW);

	// Count the attributes
	int attribCount = 2;

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)(sizeof(glm::vec3)));

	glBindVertexArray(0);
}

void MyMesh::InitQuad()
{

	glBindVertexArray(m_uVAO);
	// Consists of three vertices

	float size = 0.25f;

	// Vetex 1 (Top Left)
	vertex vertexTopLeft;
	vertexTopLeft.position = glm::vec3(-size, size, 0.0f);
	vertexTopLeft.color = glm::vec3(1.0f, 0.0f, 0.0f);
	m_vertices.push_back(vertexTopLeft);

	// Vetex 2 (Top Right)
	vertex vertexTopRight;
	vertexTopRight.position = glm::vec3(size, size, 0.0f);
	vertexTopRight.color = glm::vec3(0.0f, 1.0f, 0.0f);
	m_vertices.push_back(vertexTopRight);

	// Vetex 3 (Bottom Left)
	vertex vertexBottomLeft;
	vertexBottomLeft.position = glm::vec3(-size, -size, 0.0f);
	vertexBottomLeft.color = glm::vec3(0.0f, 0.0f, 1.0f);
	m_vertices.push_back(vertexBottomLeft);

	// Vetex 4 (Bottom Right)
	vertex vertexBottomRight;
	vertexBottomRight.position = glm::vec3(size, -size, 0.0f);
	vertexBottomRight.color = glm::vec3(0.0f, 0.0f, 1.0f);
	m_vertices.push_back(vertexBottomRight);

	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(vertex), &m_vertices[0], GL_STATIC_DRAW);

	// Fill Index buffer
	m_indices.push_back((unsigned short)1);
	m_indices.push_back((unsigned short)0);
	m_indices.push_back((unsigned short)2);

	m_indices.push_back((unsigned short)3);
	m_indices.push_back((unsigned short)1);
	m_indices.push_back((unsigned short)2);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(glm::u16vec2), &m_indices[0], GL_STATIC_DRAW);

	// Count the attributes
	int attribCount = 2;

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)(sizeof(glm::vec3)));

	glBindVertexArray(0);
}

void MyMesh::Draw()
{
	glBindVertexArray(m_uVAO);

	glm::vec3 ptOrigin = glm::vec3(0.0f, 0.0f, 0.0f);

	ptOrigin += glm::vec3(1.0f, 0.0f, 0.0f);

	// Used to get window size and create a projection matrix based on that
	auto pCameraMngr = CameraManager::GetInstance();

	glm::mat4 m4Projection = pCameraMngr->GetProjectionMatrix();						// projection matrix
	glm::mat4 m4View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f));	// view matrix
	//glm::mat4 m4Model = glm::mat4(1.0f);												// model matrix

	glm::mat4 m4Model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f));	// model matrix

	static float theta = 0.0f;
	theta += 0.01f;

	//m4Model = glm::rotate(m4Model, theta, glm::vec3(0.0f, 1.0f, 0.0f));
	//m4Model = glm::rotate(m4Model, theta, glm::vec3(1.0f, 0.0f, 0.0f));

	GLuint MVP = glGetUniformLocation(m_uShaderProgramID, "transform");
	glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(m4Projection * m4View * m4Model));

	// Draw
	// glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
	if (m_indices.size() > 0)
		glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_SHORT, (void*)0);
	else
		glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());


	glBindVertexArray(0);
}

void MyMesh::SetScale(float size)
{
	m_scale = size;
}

MyMesh* MyMesh::MakeTriangle(float size)
{
	MyMesh* pMesh = nullptr;

	pMesh = new MyMesh;
	pMesh->Init();

	// Set up triangle here
	pMesh->InitTriangle();

	pMesh->SetScale(size);

	return pMesh;
}

MyMesh* MyMesh::MakeQuad(float size)
{
	MyMesh* pMesh = nullptr;

	pMesh = new MyMesh;
	pMesh->Init();

	// Set up triangle here
	pMesh->InitQuad();

	pMesh->SetScale(size);

	return pMesh;
}

MyMesh* MyMesh::MakeCube(float size)
{
	MyMesh* pMesh = nullptr;

	pMesh = new MyMesh;
	pMesh->Init();

	// Set up triangle here
	pMesh->InitCube();

	pMesh->SetScale(size);

	return pMesh;
}

MyMesh* MyMesh::Make(MyMesh::type meshType, float size)
{
	MyMesh* pMesh = nullptr;

	switch (meshType)
	{
	case MyMesh::type::TRIANGLE:
		pMesh = MyMesh::MakeTriangle(size);
		break;
	case MyMesh::type::QUAD:
		pMesh = MyMesh::MakeQuad(size);
		break;
	case MyMesh::type::CUBE:
		pMesh = MyMesh::MakeCube(size);
		break;
	case MyMesh::type::INVALID:
	default:
		pMesh = nullptr;
	}

	return pMesh;
}