/**
 * Author:    Zackary Misso
 * Version:   0.2.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/core/shape.h>

FEIGN_BEGIN()

/////////////////////////////////////////////////
// Signed Distance Function Shape
/////////////////////////////////////////////////
class SDFShape : public Shape
{
public:
    SDFShape() : Shape(nullptr, false) { }
    SDFShape(const MediumBoundry* boundry, bool is_null)
        : Shape(boundry, is_null) { }
    ~SDFShape() { }

    virtual Float evaluate(Point3f pt) const = 0;

    // TODO: remove this, will cause major compile errors
    Float interp; // TODO: how to represent interp across all objects??
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Signed Distance Interaction
/////////////////////////////////////////////////
// this struct should describe the interaction between two signed distance
// fields. At the moment, I have no idea how this should effect materials.
/////////////////////////////////////////////////
struct SDFInteraction : public SDFShape
{
public:
    SDFInteraction(SDFShape* sdf_1,
                   SDFShape* sdf_2)
        : sdf_1(sdf_1), sdf_2(sdf_2) { }

    SDFShape* sdf_1;
    SDFShape* sdf_2;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Signed Distance Intersection
/////////////////////////////////////////////////
struct SDFIntersection : public SDFInteraction
{
public:
    SDFIntersection(SDFShape* sdf_1,
                    SDFShape* sdf_2);

    virtual Float evaluate(Point3f pt) const;

    SDFShape* sdf_1;
    SDFShape* sdf_2;
    bool is_smooth;
    Float interp; // for smooth intersection
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Signed Distance Rounding
/////////////////////////////////////////////////
// TODO
struct SDFRounding : public SDFInteraction
{
public:
    SDFRounding(SDFShape* sdf_1,
                SDFShape* sdf_2);

    virtual Float evaluate(Point3f pt) const;

    SDFShape* sdf_1;
    SDFShape* sdf_2;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Signed Distance Onion
/////////////////////////////////////////////////
// TODO
struct SDFOnion : public SDFInteraction
{
public:
    SDFOnion(SDFShape* sdf_1,
             SDFShape* sdf_2);

    virtual Float evaluate(Point3f pt) const;

    SDFShape* sdf_1;
    SDFShape* sdf_2;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Signed Distance Extrusion
/////////////////////////////////////////////////
struct SDFExtrusion : public SDFInteraction
{
public:
    SDFExtrusion(SDFShape* sdf_1,
                 SDFShape* sdf_2);

    virtual Float evaluate(Point3f pt) const;

    SDFShape* sdf_1;
    SDFShape* sdf_2;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Signed Distance Revolution
/////////////////////////////////////////////////
struct SDFRevolution : public SDFInteraction
{
public:
    SDFRevolution(SDFShape* sdf_1,
                  SDFShape* sdf_2);

    virtual Float evaluate(Point3f pt) const;

    SDFShape* sdf_1;
    SDFShape* sdf_2;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Signed Distance Union
/////////////////////////////////////////////////
struct SDFUnion : public SDFInteraction
{
public:
    SDFUnion(SDFShape* sdf_1,
             SDFShape* sdf_2);

    virtual Float evaluate(Point3f pt) const;

    SDFShape* sdf_1;
    SDFShape* sdf_2;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Signed Distance Difference
/////////////////////////////////////////////////
struct SDFDifference : public SDFInteraction
{
public:
    SDFDifference(SDFShape* sdf_1,
                  SDFShape* sdf_2);

    virtual Float evaluate(Point3f pt) const;

    SDFShape* sdf_1;
    SDFShape* sdf_2;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Signed Distance Displacement
/////////////////////////////////////////////////
struct SDFDisplacement : public SDFInteraction
{
public:
    SDFDisplacement(SDFShape* sdf_1,
                    SDFShape* sdf_2);

    virtual Float evaluate(Point3f pt) const;

    SDFShape* sdf_1;
    SDFShape* sdf_2;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Signed Distance Twist
/////////////////////////////////////////////////
struct SDFTwist : public SDFInteraction
{
public:
    SDFTwist(SDFShape* sdf_1,
             SDFShape* sdf_2);

    virtual Float evaluate(Point3f pt) const;

    SDFShape* sdf_1;
    SDFShape* sdf_2;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Signed Distance Bend
/////////////////////////////////////////////////
struct SDFBend : public SDFInteraction
{
public:
    SDFBend(SDFShape* sdf_1,
            SDFShape* sdf_2);

    virtual Float evaluate(Point3f pt) const;

    SDFShape* sdf_1;
    SDFShape* sdf_2;
};
/////////////////////////////////////////////////

// TODO: how to define infinite repetition?

/////////////////////////////////////////////////
// Signed Distance Function Sphere shape
/////////////////////////////////////////////////
class SDFSphere : public SDFShape
{
public:
    struct Params
    {
        Params(Point3f center,
               Float radius,
               Float interp,
               std::string inside_media = "null",
               std::string outside_media = "null",
               bool is_null = false)
            : inside_media(inside_media),
              outside_media(outside_media),
              is_null(is_null),
              center(center),
              radius(radius),
              interp(interp) { }

        std::string inside_media;
        std::string outside_media;
        Point3f center;
        Float radius;
        Float interp;
        bool is_null;
    };

    SDFSphere(Point3f center,
              Float radius,
              Float interp,
              const MediumBoundry* boundry = nullptr,
              bool is_null = false);

    virtual Float evaluate(Point3f pt) const;

    virtual BBox3f boundingBox() const;
    virtual Point3f centroid() const;

    virtual uint32_t primitiveCount() const { return 1; }

protected:
    Point3f center;
    Float radius;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Signed Distance Function Plane shape
/////////////////////////////////////////////////
class SDFPlane : public SDFShape
{
public:
    struct Params
    {
        Params(Point3f center,
               Normal3f normal,
               Float interp,
               std::string inside_media = "null",
               std::string outside_media = "null",
               bool is_null = false)
            : center(center),
              normal(normal),
              interp(interp),
              inside_media(inside_media),
              outside_media(outside_media),
              is_null(is_null){ }

        std::string inside_media;
        std::string outside_media;
        Point3f center;
        Normal3f normal;
        Float interp;
        bool is_null;
    };

    SDFPlane(Point3f center,
             Normal3f normal,
             Float interp,
             const MediumBoundry* boundry = nullptr,
             bool is_null = false);

    virtual Float evaluate(Point3f pt) const;

    virtual BBox3f boundingBox() const;
    virtual Point3f centroid() const;

    virtual uint32_t primitiveCount() const { return 1; }

protected:
    Point3f center;
    Normal3f normal;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Signed Distance Function Box shape
/////////////////////////////////////////////////
class SDFBox : public SDFShape
{
public:
    struct Params
    {
        Params(Point3f tlc,
               Point3f brc,
               std::string inside_media = "null",
               std::string outside_media = "null",
               bool is_null = false)
            : tlc(tlc),
              brc(brc),
              inside_media(inside_media),
              outside_media(outside_media),
              is_null(is_null) { }

        std::string inside_media;
        std::string outside_media;
        Point3f tlc;
        Point3f brc;
        bool is_null;
    };

    SDFBox(Point3f tlc,
           Point3f brc,
           const MediumBoundry* boundry = nullptr,
           bool is_null = false);

    virtual Float evaluate(Point3f pt) const;

    virtual BBox3f boundingBox() const;
    virtual Point3f centroid() const;

    virtual uint32_t primitiveCount() const { return 1; }

protected:
    Point3f tlc;
    Point3f brc;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Signed Distance Function Cylinder shape
/////////////////////////////////////////////////
class SDFCylinder : public SDFShape
{
public:
    struct Params
    {
        Params(Point3f first,
               Point3f second,
               float radius,
               std::string inside_media = "null",
               std::string outside_media = "null",
               bool is_null = false)
            : first(first),
              second(second),
              radius(radius),
              inside_media(inside_media),
              outside_media(outside_media),
              is_null(is_null){ }

        std::string inside_media;
        std::string outside_media;
        Point3f first;
        Point3f second;
        float radius;
        bool is_null;
    };

    SDFCylinder(Point3f first,
                Point3f second,
                float radius,
                const MediumBoundry* boundry = nullptr,
                bool is_null = false);

    virtual Float evaluate(Point3f pt) const;

    virtual BBox3f boundingBox() const;
    virtual Point3f centroid() const;

    virtual uint32_t primitiveCount() const { return 1; }

protected:
    Point3f first;
    Point3f second;
    float radius;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Signed Distance Function Cone shape
/////////////////////////////////////////////////
class SDFCone : public SDFShape
{
public:
    struct Params
    {
        Params(Point3f first,
               Point3f second,
               Float radius_1,
               Float radius_2,
               std::string inside_media = "null",
               std::string outside_media = "null",
               bool is_null = false)
            : first(first),
              second(second),
              radius_1(radius_1),
              radius_2(radius_2),
              inside_media(inside_media),
              outside_media(outside_media),
              is_null(is_null) { }

        std::string inside_media;
        std::string outside_media;
        Point3f first;
        Point3f second;
        Float radius_1;
        Float radius_2;
        bool is_null;
    };

    SDFCone(Point3f first,
            Point3f second,
            Float radius_1,
            Float radius_2,
            const MediumBoundry* boundry = nullptr,
            bool is_null = false);

    virtual Float evaluate(Point3f pt) const;

    virtual BBox3f boundingBox() const;
    virtual Point3f centroid() const;

    virtual uint32_t primitiveCount() const { return 1; }

protected:
    Point3f first;
    Point3f second;
    Float radius_1;
    Float radius_2;
};
/////////////////////////////////////////////////

FEIGN_END()
