#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <ostream>

#include "point.h"
#include "vect.h"

const glm::vec4 DEFAULT_TRIANGLE_COLOR = glm::vec4(0.25, 0.78, 1, 0.1); /* Color por defecto de los triángulos */

/**
 * @brief Clase que representa un triángulo
 * @details Un triángulo se representa mediante tres puntos en el espacio
 * @see Point
 */
class Triangle {
private:
	Point a; /* Punto A del triángulo */
	Point b; /* Punto B del triángulo */
	Point c; /* Punto C del triángulo */
	std::string ID; /* Identificador del triángulo */
	int index; /* Índice del triángulo */
	glm::vec4 color; /* Color del triángulo */
public:
	/**
	 * @brief Constructor por defecto de la clase
	 */
	Triangle() {
		a = Point();
		b = Point();
		c = Point();
	}

	/**
	 * @brief Constructor de la clase
	 * @param aa
	 * @param bb
	 * @param cc
	 */
	Triangle(const Point& aa, const Point& bb, const Point& cc) {
		a = aa;
		b = bb;
		c = cc;
		color = DEFAULT_TRIANGLE_COLOR;
		index = -1;
	}

	/**
	 * @brief Constructor de la clase
	 * @param aa Punto A del triángulo
	 * @param bb Punto B del triángulo
	 * @param cc Punto C del triángulo
	 * @param id Identificador del triángulo
	 * @param col Color del triángulo
	 */
	Triangle(const Point& aa, const Point& bb, const Point& cc, const std::string id) {
		a = aa;
		b = bb;
		c = cc;
		ID = id;
		color = DEFAULT_TRIANGLE_COLOR;
		index = -1;
	}

	/**
	 * @brief Constructor de la clase
	 * @param aa Punto A del triángulo
	 * @param bb Punto B del triángulo
	 * @param cc Punto C del triángulo
	 * @param id Identificador del triángulo
	 * @param col Color del triángulo
	 */
	Triangle(const Point& aa, const Point& bb, const Point& cc, const std::string id, const glm::vec4 col) {
		a = aa;
		b = bb;
		c = cc;
		ID = id;
		color = col;
		index = -1;
	}

	/**
	 * @brief Constructor de copia
	 * @param t
	 */
	Triangle(const Triangle& t) {
		a = t.getA();
		b = t.getB();
		c = t.getC();
		ID = t.getID();
		color = t.getColor();
		index = t.getIndex();
	}

	Point getA() const { return a; } /* Devuelve el punto A del triángulo */
	Point getB() const { return b; } /* Devuelve el punto B del triángulo */
	Point getC() const { return c; } /* Devuelve el punto C del triángulo */
	std::string getID() const { return ID; } /* Devuelve el identificador del triángulo */
	glm::vec4 getColor() const { return color; } /* Devuelve el color del triángulo */
	int getIndex() const { return index; } /* Devuelve el índice del triángulo */

	void setA(const Point& aa) { a = aa; } /* Configura el punto A del triángulo */
	void setB(const Point& bb) { b = bb; } /* Configura el punto B del triángulo */
	void setC(const Point& cc) { c = cc; } /* Configura el punto C del triángulo */
	void setID(const std::string id) { ID = id; } /* Configura el identificador del triángulo */
	void setColor(const glm::vec4 col) { color = col; } /* Configura el color del triángulo */
	void setIndex(const int i) { index = i; } /* Configura el índice del triángulo */

	/**
	 * @brief Sobrecarga del operador de igualdad.
	 * @param t triángulo a comparar.
	 * @return `true` si los triángulos son iguales, `false` en caso contrario.
	 */
	int operator==(const Triangle& t) const {
		return a == t.getA() && b == t.getB() && c == t.getC() ? 1 : 0;
	}

	/**
	 * @brief Sobrecarga del operador de desigualdad.
	 * @param t triángulo a comparar.
	 * @return `true` si los triángulos son distintos, `false` en caso contrario.
	 */
	bool operator!=(const Triangle& t) const {
		return a != t.getA() || b != t.getB() || c != t.getC() ? 1 : 0;
	}

	/**
	 * @brief Sobrecarga del operador de asignación.
	 * @param t triángulo a asignar.
	 * @return Referencia al triángulo.
	 */
	Triangle& operator=(const Triangle& t) {
		a = t.getA();
		b = t.getB();
		c = t.getC();
		ID = t.getID();
		color = t.getColor();
		index = t.getIndex();
		return *this;
	}

	/**
	 * @brief Devuelve el vector normal del triángulo
	 * @return Vect
	 */
	Vect getNormal() const {
		Vect v1(a, b);
		Vect v2(a, c);
		Point n = (v1 ^ v2).unit().asPoint();
		Point start = getBarycenter();
		return Vect(start, start + n);
	}


	/**
	 * @brief Devuelve el baricentro del triángulo
	 * @return Point
	 */
	Point getBarycenter() const {
		return (a + b + c) / 3.0;
	}

	/**
	 * @brief Comprueba si un punto está dentro del triángulo
	 * @param p
	 * @return int
	 */
	int isInside(const Point& p) const {
		Vect v1(a, b);
		Vect v2(a, c);
		Vect v3(a, p);
		Vect n = v1 ^ v2;
		double d = n * v3;
		return d > 0 ? 1 : 0;
	}

	/**
	 * @brief Área del triángulo
	 * @return double
	 */
	double area() const {
		Vect v1(a, b);
		Vect v2(a, c);
		Vect n = v1 ^ v2;
		return n.length() / 2.0;
	}

	bool isPointInside(const Point& p) const {
		Vect v1(a, b);
		Vect v2(a, c);
		Vect v3(a, p);
		Vect n = v1 ^ v2;
		double d = n * v3;
		return d > 0 ? true : false;
	}

	/**
	 * @brief Devuelve un array de 9 doubles con los valores de los puntos del triangulo
	 * @return double*
	 */
	double* flatArray() {
		double* res = new double[9] {a.x, a.y, a.z, b.x, b.y, b.z, c.x, c.y, c.z};
		return res;
	}

	/**
	 * @brief Sobrecarga del operador de inserción en flujo de salida.
	 * @param os Flujo de salida.
	 * @param t triángulo a imprimir.
	 * @return Referencia al flujo de salida.
	 */
	friend std::ostream& operator<<(std::ostream& os, const Triangle& t) {
		os << "Polygon(" << t.getA() << "," << t.getB() << "," << t.getC() << "," << t.getA() << ")";
		return os;
	}
};

#endif // TRIANGLE_H
