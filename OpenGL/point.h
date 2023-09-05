#ifndef POINT_H
#define POINT_H

#include <ostream>

/**
 * @class Point
 * @brief Representa un punto en un espacio tridimensional.
 */
class Point {
public:
	double x; /* Coordenada x del punto. */
	double y; /* Coordenada y del punto. */
	double z; /* Coordenada z del punto. */

	/**
	 * @brief Constructor por defecto. Inicializa las coordenadas en (0, 0, 0).
	 */
	Point() : x(0), y(0), z(0) {}

	/**
	 * @brief Constructor que inicializa las coordenadas del punto.
	 * @param xx Coordenada x.
	 * @param yy Coordenada y.
	 * @param zz Coordenada z.
	 */
	Point(double xx, double yy, double zz) : x(xx), y(yy), z(zz) {}

	/**
	 * @brief Constructor de copia.
	 * @param p Punto a copiar.
	 */
	Point(const Point& p) : x(p.x), y(p.y), z(p.z) {}

	/**
	 * @brief Sobrecarga del operador de igualdad.
	 * @param p Punto a comparar.
	 * @return `true` si los puntos son iguales, `false` en caso contrario.
	 */
	bool operator==(const Point& p) const {
		return x == p.x && y == p.y && z == p.z;
	}

	/**
	 * @brief Sobrecarga del operador de desigualdad.
	 * @param p Punto a comparar.
	 * @return `true` si los puntos son diferentes, `false` en caso contrario.
	 */
	bool operator!=(const Point& p) const {
		return !(*this == p);
	}

	/**
	 * @brief Sobrecarga del operador de asignación.
	 * @param p Punto desde el cual se asignarán las coordenadas.
	 * @return Referencia al punto actual.
	 */
	Point& operator=(const Point& p) {
		x = p.x;
		y = p.y;
		z = p.z;
		return *this;
	}

	/**
	 * @brief Sobrecarga del operador de suma.
	 * @param p Punto que se sumará.
	 * @return Nuevo punto resultado de la suma.
	 */
	Point operator+(const Point& p) const {
		return Point(x + p.x, y + p.y, z + p.z);
	}

	/**
	 * @brief Sobrecarga del operador de resta.
	 * @param p Punto que se restará.
	 * @return Nuevo punto resultado de la resta.
	 */
	Point operator-(const Point& p) const {
		return Point(x - p.x, y - p.y, z - p.z);
	}

	/**
	 * @brief Sobrecarga del operador unario de negación.
	 * @return Nuevo punto con las coordenadas negadas.
	 */
	Point operator-() const {
		return Point(-x, -y, -z);
	}

	/**
	 * @brief Sobrecarga del operador de multiplicación por un escalar.
	 * @param f Escalar por el cual se multiplicarán las coordenadas.
	 * @return Nuevo punto con las coordenadas multiplicadas por el escalar.
	 */
	Point operator*(double f) const {
		return Point(x * f, y * f, z * f);
	}

	/**
	 * @brief Sobrecarga del operador de división por un escalar.
	 * @param f Escalar por el cual se dividirán las coordenadas.
	 * @return Nuevo punto con las coordenadas divididas por el escalar.
	 */
	Point operator/(double f) const {
		double inv_f = 1.0 / f;
		return *this * inv_f;
	}

	/**
	 * @brief Sobrecarga del operador de inserción en flujo de salida.
	 * @param os Flujo de salida.
	 * @param p Punto a imprimir.
	 * @return Referencia al flujo de salida.
	 */
	friend std::ostream& operator<<(std::ostream& os, const Point& p) {
		os << "(" << p.x << ", " << p.y << ", " << p.z << ")";
		return os;
	}
};

#endif // !POINT_H
