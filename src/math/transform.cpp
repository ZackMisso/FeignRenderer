#include <feign/math/transform.h>

////////////////////////

Transform::Transform() :
                  mat(Matrix4f::identity()),
                  inv(Matrix4f::identity())
{
    // does nothing
}

Transform::Transform(const Matrix4f& matrix) :
                  mat(matrix),
                  inv(~matrix)
{
    // does nothing
}

Transform::Transform(const Matrix4f& matrix,
                     const Matrix4f& inverse) :
                 mat(matrix),
                 inv(inverse)
{
    // does nothing
}

Transform Transform::inverse() const
{
    return Transform(inv, mat);
}

Transform Transform::operator*(const Transform& other) const
{
    Matrix4f newMat = mat * other.mat;
    Matrix4f newInv = other.inv * inv;
    return Transform(newMat, newInv);
}

Vector3f Transform::operator*(const Vector3f& other) const
{
    return mat.topLeftCorner() * other;
}

Normal3f Transform::operator*(const Normal3f& other) const
{
    // ! is transpose
    return !(inv.topLeftCorner()) * other;
}

Point3f Transform::operator*(const Point3f& other) const
{
    Vec4f hom = mat * Vec4f(other, 1.f);
    Vector3f hom3 = hom.head() / hom(3);
    return Point3f(hom3(0), hom3(1), hom3(2));
}

Ray3f Transform::operator*(const Ray3f& other) const
{
    return Ray3f(operator*(other.origin),
                 operator*(other.dir),
                 other.mint,
                 other.maxt);
}

void Transform::print() const
{
    std::cout << "Transform: " << std::endl;
    std::cout << mat(0) << " " << mat(1) << " " << mat(2) << " " << mat(3) << std::endl;
    std::cout << mat(4) << " " << mat(5) << " " << mat(6) << " " << mat(7) << std::endl;
    std::cout << mat(8) << " " << mat(9) << " " << mat(10) << " " << mat(11) << std::endl;
    std::cout << mat(12) << " " << mat(13) << " " << mat(14) << " " << mat(15) << std::endl;
    std::cout << "Transform Inverse: " << std::endl;
    std::cout << inv(0) << " " << inv(1) << " " << inv(2) << " " << inv(3) << std::endl;
    std::cout << inv(4) << " " << inv(5) << " " << inv(6) << " " << inv(7) << std::endl;
    std::cout << inv(8) << " " << inv(9) << " " << inv(10) << " " << inv(11) << std::endl;
    std::cout << inv(12) << " " << inv(13) << " " << inv(14) << " " << inv(15) << std::endl;
    std::cout << std::endl;
}

const Matrix4f& Transform::getMatrix() const
{
    return mat;
}

const Matrix4f& Transform::getInverse() const
{
    return inv;
}
