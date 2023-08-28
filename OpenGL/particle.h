#ifndef PARTICLE_H
#define PARTICLE_H

#include <ostream>
#include "point.h"
#include "vect.h"

#include <learnopengl/shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

unsigned int cubeVAO; /* Vertex Array Object */
unsigned int cubeVBO; /* Vertex Buffer Object */

/* Vertices del poliedro que forma la par�ticula */
float cubeVertices[] = {
	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,

	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,

	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,

	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
};

class Particle {
private:
	Shader shader; /* Shader para dibujar la part�cula */
	bool allowScale; /* Indica si se puede escalar la part�cula */
	glm::vec4 particleColor; /* Color de la part�cula */
public:
	Point position;		/* Posici�n de la part�cula */
	Vect incidence;		/* Vector de incidencia de la part�cula */
	float energy;		/* Energ�a de la part�cula */
	float loss;			/* P�rdida de energ�a de la part�cula */
	int size;			/* Tama�o del poliedro que forma la part�cula */
	std::string name;	/* Nombre de la part�cula */

	static void initParticleBuffers() {
		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
		glBindVertexArray(cubeVAO);
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(3 * sizeof(float)));
	}


	static void deleteParticleBuffers() {
		glDeleteVertexArrays(1, &cubeVAO);
		glDeleteBuffers(1, &cubeVBO);
	}

	Particle(float* v, double e, double l, Point p, Vect i) {
		energy = e;
		loss = l;
		position = p;
		incidence = i;
		allowScale = false;
		name = "Particle";
		shader = Shader("shaders/cube.vs", "shaders/cube.fs");
		particleColor = glm::vec4(246.0f / 255.0f, 48.0f / 255.0f, 0.0f, 0.0f);
	}

	Particle(double e, double l, Point p, Vect i) {
		energy = e;
		loss = l;
		position = p;
		incidence = i;
		size = 36;
		allowScale = false;
		name = "Particle";
		shader = Shader("shaders/cube.vs", "shaders/cube.fs");
		particleColor = glm::vec4(246.0f / 255.0f, 48.0f / 255.0f, 0.0f, 0.0f);
	}

	void setAllowScale(bool b) {
		allowScale = b;
	}

	void setParticleColor(glm::vec4 c) {
		particleColor = c;
	}

	void setName(std::string n) {
		name = n;
	}

	void transform(float deltaTime, float currentFrame, glm::mat4 view, glm::mat4 projection) {
		shader.use();
		shader.setVec4("color", particleColor);

		glm::mat4 particleTransform = glm::mat4(1.0f);
		position = position + incidence.asPoint() * deltaTime * energy;

		particleTransform = glm::translate(particleTransform, glm::vec3(position.x, position.y, position.z));
		particleTransform = glm::scale(particleTransform, glm::vec3(0.02f) * (allowScale ? energy / 5.0f : 1));
		particleTransform = glm::rotate(particleTransform, currentFrame * energy, glm::vec3(0.0f, 0.0f, 1.0f));
		particleTransform = glm::rotate(particleTransform, currentFrame * energy, glm::vec3(1.0f, 0.0f, 0.0f));
		particleTransform = glm::rotate(particleTransform, currentFrame * energy, glm::vec3(0.0f, 1.0f, 0.0f));


		shader.setMat4("model", particleTransform);
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);

		glBindVertexArray(cubeVAO);
		//glActiveTexture(GL_TEXTURE0);
		glDrawArrays(GL_TRIANGLES, 0, size);
		glBindVertexArray(0);
	}

	friend std::ostream& operator<<(std::ostream& os, const Particle& p) {
		os << p.name << ": " << p.energy << " " << p.loss << " " << p.position << " " << p.incidence;
		return os;
	}

};

#endif // PARTICLE_H
