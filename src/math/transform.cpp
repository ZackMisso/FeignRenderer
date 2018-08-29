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

string TransformNode::getName() const
{
    return "transform";
}

NodeType TransformNode::getNodeType() const
{
    return NT_Transform;
}

////////////////////////

// Transform::Transform() :
//                   mat(Eigen::Matrix4f::Identity()),
//                   inv(Eigen::Matrix4f::Identity())
// {
//     // does nothing
// }
//
// Transform::Transform(const Eigen::Matrix4f& matrix) :
//                   mat(matrix),
//                   inv(matrix.inverse())
// {
//     // does nothing
// }
//
// Transform::Transform(const Eigen::Matrix4f& matrix,
//                      const Eigen::Matrix4f& inverse) :
//                  mat(matrix),
//                  inv(inverse)
// {
//     // does nothing
// }
//
// Transform Transform::inverse() const
// {
//     return Transform(inv, mat);
// }
//
// Transform Transform::operator*(const Transform& other) const
// {
//     Eigen::Matrix4f newMat = mat * other.mat;
//     Eigen::Matrix4f newInv = other.inv * inv;
//     return Transform(newMat, newInv);
// }
//
// Vector3f Transform::operator*(const Vector3f& other) const
// {
//     // TODO: fix all of these
//     return mat.topLeftCorner<3,3>() * other;
// }
//
// Normal3f Transform::operator*(const Normal3f& other) const
// {
//     return inv.topLeftCorner<3, 3>().transpose() * other;
// }
//
// Point3f Transform::operator*(const Point3f& other) const
// {
//     Eigen::Vector4f hom = mat * Eigen::Vector4f(other.x, other.y, other.z, 1.f);
//     Eigen::Vector3f hom3 = hom.head<3>() / hom.w();
//     return Point3f(hom3.x(), hom3.y(), hom3.z());
// }
//
// Ray3f Transform::operator*(const Ray3f& other) const
// {
//     return Ray3f(operator*(other.origin),
//                  operator*(other.dir),
//                  other.mint,
//                  other.maxt);
// }
//
// const Eigen::Matrix4f& Transform::getMatrix() const
// {
//     return mat;
// }
//
// const Eigen::Matrix4f& Transform::getInverse() const
// {
//     return inv;
// }
