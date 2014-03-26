#ifndef MAI_MATH_HPP
#define MAI_MATH_HPP

#include <algorithm>
#include <cmath>
#include <limits>

#ifndef M_PI
#	define M_PI 3.14159265f
#endif
#ifndef M_E
#	define M_E  2.71828182f
#endif

// This macro is used for equality and inequality tests.
// you should never actually compare float values and trust it!!
#define EPSILON 0.0001

#define RADIANS_2_DEGREES 57.2957795

#define DEGREES_2_RADIANS 0.0174532925

#endif // MAI_MATH_HPP
