#pragma once

// I do not really think any complex templates are needed for now
// the added virtual tables would only slow down the current renders.
// these are kept as separate data structures for the sole purpose
// of improving code readability and debugging

#include <feign/common.h>

struct Vector3f
{
    Vector3f();
    Vector3f(Float c);
    Vector3f(Float x, Float y, Float z);

    Float x;
    Float y;
    Float z;
};

struct Point3f
{
    Point3f();
    Point3f(Float c);
    Point3f(Float x, Float y, Float z);

    Float x;
    Float y;
    Float z;
};

struct Normal3f
{
    Normal3f();
    Normal3f(Float c);
    Normal3f(Float x, Float y, Float z);

    Float x;
    Float y;
    Float z;
};
