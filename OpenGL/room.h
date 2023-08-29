#ifndef ROOM_H
#define ROOM_H

#include <ostream>
#include "plane.h"
#include "particle.h"
#include "csv.h"

constexpr auto V_SON = 340.0f; /* Constante de la velocidad del sonido en el aire */

/*
 * @brief Clase que representa una habitación
 * @details Una habitación está formada por un conjunto de planos que la delimitan
 */
class Room {
public:
	Plane* planes;		/* Planos que delimitan la habitación */
	int numPlanes;		/* Número de planos que delimitan la habitación */
	int numTriangles;	/* Número de triángulos que forman los planos */

	/**
	 * @brief Constructor de la clase Room
	 * @param nt Número de triángulos que forman los planos
	 * @param np Número de planos que delimitan la habitación
	 */
	Room(int nt, int np) {
		numTriangles = nt;
		numPlanes = np;
		planes = new Plane[numPlanes];

		switch (numPlanes) {
		case 6:
			genCube();
			break;
		}
	}

	/**
	 * @brief Genera una habitación en forma de cubo
	 */
	void genCube() {
		Point* floorPoints = new Point[4]{ {-1, -1, 1}, {-1, -1, -1}, {1, -1, 1} ,{1, -1, -1} };
		Plane floor = Plane(floorPoints, 4, numTriangles, "floor");

		Point* ceilingPoints = new Point[4]{ {-1, 1, 1}, {1, 1, 1}, {-1, 1, -1}, {1, 1, -1} };
		Plane ceiling = { ceilingPoints, 4, numTriangles, "ceiling" };

		Point* leftWallPoints = new Point[4]{ {-1, -1, 1}, {-1, 1, 1}, {-1, -1, -1}, {-1, 1, -1} };
		Plane leftWall = { leftWallPoints, 4, numTriangles, "leftWall" };

		Point* rightWallPoints = new Point[4]{ {1, -1, 1}, {1, -1, -1}, {1, 1, 1}, {1, 1, -1} };
		Plane rightWall = { rightWallPoints, 4, numTriangles, "rightWall" };

		Point* frontWallPoints = new Point[4]{ {-1, -1, 1},{1, -1, 1},{-1, 1, 1}, {1, 1, 1} };
		Plane frontWall = { frontWallPoints, 4, numTriangles, "frontWall" };

		Point* backWallPoints = new Point[4]{ {-1, -1, -1}, {-1, 1, -1}, {1, -1, -1}, {1, 1, -1} };
		Plane backWall = { backWallPoints, 4, numTriangles, "backWall" };

		planes[0] = floor;
		planes[1] = ceiling;
		planes[2] = leftWall;
		planes[3] = rightWall;
		planes[4] = frontWall;
		planes[5] = backWall;
	}

	/**
	 * @brief Devuelve los vectores normales de todos los planos que constituyen la habitación
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
	 * @brief Devuelve los puntos de todos los planos que constituyen la habitación
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
	 * @brief Devuelve el puntero del plano candidato a ser el primero en ser alcanzado por una partícula
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
	 * @brief Controla las colisiones de una partícula con los planos de la habitación en base al método de reflexiones.
	 * @param p Puntero a la partícula
	 */
	void handleParticleCollision(Particle& p) {
		Plane* nearestSurpassed = nearestSurpassedPlane(p.position, p.incidence);
		if (nearestSurpassed != nullptr) {
			//std::cout << p.name << ": collided with " << nearestSurpassed->name << "\t energy: " << p.energy << std::endl;
			Vect normal = nearestSurpassed->getNormal();
			Vect reflex = nearestSurpassed->reflect(p.incidence);
			Point pi = nearestSurpassed->incidence(p.position, p.incidence);
			p.position = pi;
			p.incidence = reflex;
			p.energy -= p.energy * p.loss;
		}
	}

