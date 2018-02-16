#include "MeshGenerator.h"
#include <glm/detail/func_geometric.inl>
#include <iostream>

std::vector<Vertex> MeshGenerator::createRectangle(glm::vec3 start, glm::vec3 vec1, glm::vec3 vec2)
{
	std::vector<Vertex> vertices;
	for (int i = 0; i < 4; ++i)
	{
		vertices.emplace_back();
	}

	// calculating positions
	vertices[0].position = start;
	vertices[1].position = (start += vec1);
	vertices[2].position = (start += vec2);
	vertices[3].position = (start -= vec1);

	// calculating normals
	glm::vec3 v1 = vertices[1].position - vertices[0].position;
	glm::vec3 v2 = vertices[2].position - vertices[0].position;
	glm::vec3 normal = glm::cross(v1, v2);
	glm::normalize(normal);
	for(int i = 0; i < 4; ++i)
	{
		vertices[i].normal = normal;
	}

	// texcoords
	vertices[0].texCoords = glm::vec2(0.0f, 0.0f);
	vertices[1].texCoords = glm::vec2(1.0f, 0.0f);
	vertices[2].texCoords = glm::vec2(1.0f, 1.0f);
	vertices[3].texCoords = glm::vec2(0.0f, 1.0f);

	return vertices;
}

Mesh MeshGenerator::createCuboid(float width, float length, float height)
{
	std::vector<Vertex> vertices;

	glm::vec3 vectors[] = {
		glm::vec3(width, 0.0f, 0.0f),
		glm::vec3(0.0f, height, 0.0f),
		glm::vec3(0.0f, 0.0f, length)
	};
	glm::vec3 bottomLeftCorner(-width / 2, -height / 2, -length / 2);
	glm::vec3 topRightCorner(width / 2, -height / 2, length / 2);
	std::vector<Vertex> rectangle;

	//corners
	rectangle = createRectangle(bottomLeftCorner, vectors[0], vectors[1]);
	for(auto v : rectangle)
	{
		vertices.push_back(v);
	}
	rectangle = createRectangle(bottomLeftCorner, vectors[2], vectors[1]);
	for(auto v : rectangle)
	{
		vertices.push_back(v);
	}
	rectangle = createRectangle(topRightCorner, -vectors[0], vectors[1]);
	for(auto v : rectangle)
	{
		vertices.push_back(v);
	}
	rectangle = createRectangle(topRightCorner, -vectors[2], vectors[1]);
	for(auto v : rectangle)
	{
		vertices.push_back(v);
	}

	// top face
	rectangle = createRectangle(-bottomLeftCorner, -vectors[0], -vectors[2]);
	for(auto v : rectangle)
	{
		vertices.push_back(v);
	}

	// bottom face
	rectangle = createRectangle(bottomLeftCorner, vectors[0], vectors[2]);
	for(auto v : rectangle)
	{
		vertices.push_back(v);
	}
	
	std::vector<unsigned int> indices;
	// for each wall
	for(int i = 0; i < 6; ++i)
	{
		// first triangle
		indices.emplace_back(4 * i + 0);
		indices.emplace_back(4 * i + 1);
		indices.emplace_back(4 * i + 2);
		//second triangle
		indices.emplace_back(4 * i + 2);
		indices.emplace_back(4 * i + 3);
		indices.emplace_back(4 * i + 0);
	}

	Mesh mesh(vertices, indices);
	mesh.setSideFacesIndexes(0, 23);
	mesh.setTopFaceIndexes(24, 29);
	mesh.setBottomFaceIndexes(30, 35);

	return mesh;
}

Mesh MeshGenerator::createCylinder(float radius, float height)
{
	const static int SLICES = 32;
	const static float M_PI = 3.14159f;
	height /= 2;
	std::vector<Vertex> vertices;
	/*vertex at middle top */
	vertices.emplace_back(glm::vec3(0.0, height, 0.0), glm::vec2(0.5f, 0.5f));
	vertices.emplace_back(glm::vec3(0.0, -height, 0.0), glm::vec2(0.5f, 0.5f));
	// for drawing top and bottom faces
	for(int i = 0; i < SLICES+1; i++){
		float theta = (2.0*M_PI/(float)SLICES)*i;
		vertices.emplace_back(
			glm::vec3(radius*cos(theta), height, radius*sin(theta)),
			glm::vec2(0.5 + 0.5*cos(theta), 0.5 + 0.5*sin(theta))
		);
		vertices.emplace_back(
			glm::vec3(radius*cos(theta), -height, radius*sin(theta)),
			glm::vec2(0.5 + 0.5*cos(theta), 0.5 + 0.5*sin(theta))
		);
	}
	// for drawing side faces
	const float delta = 1.0f / SLICES;
	for(int i = 0; i < SLICES + 1; i++)
	{
		float theta = (2.0 * M_PI / (float)SLICES) * i;
		vertices.emplace_back(
			glm::vec3(radius * cos(theta), height, radius * sin(theta)),
			glm::vec2(i * delta, 1.0f)
		);
		vertices.emplace_back(
			glm::vec3(radius * cos(theta), -height, radius * sin(theta)),
			glm::vec2(i * delta, 0.0f)
		);
	}

	std::vector<unsigned int> indices;
	int i, j;
	for(i = 2, j = 0; j < SLICES; i+=2, ++j)
	{
		indices.emplace_back(0);
		indices.emplace_back(i);
		indices.emplace_back(i+2);

		indices.emplace_back(1);
		indices.emplace_back(i+1);
		indices.emplace_back(i+3);
	}
	i += 2;
	for(j = 0; j < SLICES; ++j, i += 2)
	{
		indices.emplace_back(i);
		indices.emplace_back(i + 1);
		indices.emplace_back(i + 3);

		indices.emplace_back(i + 3);
		indices.emplace_back(i);
		indices.emplace_back(i + 2);

		if(j == SLICES-1)
		{
			std::cout << i + 3 << std::endl;
			std::cout << vertices.size() << std::endl;
			std::cout << std::endl;
		}
	}

	setNormalVectors(vertices, indices);

	return Mesh(vertices, indices);
}

