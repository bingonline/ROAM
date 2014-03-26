#ifndef MAT3X3_HPP
#define MAT3X3_HPP

#include "math/math.hpp"
#include "math/vec3.hpp"

#include <string.h>
#include <ostream>

template <typename T>
struct Mat3x3
{
	/**
	 * NOTE:
	 *
	 * The matrix is stored in column-major order. (The OpenGL-method)
	 *
	 * The following indexes present the access to each of the elements.
	 *
	 * 0 3 6
	 * 1 4 7
	 * 2 5 8
	 */
	T m[9];

	Mat3x3() { identity(); }
	Mat3x3(T m11, T m12, T m13,
	       T m21, T m22, T m23,
	       T m31, T m32, T m33)
	{
		set(m11, m12, m13,
		    m21, m22, m23,
		    m31, m32, m33);
	}

	bool operator==(const Mat3x3<T> &rhs) const {
		for (int i=0; i<9; ++i) {
			if (std::abs(m[i] - rhs.m[i]) > EPSILON) {
				return false;
			}
		}
		return true;
	}

	bool operator!=(const Mat3x3<T> &rhs) const {
		return !(*this == rhs);
	}

	bool isIdentity() const {
		Mat3x3<T> tmp;
		return tmp == *this;
	}

	bool isZero() const {
		Mat3x3<T> tmp(0, 0, 0, 0, 0, 0, 0, 0, 0);
		return tmp == *this;
	}

	void set(T m11, T m12, T m13,
	         T m21, T m22, T m23,
	         T m31, T m32, T m33)
	{
		m[0] = m11;
		m[1] = m21;
		m[2] = m31;

		m[3] = m12;
		m[4] = m22;
		m[5] = m32;

		m[6] = m13;
		m[7] = m23;
		m[8] = m33;
	}

	void setColumns(const Vec3<T> &c1, const Vec3<T> &c2, const Vec3<T> &c3)
	{
		set(c1.x, c2.x, c3.x,
		    c1.y, c2.y, c3.y,
		    c1.z, c2.z, c3.z);
	}

	void setRows(const Vec3<T> &c1, const Vec3<T> &c2, const Vec3<T> &c3) {
		set(c1.x, c1.y, c1.z,
		    c2.x, c2.y, c2.z,
		    c3.x, c3.y, c3.z);
	}

	void setZero() {
		memset(m, 0, sizeof(T)*9);
	}

	// Matrix multiplication.
	//---------------------------------------------------------------------
	Mat3x3<T> &operator*=(const Mat3x3<T> &b) {
		Mat3x3<T> a(*this);

		m[0] = a.m[0]*b.m[0] + a.m[3]*b.m[1] + a.m[6]*b.m[2];
		m[1] = a.m[1]*b.m[0] + a.m[4]*b.m[1] + a.m[7]*b.m[2];
		m[2] = a.m[2]*b.m[0] + a.m[5]*b.m[1] + a.m[8]*b.m[2];

		m[3] = a.m[0]*b.m[3] + a.m[3]*b.m[4] + a.m[6]*b.m[5];
		m[4] = a.m[1]*b.m[3] + a.m[4]*b.m[4] + a.m[7]*b.m[5];
		m[5] = a.m[2]*b.m[3] + a.m[5]*b.m[4] + a.m[8]*b.m[5];

		m[6] = a.m[0]*b.m[6] + a.m[3]*b.m[7] + a.m[6]*b.m[8];
		m[7] = a.m[1]*b.m[6] + a.m[4]*b.m[7] + a.m[7]*b.m[8];
		m[8] = a.m[2]*b.m[6] + a.m[5]*b.m[7] + a.m[8]*b.m[8];

		return *this;
	}

	Mat3x3<T> operator*(const Mat3x3<T> &rhs) const {
		return Mat3x3<T>(*this)*=rhs;
	}
	//---------------------------------------------------------------------


	// Scalar multiplication
	//---------------------------------------------------------------------
	Mat3x3<T> &operator*=(const T multiplier) {
		m[0] *= multiplier;
		m[1] *= multiplier;
		m[2] *= multiplier;
		m[3] *= multiplier;
		m[4] *= multiplier;
		m[5] *= multiplier;
		m[6] *= multiplier;
		m[7] *= multiplier;
		m[8] *= multiplier;

		return *this;
	}

	Mat3x3<T> operator*(const T multiplier) const {
		return Mat3x3<T>(*this)*=multiplier;
	}
	//---------------------------------------------------------------------

	// Vector multiplication
	//---------------------------------------------------------------------
	// multiplication with column vector.
	Vec3<T> operator*(const Vec3<T> &v) const {
		return Vec3<T>(
			v.x*m[0] + v.y*m[3] + v.z*m[6],
			v.x*m[1] + v.y*m[4] + v.z*m[7],
			v.x*m[2] + v.y*m[5] + v.z*m[8]);
	}
	// multiplication with row vector.
	template <class C>
	friend Vec3<T> operator*(const Vec3<T> &v, const Mat3x3<T> &m);
	//---------------------------------------------------------------------


