#ifndef ROOM_H
#define ROOM_H

#include <ostream>
#include "plane.h"
#include "particle.h"
#include "csv.h"

constexpr auto V_SON = 340.0f; /* Constante de la velocidad del sonido en el aire */

/*
 * @brief Clase que representa una habitaci�n
 * @details Una habitaci�n est� formada por un conjunto de planos que la delimitan
 */
class Room {
public:
	Plane* planes;		/* Planos que delimitan la habitaci�n */
	int numPlanes;		/* N�mero de planos que delimitan la habitaci�n */
	int numTriangles;	/* N�mero de tri�ngulos que forman los planos */
	double** energyPr; /* Matriz de porcentajes de energ�a */


	/**
	 * @brief Constructor de la clase Room
	 * @param nt N�mero de tri�ngulos que forman los planos
	 * @param np N�mero de planos que delimitan la habitaci�n
	 */
	Room(int nt, int np) {
		numTriangles = nt;
		numPlanes = np;
		planes = new Plane[numPlanes];
		energyPr = new double* [numTriangles * numPlanes];

		switch (numPlanes) {
		case 6:
			genCube();
			break;
		}
	}

	/**
	 * @brief Genera una habitaci�n en forma de cubo
	 */
	void genCube() {
		Point* floorPoints = new Point[4]{ {-2, -2, 2}, {-2, -2, -2}, {2, -2, 2} ,{2, -2, -2} };
		Plane floor = Plane(floorPoints, 4, numTriangles, "floor");

		Point* ceilingPoints = new Point[4]{ {-2, 2, 2}, {2, 2, 2}, {-2, 2, -2}, {2, 2, -2} };
		Plane ceiling = { ceilingPoints, 4, numTriangles, "ceiling" };

		Point* leftWallPoints = new Point[4]{ {-2, -2, 2}, {-2, 2, 2}, {-2, -2, -2}, {-2, 2, -2} };
		Plane leftWall = { leftWallPoints, 4, numTriangles, "leftWall" };

		Point* rightWallPoints = new Point[4]{ {2, -2, 2}, {2, -2, -2}, {2, 2, 2}, {2, 2, -2} };
		Plane rightWall = { rightWallPoints, 4, numTriangles, "rightWall" };

		Point* frontWallPoints = new Point[4]{ {-2, -2, 2},{2, -2, 2},{-2, 2, 2}, {2, 2, 2} };
		Plane frontWall = { frontWallPoints, 4, numTriangles, "frontWall" };

		Point* backWallPoints = new Point[4]{ {-2, -2, -2}, {-2, 2, -2}, {2, -2, -2}, {2, 2, -2} };
		Plane backWall = { backWallPoints, 4, numTriangles, "backWall" };

		planes[0] = floor;
		planes[1] = ceiling;
		planes[2] = leftWall;
		planes[3] = rightWall;
		planes[4] = frontWall;
		planes[5] = backWall;
	}

	/**
	 * @brief Devuelve los vectores normales de todos los planos que constituyen la habitaci�n
	 * @return Array de vectores normales
	 */
	Vect* getNormals() {
		Vect* normals = new Vect[numPlanes];
		for (int i = 0; i < numPlanes; i++) {
			normals[i] = planes[i].getNormal();
		}
		return normals;
	}

	/**
	 * @brief Devuelve los puntos de todos los planos que constituyen la habitaci�n
	 * @return Array de puntos
	 */
	double** getAllVertices() {
		int tnt = numTriangles * numPlanes;
		double** vertices = new double* [tnt];

		int k = 0;
		for (int i = 0; i < tnt; i++) {
			double* flat = planes[k].triangles[i % numTriangles].flatArray();
			vertices[i] = new double[9];
			for (int j = 0; j < 9; j++) {
				vertices[i][j] = flat[j];
			}
			if (i % numTriangles == numTriangles - 1) {
				k++;
			}
		}

		return vertices;
	}


	/**
	 * @brief Devuelve el puntero del plano candidato a ser el primero en ser alcanzado por una part�cula
	 * @return Array de planos
	 */
	Plane* SurpassedPlane(const Point& p) {
		for (int i = 0; i < numPlanes; i++) {
			if (planes[i].hasSurpassed(p)) {
				return &planes[i];
			}
		}
		return nullptr;
	}

