#ifndef SOURCE_H
#define SOURCE_H

#include <ostream>
#include "point.h"

#include <learnopengl/shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

unsigned int sourceVAO;
unsigned int sourceVBO;

float icosahedron[] = {
	0.35, 0.2, 1.06,
	-0.35, 0.2, 1.06,
	0.0, -0.4, 1.06,

	0.57, -0.33, 0.4,
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
	-0.57, 0.33, 0.4,
	0, 0.41, 0,

	-0.57, 0.33, 0.4,
	-0.35, -0.2, 0,
	-0.57, -0.32, 0.65,

	-0.57, 0.33, 0.4,
	-0.35, -0.2, 0,
	-0.57, -0.32, 0.65,

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
public:
	Point position;
	float* vertices;
	int size;
	std::string name;

	Source(Point p) {
		vertices = icosahedron;
		size = 20 * 3;
		name = "Source";
		shader = Shader("shaders/cube.vs", "shaders/cube.fs");
		sourceColor = glm::vec4(246.0f / 255.0f, 48.0f / 255.0f, 0.0f, 0.0f);
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
		sourceTransform = glm::scale(sourceTransform, glm::vec3(0.15f));

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
