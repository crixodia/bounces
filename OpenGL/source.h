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

const Point errorTranslation = { -0.061, 0.066, 0.025 };
float icosahedron[] = {
	0.112, -0.036, 0.068,
	0.008, -0.036, 0.068,
	0.06, -0.126, 0.068,

	0.146, -0.016, -0.031,
	0.112, -0.036, 0.068,
	0.146, -0.114, 0.006,

	0.06, -0.126, 0.068,
	0.146, -0.114, 0.006,
	0.112, -0.036, 0.068,

	0.112, -0.036, 0.068,
	0.06, 0.033, 0.006,
	0.008, -0.036, 0.068,

	0.112, -0.036, 0.068,
	0.146, -0.016, -0.031,
	0.06, 0.033, 0.006,

	0.06, -0.126, 0.068,
	0.008, -0.036, 0.068,
	-0.025, -0.114, 0.006,

	0.008, -0.036, 0.068,
	0.06, 0.033, 0.006,
	-0.025, -0.016, -0.031,

	0.008, -0.036, 0.068,
	-0.025, -0.016, -0.031,
	-0.025, -0.114, 0.006,

	0.06, -0.126, 0.068,
	-0.025, -0.114, 0.006,
	0.06, -0.164, -0.031,

	0.06, -0.126, 0.068,
	0.06, -0.164, -0.031,
	0.146, -0.114, 0.006,

	0.06, -0.005, -0.092,
	0.112, -0.096, -0.092,
	0.008, -0.096, -0.092,

	0.06, -0.005, -0.092,
	0.146, -0.016, -0.031,
	0.112, -0.096, -0.092,

	0.06, 0.033, 0.006,
	0.146, -0.016, -0.031,
	0.06, -0.005, -0.092,

	0.146, -0.016, -0.031,
	0.146, -0.114, 0.006,
	0.112, -0.096, -0.092,

	0.06, -0.005, -0.092,
	0.008, -0.096, -0.092,
	-0.025, -0.016, -0.031,

	0.06, 0.033, 0.006,
	0.146, -0.016, -0.031,
	0.06, -0.005, -0.092,

	-0.025, -0.016, -0.031,
	0.008, -0.096, -0.092,
	-0.025, -0.114, 0.006,

	0.06, -0.164, -0.031,
	0.112, -0.096, -0.092,
	0.146, -0.114, 0.006,

	0.008, -0.096, -0.092,
	0.06, -0.164, -0.031,
	-0.025, -0.114, 0.006,

	0.112, -0.096, -0.092,
	0.06, -0.164, -0.031,
	0.008, -0.096, -0.092,
};

void initSourceBuffers(float scale, Point translate) {
	for (int i = 0; i < 180; i++) {
		icosahedron[i] *= scale;
		if (i % 3 == 0)
			icosahedron[i] += translate.x + scale * errorTranslation.x;
		else if (i % 3 == 1)
			icosahedron[i] += translate.y + scale * errorTranslation.y;
		else
			icosahedron[i] += translate.z + scale * errorTranslation.z;
	}

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
	int size;
	std::string name;
	int numParticles;

	float energy;
	float loss;

	std::vector<Particle> particles;
	std::vector<Point> positions;
	std::vector<Triangle> triangles;

	Source(Point p, int n, float e, float l) {
		initSourceBuffers(1.0f, p);

		size = 20 * 3;
		name = "Source";
		shader = Shader("shaders/cube.vs", "shaders/cube.fs");
		sourceColor = glm::vec4(246.0f / 255.0f, 148.0f / 255.0f, 0.0f, 0.0f);

		energy = e;
		loss = l;

		numParticles = n;
		genTriangles();

		for (int i = 0; i < triangles.size(); i++) {
			std::vector<Vect> tempDirs = genParticlesDirection(triangles[i], numParticles / triangles.size());
			for (int j = 0; j < tempDirs.size(); j++) {
				// rand between 0.0 and 0.3, Just for testing - Remove when it is done
				// float randX = (float)rand() / RAND_MAX * 0.3f;
				Particle temp = Particle(energy, loss, triangles[i].getBarycenter(), tempDirs[j]);
				particles.push_back(temp);
			}
		}
	}

	std::vector<Vect> genParticlesDirection(Triangle t, int vectPerTriangle) {
		std::vector<Vect> vects;
		for (int i = 0; i < vectPerTriangle; i++) {
			vects.push_back(t.getNormal()); // Just for testing
			// std::cout << "{" << t << "," << t.getNormal() << "}\n";
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
