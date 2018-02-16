#pragma once
#include "Mesh.h"

class MeshGenerator
{
	static std::vector<Vertex> createRectangle(glm::vec3 start, glm::vec3 vec1, glm::vec3 vec2);
	static void setNormalVectors(std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices);
	static glm::vec3 getNormalFromVertices(Vertex p1, Vertex p2, Vertex p3);

public:
	static Mesh createCuboid(float width, float length, float height);
	static Mesh createCylinder(float radius, float height);
	static Mesh createCone(float radius, float height);
	static Mesh createSphere(float radius);
};