	/**
	 * @brief Controla las colisiones de una part�cula con los planos de la habitaci�n en base al m�todo de reflexiones.
	 * @param p Puntero a la part�cula
	 */
	void handleParticleCollision(Particle& p) {
		int index = nearestSurpassedPlaneIndex(p.position, p.incidence);

		if (index != -1) {
			Plane* nearestSurpassed = &planes[index];
			Triangle* nearestTriangle = nullptr;
			double dist = 1000000;
			int minIndex = 0;
			for (int i = 0; i < numTriangles; i++) {
				double d = Vect(nearestSurpassed->triangles[i].getBarycenter(), p.position).length();
				if (d < dist) {
					dist = d;
					nearestTriangle = &nearestSurpassed->triangles[i];
					minIndex = i;
				}
			}

			//nearestTriangle->setColor(nearestTriangle->getColor() + glm::vec4(p.energy * p.loss, -p.energy * p.loss, -p.energy * p.loss, 0));

			int k = 0;
			for (int i = 0; i < numPlanes; i++) {
				for (int j = 0; j < numTriangles; j++) {
					glm::vec4 colorTransform = glm::vec4(p.energy * p.loss, -p.energy * p.loss, -p.energy * p.loss, 0);
					colorTransform = colorTransform * (float)energyPr[index * numTriangles + minIndex][k];
					planes[i].triangles[j].setColor(planes[i].triangles[j].getColor() + colorTransform);
					k++;
				}
			}

			//std::cout << minIndex << " " << p.name << ": collided with " << nearestSurpassed->name << "\t energy: " << p.energy << std::endl;
			Vect normal = nearestSurpassed->getNormal();
			Vect reflex = nearestSurpassed->reflect(p.incidence);
			Point pi = nearestSurpassed->incidence(p.position, p.incidence);
			p.position = pi;
			p.incidence = reflex;
			p.energy -= p.energy * p.loss;
		}
	}

	/**
	 * @brief [DEPRECATED] Devuelve el puntero del plano m�s cercano a una part�cula
	 * @return Puntero al plano m�s cercano
	 */
	Plane* nearestSurpassedPlane(const Point& p, const Vect& incidence) {
		Plane* nearest = &planes[0];

		if (SurpassedPlane(p) == nullptr) {
			return nullptr;
		}

		double minDistance = nearest->distance(p);
		double distance = nearest->distance(p);

		for (int i = 0; i < numPlanes; i++) {
			distance = planes[i].distance(p);
			if (distance < minDistance) {
				minDistance = distance;
				nearest = &planes[i];
			}
		}
		return nearest;
	}

	/**
	 * @brief Devuelve el �ndice del plano m�s cercano a una part�cula
	 * @return �ndice al plano m�s cercano
	 */
	int nearestSurpassedPlaneIndex(const Point& p, const Vect& incidence) {
		Plane* nearest = &planes[0];

		if (SurpassedPlane(p) == nullptr) {
			return -1;
		}

		double minDistance = nearest->distance(p);
		double distance = nearest->distance(p);

		int index = 0;
		for (int i = 0; i < numPlanes; i++) {
			distance = planes[i].distance(p);
			if (distance < minDistance) {
				minDistance = distance;
				nearest = &planes[i];
				index = i;
			}
		}
		return index;
	}

	/**
	 * @brief Devuelve los colores de los tri�ngulos que constituyen la habitaci�n.
	 * @return Array de colores
	 */
	std::vector<glm::vec4> getTriangleColors() {
		std::vector<glm::vec4> colors;
		for (int i = 0; i < numPlanes; i++) {
			for (int j = 0; j < numTriangles; j++) {
				colors.push_back(planes[i].triangles[j].getColor());
			}
		}
		return colors;
	}

