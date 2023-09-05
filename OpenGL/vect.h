#ifndef VECT_H
#define VECT_H

#include <ostream>
#include "point.h"

/**
 * @class Vect
 * @brief Representa un vector en el espacio tridimensional.
 */
class Vect {
public:
	Point p1; /* Punto inicial del vector */
	Point p2; /* Punto final del vector */

	/**
	 * @brief Constructor por defecto.
	*/
	Vect() {
		p1 = Point();
		p2 = Point();
	}

	/**
	 * @brief Vector dado un punto. Se toma como punto final el origen.
	 * @param p Punto final del vector.
	 */
	Vect(const Point& p) {
		p1 = Point();
		p2 = p;
	}

	/**
	 * @brief Vector dado dos puntos.
	 * @param pp1 Punto inicial del vector.
	 * @param pp2 Punto final del vector.
	 */
	Vect(const Point& pp1, const Point& pp2) {
		p1 = pp1;
		p2 = pp2;
	}

	/**
	 * @brief Constructor de copia.
	 * @param v Vector a copiar.
	 */
	Vect(const Vect& v) {
		p1 = v.p1;
		p2 = v.p2;
	}

	/**
	 * @brief Destructor por defecto.
	 */
	~Vect() {}

	double getI() const { return p2.x - p1.x; } /* Devuelve la componente i del vector */
	double getJ() const { return p2.y - p1.y; } /* Devuelve la componente j del vector */
	double getK() const { return p2.z - p1.z; } /* Devuelve la componente k del vector */

	/**
	 * @brief Cambia el punto inicial del vector. El punto final se actualiza en consecuencia.
	 * @param p Punto inicial del vector.
	 */
	void setStart(const Point& p) {
		p2 = p + asPoint();
		p1 = p;
	}

	/**
	 * @brief Sobrecarga del operador de igualdad.
	 * @param v Vector a comparar.
	 * @return Devuelve `true` si los vectores son iguales, `false` en caso contrario.
	 */
	bool operator==(const Vect& v) const {
		return p1 == v.p1 && p2 == v.p2 ? 1 : 0;
	}

	/**
	 * @brief Sobrecarga del operador de desigualdad.
	 * @param v Vector a comparar.
	 * @return Devuelve `true` si los vectores son distintos, `false` en caso contrario.
	 */
	bool operator!=(const Vect& v) const {
		return p1 != v.p1 || p2 != v.p2 ? 1 : 0;
	}

	/**
	 * @brief Sobrecarga del operador de asignación.
	 * @param v Vector a asignar.
	 * @return Devuelve el vector asignado.
	*/
	Vect& operator=(const Vect& v) {
		p1 = v.p1;
		p2 = v.p2;
		return *this;
	}

	/**
	 * @brief Sobrecarga del operador de suma.
	 * @param v Vector a sumar.
	 * @return Devuelve el vector suma.
	*/
	Vect operator+(const Vect& v) const {
		return Vect(p1 + v.p1, p2 + v.p2);
	}

	/**
	 * @brief Sobrecarga del operador de resta.
	 * @param v Vector a restar.
	 * @return Devuelve el vector resta.
	*/
	Vect operator-(const Vect& v) const {
		return Vect(p1 - v.p1, p2 - v.p2);
	}

	/**
	 * @brief Sobrecarga del operador de resta unario.
	 * @return Devuelve el vector opuesto.
	*/
	Vect operator-() const {
		return Vect(p2, p1);
	}

	/**
	 * @brief Sobrecarga del operador de multiplicación por un escalar.
	 * @param f Escalar a multiplicar.
	 * @return Devuelve el vector multiplicado por el escalar.
	*/
	Vect operator*(double f) const {
		return Vect(p1 * f, p2 * f);
	}

	/**
	 * @brief Sobrecarga del operador de división por un escalar.
	 * @param f Escalar a dividir.
	 * @return Devuelve el vector dividido por el escalar.
	*/
	Vect operator/(double f) const {
		return Vect(p1 / f, p2 / f);
	}

