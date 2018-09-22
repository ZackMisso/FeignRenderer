#include <feign/math/transform.h>

// Node Implementation //

TransformNode::TransformNode() : Node()
{
    // TODO
}

TransformNode::TransformNode(Node* parent) : Node(parent)
{
    // TODO
}

TransformNode::~TransformNode()
{
    // TODO
}

void TransformNode::preProcess()
{
    throw new NotImplementedException("preprocess transf");

    preProcessChildren();
}

string TransformNode::getName() const
{
    return "transform";
}

NodeType TransformNode::getNodeType() const
{
    return NT_Transform;
}

////////////////////////

Transform::Transform() :
                  mat(Matrix4f_identity()),
                  inv(Matrix4f_identity())
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

const Matrix4f& Transform::getMatrix() const
{
    return mat;
}

const Matrix4f& Transform::getInverse() const
{
    return inv;
}
