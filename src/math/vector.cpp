#include <feign/math/vector.h>

// Vector3f
Vector3f::Vector3f() : x(0.0), y(0.0), z(0.0) { }

Vector3f::Vector3f(Float c) : x(c), y(c), z(c) { }

Vector3f::Vector3f(Float x, Float y, Float z) : x(x), y(y), z(z) { }

// Point3f
Point3f::Point3f() : x(0.0), y(0.0), z(0.0) { }

Point3f::Point3f(Float c) : x(c), y(c), z(c) { }

Point3f::Point3f(Float x, Float y, Float z) : x(x), y(y), z(z) { }

// Normal3f
Normal3f::Normal3f() : x(0.0), y(0.0), z(0.0) { }

Normal3f::Normal3f(Float c) : x(c), y(c), z(c) { }

Normal3f::Normal3f(Float x, Float y, Float z) : x(x), y(y), z(z) { }
