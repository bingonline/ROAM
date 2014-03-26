#ifndef VEC4_HPP
#define VEC4_HPP

#include "math/math.hpp"

#include <ostream>

/**
 * 4D column vector.
 */
template <typename T>
struct Vec4
{
	T x;
	T y;
	T z;
	T w;

	Vec4() : x(0), y(0), z(0), w(0) {}
	Vec4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}

	Vec4<T> operator- () const {
		return Vec4<T>(-x, -y, -z, -w);
	}

	bool operator==(const Vec4<T> &rhs) const {
		return std::abs(x - rhs.x) < EPSILON &&
		       std::abs(y - rhs.y) < EPSILON &&
		       std::abs(z - rhs.z) < EPSILON &&
		       std::abs(w - rhs.w) < EPSILON;
	}

	bool operator!=(const Vec4<T> &rhs) const {
		return std::abs(x - rhs.x) > EPSILON ||
		       std::abs(y - rhs.y) > EPSILON ||
		       std::abs(z - rhs.z) > EPSILON ||
		       std::abs(w - rhs.w) > EPSILON;
	}

	T operator[](int i) const {
		switch (i) {
		case 0: return x;
		case 1: return y;
		case 2: return z;
		case 3: return w;
		default: return std::numeric_limits<T>::max();
		}
	}

	void setZero() {
		x = 0;
		y = 0;
		z = 0;
		w = 0;
	}

	bool isZero() {
		return x == 0 && y == 0 && z == 0 && w == 0;
	}

	void normalize() {
		float inv = 1.0 / length();
		x *= inv;
		y *= inv;
		z *= inv;
		w *= inv;
	}

	void set(T x, T y, T z, T w) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	/** Returns the depth of the vector. */
	int depth() {
		return 4;
	}

	// Addition
	//---------------------------------------------------------------------
	Vec4<T> &operator+=(const Vec4<T> &rhs) {
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		w += rhs.w;
		return *this;
	}

	Vec4<T> operator+(const Vec4<T> &rhs) const {
		return Vec4<T>(*this)+=rhs;
	}
	//---------------------------------------------------------------------


	// Substraction
	//---------------------------------------------------------------------
	Vec4<T> &operator-=(const Vec4<T> &rhs) {
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		w -= rhs.w;
		return *this;
	}

	Vec4<T> operator-(const Vec4<T> &rhs) const {
		return Vec4<T>(*this)-=rhs;
	}
	//---------------------------------------------------------------------


	// Multiplication
	//---------------------------------------------------------------------
	// with another vector.
	Vec4<T> &operator*=(const Vec4<T> &rhs) {
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;
		w *= rhs.w;
		return *this;
	}

	Vec4<T> operator*(const Vec4<T> &rhs) const {
		return Vec4<T>(*this)*=rhs;
	}

	// with scalar.
	Vec4<T> &operator*=(T factor) {
		 x *= factor;
		 y *= factor;
		 z *= factor;
		 w *= factor;
		 return *this;
	}

	Vec4<T> operator*(T factor) const {
		return Vec4<T>(*this)*=(factor);
	}

	// makes possible to write scalar * vector.
	template <class C>
	friend Vec4<T> operator*(T factor, const Vec4<T> &v);
	// specialized for double, if float is not enuff.
	template <class C>
	friend Vec4<T> operator*(double factor, const Vec4<T> &v);
	//---------------------------------------------------------------------


	// Division
	//---------------------------------------------------------------------
	Vec4<T> &operator/=(const Vec4<T> &rhs) {
		x /= rhs.x;
		y /= rhs.y;
		z /= rhs.z;
		w /= rhs.w;
		return *this;
	}

	Vec4<T> operator/(const Vec4<T> &rhs) const {
		return Vec4<T>(*this)/=rhs;
	}

	Vec4<T> &operator/=(T factor) {
		 x /= factor;
		 y /= factor;
		 z /= factor;
		 w /= factor;
		 return *this;
	}

	Vec4<T> operator/(T factor) const {
		return Vec4<T>(*this)/=(factor);
	}
	//---------------------------------------------------------------------


	// text output (for debugging)
	//---------------------------------------------------------------------
	template <class C>
	friend std::ostream& operator<<(std::ostream &s, const Vec4<T> &v);
	//---------------------------------------------------------------------

	/**
	 * Returns the length of the vector.
	 *
	 * @return length of the vector.
	 */
	T length() const {
		return sqrt(x*x + y*y + z*z + w*w);
	}

	/**
	 * Returns the length of the vector squared.
	 * Use this for perfomance.
	 *
	 * @return length of the vector squared.
	 */
	T lengthSquared() const {
		return x*x + y*y + z*z + w*w;
	}

	/**
	 * Returns a merged Vec4<T> from the min values from a and b.
	 *
	 * @param a
	 * @param b
	 *
	 * @returns merged min Vec4<T>
	 */
	static Vec4<T> min(const Vec4<T> &a, const Vec4<T> &b) {
		return Vec4<T>(
			std::min(a.x, b.x),
			std::min(a.y, b.y),
			std::min(a.z, b.z),
			std::min(a.w, b.w));
	}

	/**
	 * Returns a merged Vec4<T> from the max values from a and b.
	 *
	 * @param a
	 * @param b
	 *
	 * @returns merged max Vec4<T>
	 */
	static Vec4<T> max(const Vec4<T> &a, const Vec4<T> &b) {
		return Vec4<T>(
			std::max(a.x, b.x),
			std::max(a.y, b.y),
			std::max(a.z, b.z),
			std::max(a.w, b.w));
	}

	T dot(const Vec4<T> &rhs) const {
		return x*rhs.x + y*rhs.y + z*rhs.z + w*rhs.w;
	}

	static T dot(const Vec4<T> &a, const Vec4<T> &b) {
		return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
	}

};

template <typename T>
inline Vec4<T> operator*(T factor, const Vec4<T> &v)
{
	return Vec4<T>(factor * v.x, factor * v.y, factor * v.z, factor * v.w);
}

template <typename T>
inline Vec4<T> operator*(double factor, const Vec4<T> &v)
{
	return Vec4<T>(factor * v.x, factor * v.y, factor * v.z, factor * v.w);
}

template <typename T>
inline std::ostream& operator<<(std::ostream &out, const Vec4<T> &v)
{
	out << "{ " << v.x << ", " << v.y << ", " << v.z << ", " << v.w << " }";
	return out;
}

typedef Vec4<float>  Vec4f;
typedef Vec4<int>    Vec4i;
typedef Vec4<double> Vec4d;

#endif // VEC4_HPP
