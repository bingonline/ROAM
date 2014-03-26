#ifndef VEC3_HPP
#define VEC3_HPP

#include "math/math.hpp"

#include <ostream>

/**
 * 3D column vector.
 */
template <typename T>
struct Vec3
{
	T x;
	T y;
	T z;

	Vec3() : x(0), y(0), z(0) {}
	Vec3(T x, T y, T z) : x(x), y(y), z(z) {}

	Vec3<T> operator- () const {
		return Vec3<T>(-x, -y, -z);
	}

	bool operator==(const Vec3<T> &rhs) const {
		return std::abs(x - rhs.x) < EPSILON &&
		       std::abs(y - rhs.y) < EPSILON &&
		       std::abs(z - rhs.z) < EPSILON;
	}

	bool operator!=(const Vec3<T> &rhs) const {
		return std::abs(x - rhs.x) > EPSILON ||
		       std::abs(y - rhs.y) > EPSILON ||
		       std::abs(z - rhs.z) > EPSILON;
	}

	T operator[](int i) const {
		switch (i) {
		case 0: return x;
		case 1: return y;
		case 2: return z;
		default: return std::numeric_limits<T>::max();
		}
	}

	void setZero() {
		x = 0;
		y = 0;
		z = 0;
	}

	bool isZero() {
		return x == 0 && y == 0 && z == 0;
	}

	void normalize() {
		float inv = 1.0 / length();
		x *= inv;
		y *= inv;
		z *= inv;
	}

	void set(T x, T y, T z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	/** Returns the depth of the vector */
	int depth() {
		return 3;
	}

	// Addition
	//---------------------------------------------------------------------
	Vec3<T> &operator+=(const Vec3<T> &rhs) {
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}

	Vec3<T> operator+(const Vec3<T> &rhs) const {
		return Vec3<T>(*this)+=rhs;
	}
	//---------------------------------------------------------------------


	// Substraction
	//---------------------------------------------------------------------
	Vec3<T> &operator-=(const Vec3<T> &rhs) {
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}

	Vec3<T> operator-(const Vec3<T> &rhs) const {
		return Vec3<T>(*this)-=rhs;
	}
	//---------------------------------------------------------------------


	// Multiplication
	//---------------------------------------------------------------------
	// with another vector.
	Vec3<T> &operator*=(const Vec3<T> &rhs) {
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;
		return *this;
	}

	Vec3<T> operator*(const Vec3<T> &rhs) const {
		return Vec3<T>(*this)*=rhs;
	}

	// with a scalar.
	Vec3<T> &operator*=(T factor) {
		 x *= factor;
		 y *= factor;
		 z *= factor;
		 return *this;
	}

	Vec3<T> operator*(T factor) const {
		return Vec3<T>(*this)*=(factor);
	}

	// makes possible to write scalar * vector.
	template <class C>
	friend Vec3<T> operator*(T factor, const Vec3<T> &v);
	// specialized for double, if float is not enuff.
	template <class C>
	friend Vec3<T> operator*(double factor, const Vec3<T> &v);
	//---------------------------------------------------------------------


	// Division
	//---------------------------------------------------------------------
	Vec3<T> &operator/=(const Vec3<T> &rhs) {
		x /= rhs.x;
		y /= rhs.y;
		z /= rhs.z;
		return *this;
	}

	Vec3<T> operator/(const Vec3<T> &rhs) const {
		return Vec3<T>(*this)/=rhs;
	}

	Vec3<T> &operator/=(T factor) {
		 x /= factor;
		 y /= factor;
		 z /= factor;
		 return *this;
	}

	Vec3<T> operator/(T factor) const {
		return Vec3<T>(*this)/=(factor);
	}
	//---------------------------------------------------------------------


	// text output (for debugging)
	//---------------------------------------------------------------------
	template <class C>
	friend std::ostream& operator<<(std::ostream &s, const Vec3<T> &v);
	//---------------------------------------------------------------------

	/**
	 * Returns the length of the vector.
	 *
	 * @return length of the vector.
	 */
	T length() const {
		return sqrt(x*x + y*y + z*z);
	}

	/**
	 * Returns the length of the vector squared.
	 * Use this for perfomance.
	 *
	 * @return length of the vector squared.
	 */
	T lengthSquared() const {
		return x*x + y*y + z*z;
	}

	/**
	 * Returns a merged Vec3<T> from the min values from a and b.
	 *
	 * @param a
	 * @param b
	 *
	 * @returns merged min Vec3<T>
	 */
	static Vec3<T> min(const Vec3<T> &a, const Vec3<T> &b) {
		return Vec3<T>(
			std::min(a.x, b.x),
			std::min(a.y, b.y),
			std::min(a.z, b.z));
	}

	/**
	 * Returns a merged Vec3<T> from the max values from a and b.
	 *
	 * @param a
	 * @param b
	 *
	 * @returns merged max Vec3<T>
	 */
	static Vec3<T> max(const Vec3<T> &a, const Vec3<T> &b) {
		return Vec3<T>(
			std::max(a.x, b.x),
			std::max(a.y, b.y),
			std::max(a.z, b.z));
	}

	T dot(const Vec3<T> &rhs) const {
		return x*rhs.x + y*rhs.y + z*rhs.z;
	}

	static T dot(const Vec3<T> &a, const Vec3<T> &b) {
		return a.x*b.x + a.y*b.y + a.z*b.z; 
	}

	Vec3<T> cross(const Vec3<T> &rhs) const {
		return Vec3<T>(
			y*rhs.z - z*rhs.y,
			z*rhs.x - x*rhs.z,
			x*rhs.y - y*rhs.x);
	}

};

template <typename T>
inline Vec3<T> operator*(T factor, const Vec3<T> &v)
{
	return Vec3<T>(factor * v.x, factor * v.y, factor * v.z);
}

template <typename T>
inline Vec3<T> operator*(double factor, const Vec3<T> &v)
{
	return Vec3<T>(factor * v.x, factor * v.y, factor * v.z);
}

template <typename T>
inline std::ostream& operator<<(std::ostream &out, const Vec3<T> &v)
{
	out << "{ " << v.x << ", " << v.y << ", " << v.z << " }";
	return out;
}

typedef Vec3<float>  Vec3f;
typedef Vec3<int>    Vec3i;
typedef Vec3<double> Vec3d;

#endif // VEC3_HPP
