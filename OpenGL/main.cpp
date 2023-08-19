#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/shader.h>
#include <learnopengl/camera.h>

#include <iostream>

//#define STB_IMAGE_IMPLEMENTATION 
//#include <learnopengl/stb_image.h>

#include "room.h"
#include "plane.h"
#include "vect.h"
#include "particle.h"
#include "source.h"

// Screen size
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

// Utils (Camera, Timing)
#include "utils.h"

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Bounces - G1 [Cristian Bastidas, Julio Mora, Erick Vera, Jonathan Gonzalez]", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	double colors[] = {
		239.0f / 255.0f, 246.0f / 255.0f, 252.0f / 255.0f, 0.8,
		239.0f / 255.0f, 246.0f / 255.0f, 252.0f / 255.0f, 0.8,
		239.0f / 255.0f, 246.0f / 255.0f, 252.0f / 255.0f, 0.8,
		239.0f / 255.0f, 246.0f / 255.0f, 252.0f / 255.0f, 0.8,
		239.0f / 255.0f, 246.0f / 255.0f, 252.0f / 255.0f, 0.8,
		239.0f / 255.0f, 246.0f / 255.0f, 252.0f / 255.0f, 0.8,
	};

	// Triangle numbers
	const int n = 1800; // 2, 8, 18, 32, 56, 2n*n, 800, 1800
	const int faces = 6;

	Room room = Room(n, faces);

	const int tnt = n * faces;
	double** toDraw = room.getAllVertices();
	double allVertices[n * faces][9]{};
	for (int i = 0; i < n * faces; i++) {
		for (int j = 0; j < 9; j++) {
			allVertices[i][j] = toDraw[i][j];
		}
	}
	delete[] toDraw;

	unsigned int VBO[tnt], VAO[tnt];
	glGenVertexArrays(tnt, VAO);
	glGenBuffers(tnt, VBO);


	for (int i = 0; i < tnt; i++) {
		glBindVertexArray(VAO[i]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(allVertices[i]), allVertices[i], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(double), (void*)0);
		glEnableVertexAttribArray(0);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	Shader roomShader("shaders/room.vs", "shaders/room.fs");

	initSourceBuffers();
	Source source = Source({ 0.0, 0.0, 0.0 }, 20, 1, 0.1);

	initParticleBuffers();

	// RENDER LOOP
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);
		glClearColor(30.0f / 255.0f, 30.0f / 255.0f, 30.0f / 255.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Room
		roomShader.use();
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		roomShader.setMat4("model", model);
		roomShader.setMat4("view", view);
		roomShader.setMat4("projection", projection);

		int j = 0;
		for (int i = 0; i < tnt; i++) {
			roomShader.setVec4(
				"triangle_color",
				glm::vec4(
					colors[j],
					colors[j + 1],
					colors[j + 2],
					colors[j + 3]
				)
			);
			if (i % n == 0) {
				j += 3;
			}
			glBindVertexArray(VAO[i]);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}


		if (!particlesState) {
			deltaTime = 0;
		}

		// Source
		source.transform(deltaTime, currentFrame, view, projection);

		for (int i = 0; i < 20; i++) {
			// Particles
			source.particles[i].transform(deltaTime, currentFrame, view, projection);

			// Collisions
			room.handleParticleCollision(source.particles[i]);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(n, VAO);
	glDeleteBuffers(n, VBO);

	deleteSourceBuffers();
	deleteParticleBuffers();

	glfwTerminate();
	return 0;
}
