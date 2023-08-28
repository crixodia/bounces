#ifndef SOURCE_H
#define SOURCE_H

#include <ostream>
#include "point.h"
#include "particle.h"

#include <learnopengl/shader.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

constexpr auto PI = 3.14159265358979323846; /* pi */

unsigned int sourceVAO; /* Vertex Array Object */
unsigned int sourceVBO; /* Vertex Buffer Object */

const Point errorTranslation = { -0.061, 0.066, 0.025 }; /* Traslación para corregir el erro en la posición de la fuente */

/* Vertices del icosaedro */
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

/**
 * @class Source
 * @brief Clase que representa una fuente de partículas
 */
class Source {
private:
	Shader shader; /* Shader para dibujar la fuente */
	glm::vec4 sourceColor; /* Color de la fuente */

	/**
	 * @brief Genera los triángulos que forman la fuente en base a los vértices del icosaedro.
	 */
	void genTriangles() {
		int j = 0;
		for (int i = 0; i < 20; i++) {
			float sx = position.x;
			float sy = position.y;
			float sz = position.z;

			Point a = Point(icosahedron[j] + sx, icosahedron[j + 1] + sy, icosahedron[j + 2] + sz) * scale;
			Point b = Point(icosahedron[j + 3] + sx, icosahedron[j + 4] + sy, icosahedron[j + 5] + sz) * scale;
			Point c = Point(icosahedron[j + 6] + sx, icosahedron[j + 7] + sy, icosahedron[j + 8] + sz) * scale;
			triangles.push_back(Triangle(a, b, c));
			j += 9;
		}
	}
public:
	int size;			/* Número de caras de la fuente */
	std::string name;	/* Nombre de la fuente */
	int numParticles;	/* Número de partículas */
	float scale;		/* Escala de la fuente */
	Point position;		/* Posición de la fuente */
	float energy;		/* Energía de la fuente y de todas las paráticulas que contiene */
	float loss;			/* Pérdida de energía de la fuente y de todas las partículas que contiene */

	std::vector<Particle> particles; /* Partículas que contiene la fuente */
	std::vector<Point> positions;	 /* Posiciones de las partículas que contiene la fuente */
	std::vector<Triangle> triangles; /* Triángulos que forman la fuente */

	/**
	 * @brief Elimina los buffers de la fuente.
	 */
	static void deleteSourceBuffers() {
		glDeleteVertexArrays(1, &sourceVAO);
		glDeleteBuffers(1, &sourceVBO);
	}

	/**
	 * @brief Inicializa los buffers de la fuente.
	 */
	static void initSourceBuffers() {
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

	/**
	 * @brief Constructor de la clase Source.
	 * @param p Posición de la fuente
	 * @param n Número de partículas
	 * @param e Energía de la fuente
	 * @param l Pérdida de energía de la fuente
	 * @param s Escala de la fuente
	 */
	Source(Point p, int n, float e, float l, float s) {
		size = 20 * 3;
		name = "Source";
		shader = Shader("shaders/cube.vs", "shaders/cube.fs");
		sourceColor = glm::vec4(246.0f / 255.0f, 148.0f / 255.0f, 0.0f, 0.0f);
		scale = s;
		energy = e;
		loss = l;
		position = p + (errorTranslation * scale);
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

	/**
	 * @brief Obtiene todas las direcciones de las partículas que se generan en un triángulo en base a la normal del triángulo.
	 * Las partículas se generan en forma de abanico con un ángulo de separación de 15 grados. Luego, los vectores subsecuentes se
	 * obtienen rotando el primer vector en el eje normal del triángulo.
	 *
	 * @param t Triángulo
	 * @param vectPerTriangle Número de partículas que se generan en el triángulo
	 * @return std::vector<Vect> Direcciones de las partículas
	 */
	std::vector<Vect> genParticlesDirection(Triangle t, int vectPerTriangle) {
		std::vector<Vect> vects;

		const float angleSep = 15;

		Vect normal = t.getNormal();
		vects.push_back(normal);

		Vect perpendicular = Vect(t.getA(), t.getB()).unit();
		perpendicular.setStart(t.getBarycenter());

		Vect firstVect = Vect::rodriges(perpendicular, normal, angleSep * PI / 180).unit();
		firstVect.setStart(t.getBarycenter());

		for (int i = 0; i < vectPerTriangle - 1; i++) {
			Vect rot = Vect::rodriges(normal, firstVect, 2 * i * PI / (vectPerTriangle - 1));
			rot.setStart(t.getBarycenter());
			vects.push_back(rot);
		}
		return vects;
	}

	/**
	 * @brief Configura el color de la fuente.
	 */
	void setSourceColor(glm::vec4 color) {
		sourceColor = color;
	}

	/**
	 * @brief Configura el nombre de la fuente.
	 */
	void setName(std::string n) {
		name = n;
	}

	/**
	 * @brief Renderiza la fuente.
	 * @param deltaTime Tiempo entre frames
	 * @param currentFrame Tiempo actual
	 * @param view Matriz de vista
	 * @param projection Matriz de proyección
	 */
	void transform(float deltaTime, float currentFrame, glm::mat4 view, glm::mat4 projection) {
		shader.use();
		shader.setVec4("color", sourceColor);

		glm::mat4 sourceTransform = glm::mat4(1.0f);

		sourceTransform = glm::translate(sourceTransform, glm::vec3(position.x, position.y, position.z));
		sourceTransform = glm::scale(sourceTransform, glm::vec3(scale));

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
