#include <feign/cameras/perspective.h>
#include <feign/filters/gauss_filter.h>

Perspective::Perspective() : Camera()
{
    // should not do anything
}

Perspective::Perspective(Node* parent) : Camera(parent)
{
    // should not do anything
}

void Perspective::preProcess()
{
    preProcessChildren();

    primitives->findTransform("toWorld", cameraToWorld, Transform());
    primitives->findInt("width", filmSize[0], 1280);
    primitives->findInt("height", filmSize[1], 720);
    primitives->findFloat("aperatureRadius", aperatureRadius, 0.0);
    primitives->findFloat("focalDistance", focalDistance, 10.0);
    primitives->findFloat("fov", fov, 30.0);
    primitives->findFloat("nearClip", near, 1e-4);
    primitives->findFloat("farClip", far, 1e4);

    float aspect = float(filmSize[0]) / float(filmSize[1]);

    float recip = 1.0 / (far - near);
    float cot = 1.0 / std::tan(degToRad(fov / 2.0));

    Matrix4f perspective = Matrix4f();

    perspective[0] = cot;
    perspective[5] = cot;
    perspective[10] = far * recip;
    perspective[11] = -near * far * recip;
    perspective[14] = 1.0;

    Matrix4f aspectScale = Matrix4f::scale(Vec3f(-0.5, -0.5 * aspect, 1.0));
    Matrix4f aspectTrans = Matrix4f::translate(Vec3f(-1.0, -1.0 / aspect, 0.0));

    Matrix4f mat = aspectScale * aspectTrans * perspective;

    sampleToCamera = Transform(mat).inverse();

    // TODO: add ability to choose different filters.

    filter = new GaussFilter();
    filter->preProcess();
}

Color3f Perspective::sampleRay(Ray3f ray,
                               Point2f filmSamp,
                               Point2f appSamp) const
{
    throw new NotImplementedException("ortho sampleRay");

    return Color3f(1.0);
}

string Perspective::getName() const
{
    return Camera::getName() + "perspective";
}