	/**
	 * @brief Sobrecarga del operador de producto escalar o producto punto.
	 * @param v Vector a multiplicar.
	 * @return Devuelve el producto escalar.
	*/
	double operator*(const Vect& v) const {
		Point u = asPoint();
		Point V = v.asPoint();
		double dotx = u.x * V.x;
		double doty = u.y * V.y;
		double dotz = u.z * V.z;
		return dotx + doty + dotz;
	}

	/**
	 * @brief Sobrecarga del operador de producto vectorial o producto cruz.
	 * @param v Vector a multiplicar.
	 * @return Devuelve el producto vectorial.
	 */
	Vect operator^(const Vect& v) const {
		Point u = asPoint();
		Point V = v.asPoint();
		double crossx = u.y * V.z - u.z * V.y;
		double crossy = u.z * V.x - u.x * V.z;
		double crossz = u.x * V.y - u.y * V.x;
		return Vect(Point(crossx, crossy, crossz));
	}

	/**
	 * @brief Transforma el vector en un punto.
	 * @return Devuelve un punto con las mismas coordenadas que el vector.
	 */
	Point asPoint() const {
		return p2 - p1;
	}

	/**
	 * @brief Devuelve la longitud, magnitud o norma del vector.
	 * @return Devuelve la longitud del vector.
	 */
	double length() const {
		return sqrt(pow(getI(), 2) + pow(getJ(), 2) + pow(getK(), 2));
	}

	Vect perpendicular() const {
		Point start = p1;
		Vect r = Vect(Point(-getJ(), getI(), 0));
		r.setStart(start);
		return r;
	}

	/**
	 * @brief Devuelve el vector unitario.
	 * @return Devuelve el vector unitario.
	 */
	Vect unit() const {
		return Vect(p1 / length(), p2 / length());
	}

	/**
	 * @brief Devuelve el ángulo entre dos vectores.
	 * @param v Vector a comparar.
	 * @return Ángulo entre dos vectores.
	 */
	double angle(const Vect& v) const {
		return acos((*this * v) / (length() * v.length()));
	}

	/**
	 * @brief Devuelve el vector proyección de un vector sobre otro.
	 * @param v Vector a proyectar.
	 * @return Vector proyección.
	 */
	Vect proy(const Vect& v) const {
		return v.unit() * ((*this * v) / v.length());
	}

	/**
	 * @brief Devuelve el vector ortogonal de un vector sobre otro.
	 * @param v Vector a ortogonalizar.
	 * @return Vector ortogonal.
	 */
	Vect norm(const Vect& v) const {
		return *this - proy(v);
	}

	/**
	 * @brief Sobrecarga del operador de inserción en flujo de salida.
	 * @param os Flujo de salida.
	 * @param v Vector a insertar.
	 * @return Referencia al flujo de salida.
	 */
	friend std::ostream& operator<<(std::ostream& os, const Vect& v) {
		os << "Vector(" << v.p1 << "," << v.p2 << ")";
		return os;
	}

	/**
	 * @brief Rota un vector alrededor de un eje en un ángulo theta usando la fórmula de Rodrigues.
	 * @param k Vector eje. El vector puede o no ser unitario debido a que se normaliza.
	 * @param v Vector a rotar (no necesita ser unitario).
	 * @param theta Ángulo de rotación en radianes.
	 * @return Vector rotado.
	 *
	 * @note https://en.wikipedia.org/wiki/Rodrigues%27_rotation_formula
	 */
	static Vect rodriges(Vect k, Vect v, double theta) {
		k = k.unit();
		Vect r1 = v * cos(theta);
		Vect r2 = (k ^ v) * sin(theta);
		Vect r3 = k * (k * v) * (1 - cos(theta));
		return r1 + r2 + r3;
	}


	/**
	 * @brief [Deprecated] Rota un vector alrededor de un eje dado un ángulo theta.
	 * @param u Vector eje.
	 * @param v Vector a rotar.
	 * @param theta Ángulo de rotación.
	 * @return Vector rotado.
	 *
	 */
	static Vect rotate(Vect u, Vect v, double theta) {
		u = u.unit();
		Vect r1 = u * (u * v);
		Vect r2 = ((u ^ v) * cos(theta)) ^ u;
		Vect r3 = (u ^ v) * sin(theta);
		return r1 + r2 + r3;
	}
};

#endif // !VECT_H
