#ifndef MAT4X4_HPP
#define MAT4X4_HPP

#include "math/math.hpp"
#include "math/vec3.hpp"
#include "math/vec4.hpp"

#include <string.h>
#include <ostream>

template <typename T>
struct Mat4x4
{
	/**
	 * NOTE:
	 *
	 * The matrix is stored in column-major order. (The OpenGL-method)
	 *
	 * The following indexes present the access to each of the elements.
	 *
	 * 0 4 8  12
	 * 1 5 9  13
	 * 2 6 10 14
	 * 3 7 11 15
	 */
	T m[16];

	Mat4x4() { identity(); }
	Mat4x4(T m11, T m12, T m13, T m14,
	       T m21, T m22, T m23, T m24,
	       T m31, T m32, T m33, T m34,
	       T m41, T m42, T m43, T m44)
	{
		set(m11, m12, m13, m14,
		    m21, m22, m23, m24,
		    m31, m32, m33, m34,
		    m41, m42, m43, m44);
	}

	bool operator==(const Mat4x4<T> &rhs) const {
		for (int i=0; i<16; ++i) {
			if (std::abs(m[i] - rhs.m[i]) > EPSILON) {
				return false;
			}
		}
		return true;
	}

	bool operator!=(const Mat4x4<T> &rhs) const {
		return !(*this == rhs);
	}

	void set(T m11, T m12, T m13, T m14,
	         T m21, T m22, T m23, T m24,
	         T m31, T m32, T m33, T m34,
	         T m41, T m42, T m43, T m44)
	{
		m[0] = m11;
		m[1] = m21;
		m[2] = m31;
		m[3] = m41;

		m[4] = m12;
		m[5] = m22;
		m[6] = m32;
		m[7] = m42;

		m[8]  = m13;
		m[9]  = m23;
		m[10] = m33;
		m[11] = m43;

		m[12] = m14;
		m[13] = m24;
		m[14] = m34;
		m[15] = m44;
	}

	void setColumns(const Vec4<T> &c1, const Vec4<T> &c2,
	                const Vec4<T> &c3, const Vec4<T> &c4)
	{
		set(c1.x, c2.x, c3.x, c4.x,
		    c1.y, c2.y, c3.y, c4.y,
		    c1.z, c2.z, c3.z, c4.z,
		    c1.w, c2.w, c3.w, c4.w);
	}

	void setRows(const Vec4<T> &r1, const Vec4<T> &r2,
	             const Vec4<T> &r3, const Vec4<T> &r4)
	{
		set(r1.x, r1.y, r1.z, r1.w,
		    r2.x, r2.y, r2.z, r2.w,
		    r3.x, r3.y, r3.z, r3.w,
		    r4.x, r4.y, r4.z, r4.w);
	}

	void setZero() {
		memset(m, 0, sizeof(T)*16);
	}

	bool isIdentity() const {
		Mat4x4<T> tmp;
		return tmp == *this;
	}

