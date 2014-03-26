#ifndef MAT2X2_HPP
#define MAT2X2_HPP

#include "math/math.hpp"
#include "math/vec2.hpp"

#include <string.h>
#include <ostream>

template <typename T>
struct Mat2x2
{
	/**
	 * NOTE:
	 *
	 * The matrix is stored in column-major order. (The OpenGL-method)
	 *
	 * The following indexes present the access to each of the elements.
	 *
	 * 0 2
	 * 1 3
	 */
	T m[4];

	Mat2x2() { identity(); }
	Mat2x2(T m11, T m12,
	       T m21, T m22)
	{
		set(m11, m12, m21, m22);
	}

	bool operator==(const Mat2x2<T> &rhs) const {
		for (int i=0; i<4; ++i) {
			if (std::abs(m[i] - rhs.m[i]) > EPSILON) {
				return false;
			}
		}
		return true;
	}

	bool operator!=(const Mat2x2<T> &rhs) const {
		return !(*this == rhs);
	}

	bool isIdentity() const {
		Mat2x2<T> tmp;
		return tmp == *this;
	}

	bool isZero() const {
		Mat2x2<T> tmp(0, 0, 0, 0);
		return tmp == *this;
	}

	void set(T m11, T m12,
	         T m21, T m22)
	{
		m[0] = m11;
		m[1] = m21;
		m[2] = m12;
		m[3] = m22;
	}

	void setColumns(const Vec2<T> &c1, const Vec2<T> &c2)
	{
		set(c1.x, c2.x,
		    c1.y, c2.y);
	}

	void setRows(const Vec2<T> &c1, const Vec2<T> &c2)
	{
		set(c1.x, c1.y,
		    c2.x, c2.y);
	}

	void setZero() {
		memset(m, 0, sizeof(T)*4);
	}

	// Matrix multiplication.
	//---------------------------------------------------------------------
	Mat2x2<T> &operator*=(const Mat2x2<T> &b) {
		Mat2x2<T> a(*this);

		m[0] = a.m[0]*b.m[0] + a.m[2]*b.m[1];
		m[1] = a.m[1]*b.m[0] + a.m[3]*b.m[1];

		m[2] = a.m[0]*b.m[2] + a.m[2]*b.m[3];
		m[3] = a.m[1]*b.m[2] + a.m[3]*b.m[3];

		return *this;
	}

	Mat2x2<T> operator*(const Mat2x2<T> &rhs) const {
		return Mat2x2<T>(*this)*=rhs;
	}
	//---------------------------------------------------------------------


	// Scalar multiplication
	//---------------------------------------------------------------------
	Mat2x2<T> &operator*=(const T multiplier) {
		m[0] *= multiplier;
		m[1] *= multiplier;
		m[2] *= multiplier;
		m[3] *= multiplier;

		return *this;
	}

	Mat2x2<T> operator*(const T multiplier) const {
		return Mat2x2<T>(*this)*=multiplier;
	}
	//---------------------------------------------------------------------

	// Vector multiplication
	//---------------------------------------------------------------------
	// multiplication with column vector.
	Vec2<T> operator*(const Vec2<T> &v) const {
		return Vec2<T>(v.x*m[0] + v.y*m[2], v.x*m[1] + v.y*m[3]);
	}
	// multiplication with row vector.
	template <class C>
	friend Vec2<T> operator*(const Vec2<T> &v, const Mat2x2<T> &m);
	//---------------------------------------------------------------------


	// Addition
	//---------------------------------------------------------------------
	Mat2x2<T> &operator+=(const Mat2x2<T> &rhs) {
		m[0] += rhs.m[0];
		m[1] += rhs.m[1];
		m[2] += rhs.m[2];
		m[3] += rhs.m[3];

		return *this;
	}

	Mat2x2<T> operator+(const Mat2x2<T> &rhs) const {
		return Mat2x2<T>(*this)+=rhs;
	}
	//---------------------------------------------------------------------


	// Substraction
	//---------------------------------------------------------------------
	Mat2x2<T> &operator-=(const Mat2x2<T> &rhs) {
		m[0] -= rhs.m[0];
		m[1] -= rhs.m[1];
		m[2] -= rhs.m[2];
		m[3] -= rhs.m[3];

		return *this;
	}

	Mat2x2<T> operator-(const Mat2x2<T> &rhs) const {
		return Mat2x2<T>(*this)-=rhs;
	}
	//---------------------------------------------------------------------


	// text output (for debugging)
	//---------------------------------------------------------------------
	template <class C>
	friend std::ostream& operator<<(std::ostream &s, const Mat2x2<T> &m);
	//---------------------------------------------------------------------

	/**
	 * Transposes this vector.
	 */
	void transpose() {
		T tmp[4];
		memcpy(tmp, m, sizeof(T)*4);

		m[0] = tmp[0];
		m[1] = tmp[2];
		m[2] = tmp[1];
		m[3] = tmp[3];
	}

	/**
	 * Sets this matrix as identity matrix.
	 */
	void identity() {
		setZero();
		m[0] = 1;
		m[1] = 0;
		m[2] = 0;
		m[3] = 1;
	}

	/**
	 * Get the determinant of this matrix.
	 *
	 * @return determimant of the matrix.
	 */
	T getDeterminant() const {
		return Mat2x2<T>::getDeterminant(m[0], m[2], m[1], m[3]);
	}

	/**
	 * Get the determinant for the matrix given.
	 *
	 * @param  m11 ... m33
	 *
	 * @return determinant of the given matrix.
	 */
	static T getDeterminant(float m11, float m12,
	                        float m21, float m22)
	{
		return m11 * m22 - m21 * m12;
	}

	/**
	 * Get adjoinm of the matrix.
	 *
	 * @return adjoint of the matrix.
	 */
	Mat2x2<T> getAdjoint() const {
		return Mat2x2<T> (m[3], -m[2],
		                 -m[1],  m[0]);
	}

	Mat2x2<T> getInverse() const {
		T det = getDeterminant();

		if (std::abs(det) < EPSILON) {
			return Mat2x2<T>();
		}

		Mat2x2<T> inverse(getAdjoint());
		T detInv = ((T)1)/det;
		inverse *= detInv;
		return inverse;
	}

};

template <typename T>
inline Vec2<T> operator*(const Vec2<T> &v, const Mat2x2<T> &m)
{
	Vec2<T> res;

	res.x = m.m[0]*v.x + m.m[1]*v.y;
	res.y = m.m[2]*v.x + m.m[3]*v.y;

	return res;
}

template <typename T>
inline std::ostream &operator<<(std::ostream &out, const Mat2x2<T> &m)
{
	using namespace std;

	out << "| " << m.m[0] << " " << m.m[2] << "|" << endl;
	out << "| " << m.m[1] << " " << m.m[3] << "|" << endl;

	return out;
}

typedef Mat2x2<float>  Mat2x2f;
typedef Mat2x2<double> Mat2x2d;

#endif // MAT2X2_HPP
