#ifndef RECEPTOR_H
#define RECEPTOR_H

#include <ostream>

#include "point.h"
#include "triangle.h"
#include "source.h"
#include "particle.h"

const glm::vec4 DEFAULT_RECEPTOR_COLOR = glm::vec4(0.32, 0.8, 0.37, 1); /* Color por defecto del receptor */
const int MAX_RECEPTOR_DATA = 10000;

unsigned int receptorVAO; /* Vertex Array Object */
unsigned int receptorVBO; /* Vertex Buffer Object */

/**
 * @class Receptor
 * @brief Clase que representa un receptor de part�culas
 */
class Receptor {
private:
	Shader shader; /* Shader para dibujar el receptor */
	glm::vec4 receptorColor; /* Color del receptor */


	/**
	 * @brief Genera los tri�ngulos que forman el receptor en base a los v�rtices del icosaedro.
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
	int size;			/* N�mero de caras del receptor */
	int ID;	/* Nombre del receptor */
	float scale;		/* Escala del receptor */
	Point position;		/* Posici�n del receptor */
	double radio;		/* Radio del receptor */
	double energy; /* Energ�a del receptor */
	double* energyRoom;
	double** data; /* Datos del receptor */
	int idx = 0; /* �ndice de los datos del receptor */
	bool saved; /* Indica si el receptor ha sido guardado */
	float lt;

	std::vector<Triangle> triangles; /* Tri�ngulos que forman el receptor */

	/**
	 * @brief Elimina los buffers del recptor.
	 */
	static void deleteReceptorBuffers() {
		glDeleteVertexArrays(1, &receptorVAO);
		glDeleteBuffers(1, &receptorVBO);
	}

	/**
	 * @brief Inicializa los buffers del receptor.
	 */
	static void initReceptorBuffers() {
		glGenVertexArrays(1, &receptorVAO);
		glGenBuffers(1, &receptorVBO);
		glBindVertexArray(receptorVAO);
		glBindBuffer(GL_ARRAY_BUFFER, receptorVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(icosahedron), &icosahedron, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(3 * sizeof(float)));
	}

	Receptor() {}

	/**
	 * @brief Constructor de la clase Receptor.
	 * @param p Posici�n del receptor
	 * @param s Escala del receptor
	 */
	Receptor(Point p, float s) {
		size = 20 * 3;
		saved = false;
		ID = -1;
		shader = Shader("shaders/cube.vs", "shaders/cube.fs");
		receptorColor = DEFAULT_RECEPTOR_COLOR;
		scale = s;
		position = p + (errorTranslation * scale);
		radio = computeRadio();
		genTriangles();
		data = new double* [MAX_RECEPTOR_DATA];
		idx = 0;
		lt = 0;
	}

	Receptor& operator=(const Receptor& r) {
		size = r.size;
		ID = r.ID;
		shader = r.shader;
		receptorColor = r.receptorColor;
		scale = r.scale;
		position = r.position;
		radio = r.radio;
		triangles = r.triangles;
		energy = r.energy;
		energyRoom = r.energyRoom;
		data = r.data;
		idx = r.idx;
		saved = r.saved;
		lt = r.lt;
		return *this;
	}

	bool operator==(const Receptor& r) {
		return r.ID == ID;
	}

	bool operator!=(const Receptor& r) {
		return r.ID != ID;
	}

	Receptor(const Receptor& r) {
		size = r.size;
		ID = r.ID;
		shader = r.shader;
		receptorColor = r.receptorColor;
		scale = r.scale;
		position = r.position;
		radio = r.radio;
		triangles = r.triangles;
		energy = r.energy;
		energyRoom = r.energyRoom;
		data = r.data;
		idx = r.idx;
		saved = r.saved;
		lt = r.lt;
	}

	double computeRadio() {
		return 0.21 * sqrt(3) * (3 + sqrt(5)) * scale / 12;
	}

	/**
	 * @brief Configura el color del receptor
	 */
	void setReceptorColor(glm::vec4 color) {
		receptorColor = color;
	}


	void setEnergyRoom(double* energyRoom) {
		this->energyRoom = energyRoom;
	}

	/**
	 * @brief Configura el nombre del receptor
	 */
	void setID(int n) {
		ID = n;
	}

	/**
	 * @brief Renderiza el receptor.
	 * @param deltaTime Tiempo entre frames
	 * @param currentFrame Tiempo actual
	 * @param view Matriz de vista
	 * @param projection Matriz de proyecci�n
	 */
	void transform(float deltaTime, float currentFrame, glm::mat4 view, glm::mat4 projection) {
		shader.use();
		shader.setVec4("color", receptorColor);

		glm::mat4 sourceTransform = glm::mat4(1.0f);

		sourceTransform = glm::translate(sourceTransform, glm::vec3(position.x, position.y, position.z));
		sourceTransform = glm::scale(sourceTransform, glm::vec3(scale));

		shader.setMat4("model", sourceTransform);
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);

		glBindVertexArray(receptorVAO);
		glDrawArrays(GL_TRIANGLES, 0, size);

		shader.setVec4("color", receptorColor - glm::vec4(0.3));
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_TRIANGLES, 0, size);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glBindVertexArray(0);

	}

	void handleParticleCollision(Particle& p, float currentTime, bool paused) {
		Vect dist = Vect(p.position, position);
		if (p.lastTriangle == -1) {
			return;
		}

		double particleEnergy = 0;
		double roomEnergy = 0;
		if (dist.length() < radio && p.lastReceptor == -1) {
			energy = p.energy + energyRoom[p.lastTriangle];
			p.setLastReceptor(ID);
			receptorColor = receptorColor + p.energy * glm::vec4(0.05);
		}

		if (lt == 0) {
			lt = currentTime;
		}

		if (!saved) {
			if (lt == currentTime) {
				return;
			}
			else {
				lt = currentTime;
			}

			if (paused && idx < MAX_RECEPTOR_DATA) {
				data[idx] = new double[4];
				data[idx][0] = currentTime;
				data[idx][1] = energy;
				idx++;
			}
			else if (idx >= MAX_RECEPTOR_DATA) {
				saved = true;
				char* filename = new char[100];
				snprintf(filename, 100, "csv/receptors/receptor_%f_%f_%f.csv", position.x, position.y, position.z);
				CSV(filename, data, MAX_RECEPTOR_DATA, 2);
				std::cout << "Receptor " << ID << " saved" << std::endl;
			}
		}
	}

	friend std::ostream& operator<<(std::ostream& os, const Receptor& r) {
		os << r.position << " " << r.scale << " " << r.ID;
		return os;
	}

};

#endif // RECEPTOR_H