	/**
	 * @brief Devuelve el puntero del plano más cercano a una partícula
	 * @return Puntero al plano más cercano
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
	 * @brief Devuelve los baricentros de los triángulos que constituyen la habitación.
	 * @return Array de puntos
	 */
	std::vector<Point> getBarycenters() {
		std::vector<Point> barycenters;
		for (int i = 0; i < numPlanes; i++) {
			for (int j = 0; j < numTriangles; j++) {
				barycenters.push_back(planes[i].triangles[j].getBarycenter());
			}
		}
		return barycenters;
	}

	/**
	 * @brief Devuelve los triángulos que constituyen la habitación.
	 * @return Array de triángulos
	 */
	std::vector<Triangle> getTriangles() {
		std::vector<Triangle> triangles;
		for (int i = 0; i < numPlanes; i++) {
			for (int j = 0; j < numTriangles; j++) {
				triangles.push_back(planes[i].triangles[j]);
			}
		}
		return triangles;
	}

	/**
	 * @brief Devuelve el ángulo sólido entre dos triángulos a una distancia dada.
	 * @param from Triángulo desde el que se mide el ángulo sólido
	 * @param to Triángulo hasta el que se mide el ángulo sólido
	 * @param distance Distancia entre los triángulos
	 * @return Ángulo sólido
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
	 * @brief Devuelve el punto de intersección entre un plano y una recta.
	 * @param plane Punto del plano
	 * @param normal Vector normal del plano
	 * @param line Punto de la recta
	 * @param direction Vector dirección de la recta
	 * @return Punto de intersección
	 */
	Point intersection(Point plane, Vect normal, Point line, Vect direction) {
		Vect vectToPlane = Vect(line, plane);

		// OJO: Si el vector dirección es paralelo al plano, no hay intersección	
		if (direction * normal == 0) {
			return Point(0, 0, 0);
		}

		double t = (vectToPlane * normal) / (direction * normal);
		Point intersection = line + direction.asPoint() * t;
		return intersection;
	}

	/**
	 * @brief Cálculo de matrices necesarias para el algoritmo de transferencia de energía.
	 */
	void energyTrans() {
		std::vector<Triangle> triangles = getTriangles();

		std::vector<Point> barycenters;
		for (int i = 0; i < triangles.size(); i++) {
			barycenters.push_back(triangles[i].getBarycenter());
		}

		std::vector<std::vector<double>> distances;
		for (int i = 0; i < barycenters.size(); i++) {
			std::vector<double> row;
			for (int j = 0; j < barycenters.size(); j++) {
				if (triangles[i].getID() == triangles[j].getID()) {
					row.push_back(0);
					continue;
				}
				row.push_back(Vect(barycenters[i], barycenters[j]).length());
			}
			distances.push_back(row);
		}

		std::vector<std::vector<double>> time;
		for (int i = 0; i < barycenters.size(); i++) {
			std::vector<double> row;
			for (int j = 0; j < barycenters.size(); j++) {
				if (triangles[i].getID() == triangles[j].getID()) {
					row.push_back(0);
					continue;
				}
				row.push_back(distances[i][j] / V_SON);
			}
			time.push_back(row);
		}

		std::vector<std::vector<double>> energyPr;
		for (int i = 0; i < barycenters.size(); i++) {
			std::vector<double> row;
			double sumAreas = 0;

			for (int j = 0; j < barycenters.size(); j++) {
				if (triangles[i].getID() == triangles[j].getID()) {
					row.push_back(0);
					continue;
				}
				row.push_back(solidAngle(triangles[i], triangles[j], 0.2));
				sumAreas += row[j];
			}

			for (int j = 0; j < barycenters.size(); j++) {
				row[j] /= sumAreas;
			}
			energyPr.push_back(row);
		}

		// Se guardan las matrices en archivos CSV
		CSV("csv/distances.csv", distances);
		CSV("csv/time.csv", time);
		CSV("csv/energyPr.csv", energyPr);
	}

};

#endif // !ROOM_H
