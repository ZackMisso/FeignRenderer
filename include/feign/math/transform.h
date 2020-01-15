/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/common.h>
#include <feign/math/ray.h>
#include <feign/math/vector.h>

class Transform
{
public:
    Transform();
    Transform(const Matrix4f& matrix);
    Transform(const Matrix4f& matrix,
              const Matrix4f& inverse);

    // operators
    Transform operator*(const Transform& other) const;
    Vector3f operator*(const Vector3f& other) const;
    Normal3f operator*(const Normal3f& other) const;
    Point3f operator*(const Point3f& other) const;
    Ray3f operator*(const Ray3f& other) const;

    void operator*=(const Transform& other);

    // methods
    Transform inverse() const;
    void print() const;

    const Matrix4f& getMatrix() const;
    const Matrix4f& getInverse() const;

    // static Transform identity();

protected:
    Matrix4f mat;
    Matrix4f inv;
};
