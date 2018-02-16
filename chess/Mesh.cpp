#include "Mesh.h"
#include <glm/detail/type_mat4x4.hpp>
#include <glm/gtc/type_ptr.inl>
#include <iostream>

Vertex::Vertex()
{
	
}

Vertex::Vertex(glm::vec3 pos)
{
	position = pos;
}

Vertex::Vertex(glm::vec3 pos, glm::vec2 texC)
{
	position = pos;
	texCoords = texC;
}

Vertex::Vertex(glm::vec3 pos, glm::vec3 norm, glm::vec2 texC)
{
	position = pos;
	normal = norm;
	texCoords = texC;
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
	this->m_vertices = vertices;
	this->m_indices = indices;
	this->m_textures = textures;

	setupMesh();
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
{
	this->m_vertices = vertices;
	this->m_indices = indices;

	setupMesh();
}

void Mesh::setupMesh()
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int),
		&m_indices[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

	glBindVertexArray(0);
}

void Mesh::drawRange(unsigned begin, unsigned end)
{
	glDrawElements(GL_TRIANGLES, end-begin+1, GL_UNSIGNED_INT, (char*)(begin*sizeof(GL_UNSIGNED_INT)));
}

void Mesh::setSideFacesIndexes(unsigned begin, unsigned end)
{
	m_sideFacesBeginIndex = begin;
	m_sideFacesEndIndex = end;
}

void Mesh::setTopFaceIndexes(unsigned begin, unsigned end)
{
	m_topFaceBeginIndex = begin;
	m_topFaceEndIndex = end;
}

void Mesh::setBottomFaceIndexes(unsigned begin, unsigned end)
{
	m_bottomFaceBeginIndex = begin;
	m_bottomFaceEndIndex = end;
}

void Mesh::draw(Shader shader, unsigned modelLoc, glm::mat4 trans)
{
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(trans));
	// draw mesh
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::drawSideFaces(Shader shader, unsigned modelLoc, glm::mat4 trans)
{
	if(m_sideFacesBeginIndex > -1)
	{
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(trans));
		// draw mesh
		glBindVertexArray(m_VAO);
		drawRange(m_sideFacesBeginIndex, m_sideFacesEndIndex); 
		glBindVertexArray(0);
	}
}

void Mesh::drawTopFace(Shader shader, unsigned modelLoc, glm::mat4 trans)
{
	if(m_topFaceBeginIndex > -1)
	{
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(trans));
		// draw mesh
		glBindVertexArray(m_VAO);
		drawRange(m_topFaceBeginIndex, m_topFaceEndIndex);
		glBindVertexArray(0);
	}
}

void Mesh::drawBottomFace(Shader shader, unsigned modelLoc, glm::mat4 trans)
{
	if(m_bottomFaceBeginIndex > -1)
	{
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(trans));
		// draw mesh
		glBindVertexArray(m_VAO);
		drawRange(m_bottomFaceBeginIndex, m_bottomFaceEndIndex); 
		glBindVertexArray(0);
	}
}