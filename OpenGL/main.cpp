#include <iostream>

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

#include "utils.h"

#include "room.h"
#include "source.h"
#include "receptor.h"
#include "particle.h"

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

	// Receptors
	int RECEPTORS = 27; // 3, 9, 27, 81, 243, 729, 2187
	int receptorsPerSide = pow(RECEPTORS, 1.0f / 3.0f);
	float receptorDelta = (2 * 1) / (receptorsPerSide - static_cast<float>(1));
	Receptor* receptors = new Receptor[RECEPTORS];
	int l = 0;
	for (float i = 0; i < receptorsPerSide; i += 1) {
		for (float j = 0; j < receptorsPerSide; j += 1) {
			for (float k = 0; k < receptorsPerSide; k += 1) {
				//Receptor rec = Receptor({ -1.6 + i * receptorDelta, -1.6 + j * (receptorDelta - 0.4), -1.6 + k * receptorDelta }, 1.0f);
				Receptor rec = Receptor({ -1 + i * receptorDelta, -1 + j * (receptorDelta), -1 + k * receptorDelta }, 1.0f);
				rec.setID(l);
				receptors[l] = rec;
				l++;
			}
		}
	}

	// Triangle numbers
	const int n = 242; // 2, 8, 18, 32, 50, 2n*n, 800, 1800
	// El l�mite es 242 si se quiere calcular la transferencia de energ�a
	// se recomienda usar 128 para un rendimiento �ptimo
	const int faces = 6;

	Room room = Room(n, faces, RECEPTORS, receptors);

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

	Particle::initParticleBuffers();
	Source::initSourceBuffers();
	Receptor::initReceptorBuffers();

	int MAX_PARTICLES = 800; // Se recomienda usar 400 para un rendimiento �ptimo
	float ENERGY = 800;
	float LOSS = 0.2;

	//Source source = Source({ 0, 0, 0 }, MAX_PARTICLES, ENERGY, LOSS);
	Source source2 = Source({ 1.6, 1.6, -1.6 }, MAX_PARTICLES, ENERGY, LOSS);

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

		std::vector<glm::vec4> roomColors = room.getTriangleColors();
		for (int i = 0; i < tnt; i++) {
			roomShader.setVec4("triangle_color", roomColors[i]);
			glBindVertexArray(VAO[i]);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}


		if (!particlesState) {
			deltaTime = 0;
		}

		// Receptors
		for (int i = 0; i < RECEPTORS; i++) {
			receptors[i].transform(deltaTime, currentFrame, view, projection);
		}

		// Source
		//source.transform(deltaTime, currentFrame, view, projection);
		source2.transform(deltaTime, currentFrame, view, projection);

		for (int i = 0; i < MAX_PARTICLES; i++) {
			// Particles
			//source.particles[i].transform(deltaTime, currentFrame, view, projection);
			source2.particles[i].transform(deltaTime, currentFrame, view, projection);

			// Collisions
			//room.handleParticleCollision(source.particles[i]);
			room.handleParticleCollision(source2.particles[i]);

			// Receptor Collision
			for (int j = 0; j < RECEPTORS; j++) {
				receptors[j].handleParticleCollision(source2.particles[i], currentFrame, particlesState);
			}
		}



		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(n, VAO);
	glDeleteBuffers(n, VBO);

	Source::deleteSourceBuffers();
	Particle::deleteParticleBuffers();
	Receptor::deleteReceptorBuffers();

	glfwTerminate();
	return 0;
}
