#ifndef SOURCE_H
#define SOURCE_H

#include <ostream>
#include "point.h"
#include "particle.h"

#include <learnopengl/shader.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

unsigned int sourceVAO;
unsigned int sourceVBO;

float icosahedron[] = {
	0.35, 0.2, 1.06,
	-0.35, 0.2, 1.06,
	0.0, -0.4, 1.06,

	0.57, 0.33, 0.4,
	0.57, -0.32, 0.65,
	0.35, 0.2, 1.06,

	0, -0.4, 1.06,
	0.35, 0.2, 1.06,
	0.57, -0.32, 0.65,

	0.35, 0.2, 1.06,
	-0.35, 0.2, 1.06,
	0, 0.66, 0.65,

	0.35, 0.2, 1.06,
	0, 0.66, 0.65,
	0.57, 0.33, 0.4,

	0, -0.4, 1.06,
	-0.35, 0.2, 1.06,
	-0.57, -0.32, 0.65,

	-0.35, 0.2, 1.06,
	0, 0.66, 0.65,
	-0.57, 0.33, 0.4,

	-0.35, 0.2, 1.06,
	-0.57, -0.32, 0.65,
	-0.57, 0.33, 0.4,

	0, -0.4, 1.06,
	-0.57, -0.32, 0.65,
	0, -0.65, 0.4,

	0, -0.4, 1.06,
	0.57, -0.32, 0.65,
	0, -0.65, 0.4,

	0, 0.41, 0,
	-0.35, -0.2, 0,
	0.35, -0.2, 0,

	0, 0.41, 0,
	0.57, 0.33, 0.4,
	0.35, -0.2, 0,

	0, 0.66, 0.65,
	0, 0.41, 0,
	0.57, 0.33, 0.4,

	0.57, 0.33, 0.4,
	0.35, -0.2, 0,
	0.57, -0.32, 0.65,

	0, 0.41, 0,
	-0.57, 0.33, 0.4,
	-0.35, -0.2, 0,

	0, 0.66, 0.65,
	0.57, 0.33, 0.4,
	0, 0.41, 0,

	-0.57, 0.33, 0.4,
	-0.35, -0.2, 0,
	-0.57, -0.32, 0.65,

	0, -0.65, 0.4,
	0.35, -0.2, 0,
	0.57, -0.32, 0.65,

	-0.35, -0.2, 0,
	-0.57, -0.32, 0.65,
	0, -0.65, 0.4,

	0.35, -0.2, 0,
	-0.35, -0.2, 0,
	0, -0.65, 0.4
};

void initSourceBuffers() {
	glGenVertexArrays(1, &sourceVAO);
	glGenBuffers(1, &sourceVBO);
	glBindVertexArray(sourceVAO);
	glBindBuffer(GL_ARRAY_BUFFER, sourceVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(icosahedron), &icosahedron, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(3 * sizeof(float)));
}

void deleteSourceBuffers() {
	glDeleteVertexArrays(1, &sourceVAO);
	glDeleteBuffers(1, &sourceVBO);
}

class Source {
private:
	Shader shader;
	glm::vec4 sourceColor;

	void genTriangles() {
		int j = 0;
		for (int i = 0; i < 20; i++) {
			Point a = Point(icosahedron[j], icosahedron[j + 1], icosahedron[j + 2]);
			Point b = Point(icosahedron[j + 3], icosahedron[j + 4], icosahedron[j + 5]);
			Point c = Point(icosahedron[j + 6], icosahedron[j + 7], icosahedron[j + 8]);
			triangles.push_back(Triangle(a, b, c));
			j += 9;
		}
	}
public:
	Point position;
	float* vertices;
	int size;
	std::string name;
	int numParticles;

	float energy;
	float loss;

	std::vector<Particle> particles;
	std::vector<Point> positions;
	std::vector<Triangle> triangles;

	Source(Point p, int n, float e, float l) {
		vertices = icosahedron;
		size = 20 * 3;
		name = "Source";
		shader = Shader("shaders/cube.vs", "shaders/cube.fs");
		sourceColor = glm::vec4(246.0f / 255.0f, 148.0f / 255.0f, 0.0f, 0.0f);

		energy = e;
		loss = l;

		position = p;
		numParticles = n;
		genTriangles();

		for (int i = 0; i < triangles.size(); i++) {
			std::vector<Vect> tempDirs = genParticlesDirection(triangles[i], numParticles / triangles.size());
			for (int j = 0; j < tempDirs.size(); j++) {
				// rand between 0.0 and 0.3, Just for testing - Remove when it is done
				float randX = (float)rand() / RAND_MAX * 0.3f;
				Particle temp = Particle(energy, loss, triangles[i].getBarycenter(), tempDirs[j]);
				particles.push_back(temp);
			}
		}
	}

	std::vector<Vect> genParticlesDirection(Triangle t, int vectPerTriangle) {
		std::vector<Vect> vects;
		for (int i = 0; i < vectPerTriangle; i++) {
			vects.push_back(t.getNormal()); // Just for testing
			std::cout << "Normal: " << t << " " << t.getNormal() << std::endl;
		}

		return vects;
	}

	void setSourceColor(glm::vec4 color) {
		sourceColor = color;
	}

	void setName(std::string n) {
		name = n;
	}

	void transform(float deltaTime, float currentFrame, glm::mat4 view, glm::mat4 projection) {
		shader.use();
		shader.setVec4("color", sourceColor);

		glm::mat4 sourceTransform = glm::mat4(1.0f);

		sourceTransform = glm::translate(sourceTransform, glm::vec3(position.x, position.y, position.z));
		sourceTransform = glm::scale(sourceTransform, glm::vec3(0.1f));

		shader.setMat4("model", sourceTransform);
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);

		glBindVertexArray(sourceVAO);
		glDrawArrays(GL_TRIANGLES, 0, size);

		shader.setVec4("color", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_TRIANGLES, 0, size);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glBindVertexArray(0);

	}
};

#endif // SOURCE_H