void MeshGenerator::setNormalVectors(std::vector<Vertex>& vertices, const std::vector<unsigned>& indices)
{
	for(int i = 0; i < indices.size(); i += 3)
	{
		glm::vec3 normal = getNormalFromVertices(
			vertices[indices[i]],
			vertices[indices[i+1]],
			vertices[indices[i+2]]
		);
		vertices[indices[i]].normal = normal;
		vertices[indices[i+1]].normal = normal;
		vertices[indices[i+2]].normal = normal;
	}
}

glm::vec3 MeshGenerator::getNormalFromVertices(Vertex p1, Vertex p2, Vertex p3)
{
	// calculating normals
	glm::vec3 v1 = p2.position - p1.position;
	glm::vec3 v2 = p3.position - p1.position;
	glm::vec3 normal = glm::cross(v1, v2);
	normal = glm::normalize(normal);
	return normal;
}

Mesh MeshGenerator::createCone(float radius, float height)
{
	const static int SLICES = 32;
	const static float M_PI = 3.14159f;
	height /= 2;
	float generatrix = sqrt(radius*radius + height*height),
		deltaAngle = 2 * M_PI*radius / (SLICES*generatrix);

	std::vector<Vertex> vertices;
	vertices.emplace_back(glm::vec3(0.0f, height, 0.0f), glm::vec2(0.5f, 0.5f));;
	vertices.emplace_back(glm::vec3(0.0f, -height, 0.0f), glm::vec2(0.5f, 0.5f));
	for(int i = 0; i < SLICES + 1; i++){
		float theta = (2.0*M_PI / (float)SLICES)*i;
		Vertex vertex(
			glm::vec3(radius*cos(theta), -height, radius*sin(theta)), 
			glm::vec2(0.5 + 0.5*sin(deltaAngle*i), 0.5 + 0.5*cos(deltaAngle*i))
		);
		vertices.push_back(vertex);
	}

	std::vector<unsigned int> indices;
	for(int i = 0, j = 2; i < SLICES; ++i, ++j)
	{
		indices.emplace_back(0);
		indices.emplace_back(j);
		indices.emplace_back(j + 1);

		indices.emplace_back(1);
		indices.emplace_back(j);
		indices.emplace_back(j + 1);
	}

	setNormalVectors(vertices, indices);

	return Mesh(vertices, indices);
}

Mesh MeshGenerator::createSphere(float radius)
{
	static const int RINGS = 20,
		SECTORS = 20;
	static const float M_PI = 3.14159f;

	float const R = 1. / (float)(RINGS - 1);
	float const S = 1. / (float)(SECTORS - 1);
	int r, s;

	std::vector<Vertex> vertices(RINGS * SECTORS * 3);

	auto v = vertices.begin();
	for(r = 0; r < RINGS; r++) for(s = 0; s < SECTORS; s++) {
		float const y = sin(-M_PI/2 + M_PI * r * R);
		float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
		float const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

		v->texCoords = glm::vec2(s*S, r*R);
		v->position = glm::vec3(x * radius, y*radius, z*radius);
		v++->normal = glm::vec3(x, y, z);
	}

	std::vector<unsigned int> indices;
	for(r = 0; r < RINGS; r++) for(s = 0; s < SECTORS; s++) {
		indices.emplace_back(r * SECTORS + s);
		indices.emplace_back(r * SECTORS + (s + 1));
		indices.emplace_back((r + 1) * SECTORS + (s + 1));

		indices.emplace_back((r + 1) * SECTORS + (s + 1));
		indices.emplace_back((r + 1) * SECTORS + s);
		indices.emplace_back(r * SECTORS + s);
	}

	return Mesh(vertices, indices);
}
