#ifndef PARTICLE_H
#define PARTICLE_H

#include <ostream>
#include "point.h"
#include "vect.h"

#include <learnopengl/shader.h>
#include <glm/glm.hpp>

float cubeVertices[] = {
	// positions          // texture Coords
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

class Particle {
public:
	Point position;
	Vect incidence;
	Shader shader;
	float* vertices;
	float energy;
	float loss;
	bool allowScale;
	int size;

	Particle(float* v, Shader s, double e, double l, Point p, Vect i) {
		vertices = v;
		energy = e;
		loss = l;
		position = p;
		incidence = i;
		shader = s;
		allowScale = false;
	}

	Particle(Shader s, double e, double l, Point p, Vect i) {
		vertices = cubeVertices;
		energy = e;
		loss = l;
		position = p;
		incidence = i;
		shader = s;
		allowScale = false;
		size = 36;
	}

	void setAllowScale(bool b) {
		allowScale = b;
	}

	void transform(float deltaTime, float currentFrame, glm::mat4 view, glm::mat4 projection) {
		shader.use();
		shader.setVec4("color", glm::vec4(246.0f / 255.0f, 48.0f / 255.0f, 0.0f, 0.0f));

		glm::mat4 particleTransform = glm::mat4(1.0f);
		position = position + incidence.asPoint() * deltaTime * energy;
		particleTransform = glm::translate(particleTransform, glm::vec3(position.x, position.y, position.z));
		particleTransform = glm::scale(particleTransform, glm::vec3(0.05f) * (allowScale ? energy / 5.0f : 1));

		particleTransform = glm::rotate(particleTransform, currentFrame * energy, glm::vec3(0.0f, 0.0f, 1.0f));
		particleTransform = glm::rotate(particleTransform, currentFrame * energy, glm::vec3(1.0f, 0.0f, 0.0f));
		particleTransform = glm::rotate(particleTransform, currentFrame * energy, glm::vec3(0.0f, 1.0f, 0.0f));

		shader.setMat4("model", particleTransform);
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);
	}

	friend std::ostream& operator<<(std::ostream& os, const Particle& p) {
		os << "Particle: " << p.vertices << " " << p.energy << " " << p.loss << " " << p.position << " " << p.incidence;
		return os;
	}

};

#endif // PARTICLE_H