	/**
	 * @brief Devuelve el �ngulo s�lido entre dos tri�ngulos a una distancia dada.
	 * @param from Tri�ngulo desde el que se mide el �ngulo s�lido
	 * @param to Tri�ngulo hasta el que se mide el �ngulo s�lido
	 * @param distance Distancia entre los tri�ngulos
	 * @return �ngulo s�lido
	 */
	double solidAngle(Triangle from, Triangle to, double distance) {
		Point baricenterFrom = from.getBarycenter();

		Vect ABc = Vect(baricenterFrom, to.getA()).unit();
		Vect BBc = Vect(baricenterFrom, to.getB()).unit();
		Vect CBc = Vect(baricenterFrom, to.getC()).unit();

		ABc.setStart(baricenterFrom);
		BBc.setStart(baricenterFrom);
		CBc.setStart(baricenterFrom);

		Point A = ABc.p2;
		Point B = BBc.p2;
		Point C = CBc.p2;

		Triangle temp = Triangle(A, B, C);
		Vect newNormal = temp.getNormal();
		newNormal.setStart(baricenterFrom);

		Vect transBarycenter = newNormal * distance;
		transBarycenter.setStart(baricenterFrom);

		Point newBarycenter = transBarycenter.p2;

		A = intersection(newBarycenter, newNormal, A, ABc);
		B = intersection(newBarycenter, newNormal, B, BBc);
		C = intersection(newBarycenter, newNormal, C, CBc);

		return Triangle(A, B, C).area();
	}

	/**
	 * @brief Devuelve el punto de intersecci�n entre un plano y una recta.
	 * @param plane Punto del plano
	 * @param normal Vector normal del plano
	 * @param line Punto de la recta
	 * @param direction Vector direcci�n de la recta
	 * @return Punto de intersecci�n
	 */
	Point intersection(Point plane, Vect normal, Point line, Vect direction) {
		Vect vectToPlane = Vect(line, plane);

		// OJO: Si el vector direcci�n es paralelo al plano, no hay intersecci�n	
		if (direction * normal == 0) {
			return Point(0, 0, 0);
		}

		double t = (vectToPlane * normal) / (direction * normal);
		Point intersection = line + direction.asPoint() * t;
		return intersection;
	}

	/**
	 * @brief C�lculo de matrices necesarias para el algoritmo de transferencia de energ�a.
	 */
	void energyTrans() {
		int dim = numPlanes * numTriangles;

		Triangle* triangles = new Triangle[dim];

		int k = 0;
		for (int i = 0; i < numPlanes; i++) {
			for (int j = 0; j < numTriangles; j++) {
				triangles[k] = planes[i].triangles[j];
				k++;
			}
		}

		double** distances = new double* [dim];
		double** time = new double* [dim];

		for (int i = 0; i < dim; i++) {
			distances[i] = new double[dim];
			time[i] = new double[dim];
			energyPr[i] = new double[dim];
		}

		for (int i = 0; i < dim; i++) {
			double sumAreas = 0;
			for (int j = 0; j < dim; j++) {
				if (triangles[i].getID() == triangles[j].getID()) {
					distances[i][j] = 0;
					time[i][j] = 0;
					energyPr[i][j] = 0;
				}
				else {
					distances[i][j] = Vect(triangles[i].getBarycenter(), triangles[j].getBarycenter()).length();
					time[i][j] = distances[i][j] / V_SON;
					energyPr[i][j] = solidAngle(triangles[i], triangles[j], 0.2);
					sumAreas += energyPr[i][j];
				}
			}

			for (int j = 0; j < dim; j++) {
				energyPr[i][j] /= sumAreas;
			}
		}


		// Se guardan las matrices en archivos CSV
		std::cout << "Exportando distancias a csv/distances.csv" << std::endl;
		CSV("csv/distances.csv", distances, dim, dim);

		std::cout << "Exportando tiempos a csv/time.csv" << std::endl;
		CSV("csv/time.csv", time, dim, dim);

		std::cout << "Exportando angulos solidos a csv/solidAngles.csv" << std::endl;
		CSV("csv/energyPr.csv", energyPr, dim, dim);

		for (int i = 0; i < dim; i++) {
			delete[] distances[i];
			delete[] time[i];
		}
		delete[] triangles;
		delete[] distances;
		delete[] time;
	}

};

#endif // !ROOM_H