	// Addition
	//---------------------------------------------------------------------
	Mat3x3<T> &operator+=(const Mat3x3<T> &rhs) {
		m[0] += rhs.m[0];
		m[1] += rhs.m[1];
		m[2] += rhs.m[2];
		m[3] += rhs.m[3];
		m[4] += rhs.m[4];
		m[5] += rhs.m[5];
		m[6] += rhs.m[6];
		m[7] += rhs.m[7];
		m[8] += rhs.m[8];

		return *this;
	}

	Mat3x3<T> operator+(const Mat3x3<T> &rhs) const {
		return Mat3x3<T>(*this)+=rhs;
	}
	//---------------------------------------------------------------------


	// Substraction
	//---------------------------------------------------------------------
	Mat3x3<T> &operator-=(const Mat3x3<T> &rhs) {
		m[0] -= rhs.m[0];
		m[1] -= rhs.m[1];
		m[2] -= rhs.m[2];
		m[3] -= rhs.m[3];
		m[4] -= rhs.m[4];
		m[5] -= rhs.m[5];
		m[6] -= rhs.m[6];
		m[7] -= rhs.m[7];
		m[8] -= rhs.m[8];

		return *this;
	}

	Mat3x3<T> operator-(const Mat3x3<T> &rhs) const {
		return Mat3x3<T>(*this)-=rhs;
	}
	//---------------------------------------------------------------------


	// text output (for debugging)
	//---------------------------------------------------------------------
	template <class C>
	friend std::ostream& operator<<(std::ostream &s, const Mat3x3<T> &m);
	//---------------------------------------------------------------------

	/**
	 * Transposes this vector.
	 */
	void transpose() {
		T tmp[9];
		memcpy(tmp, m, sizeof(T)*9);

		m[0] = tmp[0];
		m[1] = tmp[3];
		m[2] = tmp[6];

		m[3] = tmp[1];
		m[4] = tmp[4];
		m[5] = tmp[7];

		m[6] = tmp[2];
		m[7] = tmp[5];
		m[8] = tmp[8];
	}

	/**
	 * Sets this matrix as identity matrix.
	 */
	void identity() {
		setZero();
		m[0] = 1;
		m[4] = 1;
		m[8] = 1;
	}

	/**
	 * Get the determinant of this matrix.
	 *
	 * @return determimant of the matrix.
	 */
	T getDeterminant() const {
		return Mat3x3<T>::getDeterminant(m[0], m[3], m[6],
		                                 m[1], m[4], m[7],
		                                 m[2], m[5], m[8]);
	}

	/**
	 * Get the determinant for the matrix given.
	 *
	 * @param  m11 ... m33
	 *
	 * @return determinant of the given matrix.
	 */
	static T getDeterminant(float m11, float m12, float m13,
	                        float m21, float m22, float m23,
	                        float m31, float m32, float m33)
	{
		return m11 * (m22*m33 - m23*m32)
		     + m12 * (m23*m31 - m21*m33)
		     + m13 * (m21*m32 - m22*m31);
	}

	/**
	 * Get adjoinm of the matrix.
	 *
	 * @return adjoint of the matrix.
	 */
	Mat3x3<T> getAdjoint() const {
		return Mat3x3<T>(
			m[4]*m[8] - m[7]*m[5],
			m[6]*m[5] - m[3]*m[8],
			m[3]*m[7] - m[6]*m[4],
			m[7]*m[2] - m[1]*m[8],
			m[0]*m[8] - m[6]*m[2],
			m[6]*m[1] - m[0]*m[7],
			m[1]*m[5] - m[4]*m[2],
			m[3]*m[2] - m[0]*m[5],
			m[0]*m[4] - m[3]*m[1]
		);
	}

	Mat3x3<T> getInverse() const {
		T det = getDeterminant();

		if (std::abs(det) < EPSILON) {
			return Mat3x3<T>();
		}

		Mat3x3<T> inverse(getAdjoint());
		T detInv = ((T)1)/det;
		inverse *= detInv;
		return inverse;
	}

};

template <typename T>
inline Vec3<T> operator*(const Vec3<T> &v, const Mat3x3<T> &m)
{
	Vec3<T> res;

	res.x = m.m[0]*v.x + m.m[1]*v.y + m.m[2]*v.z;
	res.y = m.m[3]*v.x + m.m[4]*v.y + m.m[5]*v.z;
	res.z = m.m[6]*v.x + m.m[7]*v.y + m.m[8]*v.z;

	return res;
}

template <typename T>
inline std::ostream &operator<<(std::ostream &out, const Mat3x3<T> &m)
{
	using namespace std;

	out << "| " << m.m[0] << " " << m.m[3] << " " << m.m[6] << "|" << endl;
	out << "| " << m.m[1] << " " << m.m[4] << " " << m.m[7] << "|" << endl;
	out << "| " << m.m[2] << " " << m.m[5] << " " << m.m[8] << "|" << endl;

	return out;
}

typedef Mat3x3<float>  Mat3x3f;
typedef Mat3x3<double> Mat3x3d;

#endif // MAT3X3_HPP
