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


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

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
	const int n = 800; // 2, 8, 18, 32, 56, 2n*n, 800, 1800
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


	unsigned int cubeVAO, cubeVBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	Shader roomShader("shaders/room.vs", "shaders/room.fs");
	Shader cubeShader("shaders/cube.vs", "shaders/cube.fs");

	// Particle Definition
	Particle particle = Particle(cubeShader, 5, 0.2, { 0.9, 0.2, 0.3 }, Vect({ -1.0, -0.1, 0.3 }));

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

		// Draw a line from 1 1 1 to -1 -1 -1
		roomShader.setVec4("triangle_color", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		glBindVertexArray(VAO[0]);
		glDrawArrays(GL_LINES, 0, 2);



		particle.transform(deltaTime, currentFrame, view, projection);

		glBindVertexArray(cubeVAO);
		glActiveTexture(GL_TEXTURE0);
		glDrawArrays(GL_TRIANGLES, 0, particle.size);
		glBindVertexArray(0);

		room.handleParticleCollision(particle);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(n, VAO);
	glDeleteBuffers(n, VBO);

	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteBuffers(1, &cubeVBO);

	glfwTerminate();
	return 0;
}
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);

	// Allows to change between fill and line mode
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