	bool isZero() const {
		Mat4x4<T> tmp(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
		return tmp == *this;
	}

	// Matrix multiplication.
	//---------------------------------------------------------------------
	Mat4x4<T> &operator*=(const Mat4x4<T> &b) {
		Mat4x4<T> a(*this);

		m[ 0] = a.m[ 0]*b.m[ 0] + a.m[ 4]*b.m[1]
		      + a.m[ 8]*b.m[ 2] + a.m[12]*b.m[3];
		m[ 1] = a.m[ 1]*b.m[ 0] + a.m[ 5]*b.m[1]
		      + a.m[ 9]*b.m[ 2] + a.m[13]*b.m[3];
		m[ 2] = a.m[ 2]*b.m[ 0] + a.m[ 6]*b.m[1]
		      + a.m[10]*b.m[ 2] + a.m[14]*b.m[3];
		m[ 3] = a.m[ 3]*b.m[ 0] + a.m[ 7]*b.m[1]
		      + a.m[11]*b.m[ 2] + a.m[15]*b.m[3];

		m[ 4] = a.m[ 0]*b.m[ 4] + a.m[ 4]*b.m[5]
		      + a.m[ 8]*b.m[ 6] + a.m[12]*b.m[7];
		m[ 5] = a.m[ 1]*b.m[ 4] + a.m[ 5]*b.m[5]
		      + a.m[ 9]*b.m[ 6] + a.m[13]*b.m[7];
		m[ 6] = a.m[ 2]*b.m[ 4] + a.m[ 6]*b.m[5]
		      + a.m[10]*b.m[ 6] + a.m[14]*b.m[7];
		m[ 7] = a.m[ 3]*b.m[ 4] + a.m[ 7]*b.m[5]
		      + a.m[11]*b.m[ 6] + a.m[15]*b.m[7];

		m[ 8] = a.m[ 0]*b.m[ 8] + a.m[ 4]*b.m[ 9]
		      + a.m[ 8]*b.m[10] + a.m[12]*b.m[11];
		m[ 9] = a.m[ 1]*b.m[ 8] + a.m[ 5]*b.m[ 9]
		      + a.m[ 9]*b.m[10] + a.m[13]*b.m[11];
		m[10] = a.m[ 2]*b.m[ 8] + a.m[ 6]*b.m[ 9]
		      + a.m[10]*b.m[10] + a.m[14]*b.m[11];
		m[11] = a.m[ 3]*b.m[ 8] + a.m[ 7]*b.m[ 9]
		      + a.m[11]*b.m[10] + a.m[15]*b.m[11];

		m[12] = a.m[ 0]*b.m[12] + a.m[ 4]*b.m[13]
		      + a.m[ 8]*b.m[14] + a.m[12]*b.m[15];
		m[13] = a.m[ 1]*b.m[12] + a.m[ 5]*b.m[13]
		      + a.m[ 9]*b.m[14] + a.m[13]*b.m[15];
		m[14] = a.m[ 2]*b.m[12] + a.m[ 6]*b.m[13]
		      + a.m[10]*b.m[14] + a.m[14]*b.m[15];
		m[15] = a.m[ 3]*b.m[12] + a.m[ 7]*b.m[13]
		      + a.m[11]*b.m[14] + a.m[15]*b.m[15];

		return *this;
	}

	Mat4x4<T> operator*(const Mat4x4<T> &rhs) const {
		return Mat4x4<T>(*this)*=rhs;
	}
	//---------------------------------------------------------------------


	// Scalar multiplication
	//---------------------------------------------------------------------
	Mat4x4<T> &operator*=(const T multiplier) {
		m[ 0] *= multiplier;
		m[ 1] *= multiplier;
		m[ 2] *= multiplier;
		m[ 3] *= multiplier;
		m[ 4] *= multiplier;
		m[ 5] *= multiplier;
		m[ 6] *= multiplier;
		m[ 7] *= multiplier;
		m[ 8] *= multiplier;
		m[ 9] *= multiplier;
		m[10] *= multiplier;
		m[11] *= multiplier;
		m[12] *= multiplier;
		m[13] *= multiplier;
		m[14] *= multiplier;
		m[15] *= multiplier;

		return *this;
	}

	Mat4x4<T> operator*(const T multiplier) {
		return Mat4x4<T>(*this)*=multiplier;
	}
	//---------------------------------------------------------------------


	// Vector multiplication
	//---------------------------------------------------------------------
	// Multiplication with column vector.
	Vec4<T> operator*(const Vec4<T> &rhs) const {
		Vec4<T> res;

		res.x = rhs.x*m[0] + rhs.y*m[4] + rhs.z*m[ 8] + rhs.w*m[12];
		res.y = rhs.x*m[1] + rhs.y*m[5] + rhs.z*m[ 9] + rhs.w*m[13];
		res.z = rhs.x*m[2] + rhs.y*m[6] + rhs.z*m[10] + rhs.w*m[14];
		res.w = rhs.x*m[3] + rhs.y*m[7] + rhs.z*m[11] + rhs.w*m[15];

		return res;
	}
	// multplication with row vector.
	template <class C>
	friend Vec4<T> operator*(const Vec4<T> &v, const Mat4x4<T> &m);
	//---------------------------------------------------------------------

	// Addition
	//---------------------------------------------------------------------
	Mat4x4<T> &operator+=(const Mat4x4<T> &rhs) {
		m[ 0] += rhs.m[ 0];
		m[ 1] += rhs.m[ 1];
		m[ 2] += rhs.m[ 2];
		m[ 3] += rhs.m[ 3];
		m[ 4] += rhs.m[ 4];
		m[ 5] += rhs.m[ 5];
		m[ 6] += rhs.m[ 6];
		m[ 7] += rhs.m[ 7];
		m[ 8] += rhs.m[ 8];
		m[ 9] += rhs.m[ 9];
		m[10] += rhs.m[10];
		m[11] += rhs.m[11];
		m[12] += rhs.m[12];
		m[13] += rhs.m[13];
		m[14] += rhs.m[14];
		m[15] += rhs.m[15];

		return *this;
	}

	Mat4x4<T> operator+(const Mat4x4<T> &rhs) const {
		return Mat4x4<T>(*this)+=rhs;
	}
	//---------------------------------------------------------------------

	// Substraction
	//---------------------------------------------------------------------
	Mat4x4<T> &operator-=(const Mat4x4<T> &rhs) {
		m[ 0] -= rhs.m[ 0];
		m[ 1] -= rhs.m[ 1];
		m[ 2] -= rhs.m[ 2];
		m[ 3] -= rhs.m[ 3];
		m[ 4] -= rhs.m[ 4];
		m[ 5] -= rhs.m[ 5];
		m[ 6] -= rhs.m[ 6];
		m[ 7] -= rhs.m[ 7];
		m[ 8] -= rhs.m[ 8];
		m[ 9] -= rhs.m[ 9];
		m[10] -= rhs.m[10];
		m[11] -= rhs.m[11];
		m[12] -= rhs.m[12];
		m[13] -= rhs.m[13];
		m[14] -= rhs.m[14];
		m[15] -= rhs.m[15];

		return *this;
	}

	Mat4x4<T> operator-(const Mat4x4<T> &rhs) const {
		return Mat4x4<T>(*this)-=rhs;
	}
	//---------------------------------------------------------------------


	// text output (for debugging)
	//---------------------------------------------------------------------
	template <class C>
	friend std::ostream& operator<<(std::ostream &s, const Mat4x4<T> &m);
	//---------------------------------------------------------------------


	/**
	 * Transposes this vector.
	 */
	void transpose() {
		T tmp[16];
		memcpy(tmp, m, sizeof(T)*16);

		m[0] = tmp[0];
		m[1] = tmp[4];
		m[2] = tmp[8];
		m[3] = tmp[12];

		m[4] = tmp[1];
		m[5] = tmp[5];
		m[6] = tmp[9];
		m[7] = tmp[13];

		m[8] = tmp[2];
		m[9] = tmp[6];
		m[10] = tmp[10];
		m[11] = tmp[14];

		m[12] = tmp[3];
		m[13] = tmp[7];
		m[14] = tmp[11];
		m[15] = tmp[15];
	}

	/**
	 * Sets this matrix as identity matrix.
	 */
	void identity() {
		setZero();
		m[0] = 1;
		m[5] = 1;
		m[10] = 1;
		m[15] = 1;
	}


	/**
	 * Get the determinant of this matrix.
	 *
	 * @return determimant of the matrix.
	 */
	T getDeterminant() const {
		return Mat4x4<T>::getDeterminant(m[0], m[4], m[8],  m[12],
		                                 m[1], m[5], m[9],  m[13],
		                                 m[2], m[6], m[10], m[14],
		                                 m[3], m[7], m[11], m[15]);
	}

	/**
	 * Get the determinant for the given matrix.
	 *
	 * @param  m11 .. m44
	 *
	 * @return determinant of the given matrix.
	 */
	static T getDeterminant(float m11, float m12, float m13, float m14,
	                        float m21, float m22, float m23, float m24,
	                        float m31, float m32, float m33, float m34,
	                        float m41, float m42, float m43, float m44)
	{
		T a0 = m11*m22 - m21*m12;
		T a1 = m11*m32 - m31*m12;
		T a2 = m11*m42 - m41*m12;
		T a3 = m21*m32 - m31*m22;
		T a4 = m21*m42 - m41*m22;
		T a5 = m31*m42 - m41*m32;
		T b0 = m13*m24 - m23*m14;
		T b1 = m13*m34 - m33*m14;
		T b2 = m13*m44 - m43*m14;
		T b3 = m23*m34 - m33*m24;
		T b4 = m23*m44 - m43*m24;
		T b5 = m33*m44 - m43*m34;

		return a0*b5 - a1*b4 + a2*b3 + a3*b2 - a4*b1 + a5*b0;
	}

	/**
	 * Get adjoint of the matrix.
	 *
	 * @return adjoin of the matrix.
	 */
	Mat4x4<T> getAdjoint() const {
		T a0 = m[ 0]*m[ 5] - m[ 4]*m[ 1];
		T a1 = m[ 0]*m[ 9] - m[ 8]*m[ 1];
		T a2 = m[ 0]*m[13] - m[12]*m[ 1];
		T a3 = m[ 4]*m[ 9] - m[ 8]*m[ 5];
		T a4 = m[ 4]*m[13] - m[12]*m[ 5];
		T a5 = m[ 8]*m[13] - m[12]*m[ 9];

		T b0 = m[ 2]*m[ 7] - m[ 6]*m[ 3];
		T b1 = m[ 2]*m[11] - m[10]*m[ 3];
		T b2 = m[ 2]*m[15] - m[14]*m[ 3];
		T b3 = m[ 6]*m[11] - m[10]*m[ 7];
		T b4 = m[ 6]*m[15] - m[14]*m[ 7];
		T b5 = m[10]*m[15] - m[14]*m[11];

		return Mat4x4<T>(
			+ m[ 5]*b5 - m[ 9]*b4 + m[13]*b3,
			- m[ 4]*b5 + m[ 8]*b4 - m[12]*b3,
			+ m[ 7]*a5 - m[11]*a4 + m[15]*a3,
			- m[ 6]*a5 + m[10]*a4 - m[14]*a3,
			- m[ 1]*b5 + m[ 9]*b2 - m[13]*b1,
			+ m[ 0]*b5 - m[ 8]*b2 + m[12]*b1,
			- m[ 3]*a5 + m[11]*a2 - m[15]*a1,
			+ m[ 2]*a5 - m[10]*a2 + m[14]*a1,
			+ m[ 1]*b4 - m[ 5]*b2 + m[13]*b0,
			- m[ 0]*b4 + m[ 4]*b2 - m[12]*b0,
			+ m[ 3]*a4 - m[ 7]*a2 + m[15]*a0,
			- m[ 2]*a4 + m[ 6]*a2 - m[14]*a0,
			- m[ 1]*b3 + m[ 5]*b1 - m[ 9]*b0,
			+ m[ 0]*b3 - m[ 4]*b1 + m[ 8]*b0,
			- m[ 3]*a3 + m[ 7]*a1 - m[11]*a0,
			+ m[ 2]*a3 - m[ 6]*a1 + m[10]*a0
		);
	}

	Mat4x4<T> getInverse() const {
		T det = getDeterminant();

		if (std::abs(det) < EPSILON) {
			return Mat4x4<T>();
		}

		Mat4x4<T> inverse(getAdjoint());
		T detInv = ((T)1)/det;
		inverse *= detInv;
		return inverse;
	}


	// Affine transformations
	//---------------------------------------------------------------------
	void rotate(const Vec3<T> &axis, T theta) {
		double c = cos(theta);
		double s = sin(theta);
		double t = 1 - c;
		double x = axis.x;
		double y = axis.y;
		double z = axis.z;

		*this *= Mat4x4<T>(t*x*x + c  , t*x*y - s*z, t*x*z + s*y, 0,
		                   t*x*y + s*z, t*y*y + c  , t*y*z - s*x, 0,
		                   t*x*z - s*y, t*y*z + s*x, t*z*z + c  , 0,
		                   0          , 0          , 0          , 1);
	}

	void scale(const Vec3<T> &scale) {
		*this *= Mat4x4<T>(scale.x, 0      , 0      , 0,
		                   0      , scale.y, 0      , 0,
		                   0      , 0      , scale.z, 0,
		                   0      , 0      , 0      , 1);
	}

	void translate(const Vec3<T> &translate) {
		*this *= Mat4x4<T>(1, 0, 0, translate.x,
		                   0, 1, 0, translate.y,
		                   0, 0, 1, translate.z,
		                   0, 0, 0, 1);
	}

	/**
	 * Decompose transformation matrix into separate transformations.
	 *
	 * Expects that rotation is done around axes and the order is ZYX.
	 * values are saved into parameters.
	 *
	 * @param rotation
	 * @param translation
	 * @param scale
	 */
	void decomposeZYX(Vec3<T> &rotation, Vec3<T> &translation, Vec3<T> &scale) const
	{
		Mat4x4<T> copy(*this);

		translation.x = copy.m[12];
		translation.y = copy.m[13];
		translation.z = copy.m[14];

		scale.x = Vec3<T>(copy.m[0], copy.m[4], copy.m[ 8]).length();
		scale.y = Vec3<T>(copy.m[1], copy.m[5], copy.m[ 9]).length();
		scale.z = Vec3<T>(copy.m[2], copy.m[6], copy.m[10]).length();

		T d = copy.getDeterminant();
		if (d < 0) {
			scale.x *= -1;
		}

		// undo the scale.
		Mat4x4<T> s;
		s.scale(scale);
		copy = s.getInverse() * copy;

		/*
		 * ZYX rotation matrix is following, where
		 * A = amount of rotation around X axis
		 * B = ditto                     Y axis
		 * C = ditto                     Z axis
		 *
		 * |  cos(C) * cos(B)       ....              ....      |
		 * | -sin(C) * cos(B)       ....              ....      |
		 * |  sin(B)            cos(B)*-sin(A)    cos(B)*cos(A) |
		 */
		rotation.x = std::atan2(-copy.m[6], copy.m[10]);
		rotation.y = std::asin(  copy.m[2]);
		rotation.z = std::atan2(-copy.m[1], copy.m[0]);
		rotation *= -1;
	}

	/**
	 * Decompose transformation matrix into separate transformations.
	 *
	 * Expects that rotation is done around axes and the order is XYZ.
	 * values are saved into parameters.
	 *
	 * http://www.robertblum.com/articles/2005/02/14/decomposing-matrices
	 *
	 * @param rotation
	 * @param translation
	 * @param scale
	 */
	void decomposeXYZ(Vec3<T> &rotation, Vec3<T> &translation, Vec3<T> &scale) const
	{
		Mat4x4<T> copy(*this);

		translation.x = copy.m[12];
		translation.y = copy.m[13];
		translation.z = copy.m[14];

		scale.x = Vec3<T>(copy.m[0], copy.m[4], copy.m[ 8]).length();
		scale.y = Vec3<T>(copy.m[1], copy.m[5], copy.m[ 9]).length();
		scale.z = Vec3<T>(copy.m[2], copy.m[6], copy.m[10]).length();

		T d = copy.getDeterminant();
		if (d < 0) {
			scale.x *= -1;
		}

		// undo the scale.
		Mat4x4<T> s;
		s.scale(scale);
		copy = s.getInverse() * copy;

		rotation.x = std::atan2(copy.m[9], copy.m[10]);
		rotation.y = std::asin(-copy.m[8]);
		rotation.z = std::atan2(copy.m[4], copy.m[0]);
		rotation *= -1;
	}

};

template <typename T>
inline Vec4<T> operator*(const Vec4<T> &v, const Mat4x4<T> &m)
{
	Vec4<T> res;

	res.x = m.m[0]*v.x  + m.m[1]*v.y  + m.m[2]*v.z  + m.m[3]*v.w;
	res.y = m.m[4]*v.x  + m.m[5]*v.y  + m.m[6]*v.z  + m.m[7]*v.w;
	res.z = m.m[8]*v.x  + m.m[9]*v.y  + m.m[10]*v.z + m.m[11]*v.w;
	res.w = m.m[12]*v.x + m.m[13]*v.y + m.m[14]*v.z + m.m[15]*v.w;

	return res;
}

template <typename T>
inline std::ostream &operator<<(std::ostream &out, const Mat4x4<T> &m)
{
	using namespace std;

	out<<"| "<<m.m[0]<<" "<<m.m[4]<<" "<<m.m[ 8]<<" "<<m.m[12]<< "|"<<endl;
	out<<"| "<<m.m[1]<<" "<<m.m[5]<<" "<<m.m[ 9]<<" "<<m.m[13]<< "|"<<endl;
	out<<"| "<<m.m[2]<<" "<<m.m[6]<<" "<<m.m[10]<<" "<<m.m[14]<< "|"<<endl;
	out<<"| "<<m.m[3]<<" "<<m.m[7]<<" "<<m.m[11]<<" "<<m.m[15]<< "|"<<endl;

	return out;
}

typedef Mat4x4<float>  Mat4x4f;
typedef Mat4x4<double> Mat4x4d;

#endif // MAT4X4_HPP
