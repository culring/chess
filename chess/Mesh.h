#pragma once
#include <glm/detail/type_vec3.hpp>
#include <glm/detail/type_vec2.hpp>
#include <string>
#include <vector>
#include "Shader.h"
#include <glm/mat4x2.hpp>

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;

	Vertex();
	Vertex(glm::vec3 pos);
	Vertex(glm::vec3 pos, glm::vec2 texC);
	Vertex(glm::vec3 pos, glm::vec3 norm, glm::vec2 texC);
};

struct Texture {
	unsigned int id;
	std::string type;
};

class Mesh{
public:
	/*  Functions  */
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

	void setSideFacesIndexes(unsigned int begin, unsigned int end);
	void setTopFaceIndexes(unsigned int begin, unsigned int end);
	void setBottomFaceIndexes(unsigned int begin, unsigned int end);

	void draw(Shader shader, unsigned int modelLoc, glm::mat4 trans);
	void drawSideFaces(Shader shader, unsigned int modelLoc, glm::mat4 trans);
	void drawTopFace(Shader shader, unsigned int modelLoc, glm::mat4 trans);
	void drawBottomFace(Shader shader, unsigned int modelLoc, glm::mat4 trans);

private:
	/*  Mesh Data  */
	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;
	int m_sideFacesBeginIndex = 1,
		m_sideFacesEndIndex = -1,
		m_topFaceBeginIndex = -1,
		m_topFaceEndIndex = -1,
		m_bottomFaceBeginIndex = -1,
		m_bottomFaceEndIndex = -1;
	std::vector<Texture> m_textures;
	/*  Render data  */
	unsigned int m_VAO, m_VBO, m_EBO;

	void setupMesh();
	void drawRange(unsigned int begin, unsigned int end);

};
