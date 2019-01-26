#pragma once

#include <tests/test.h>

struct RayIntersectTestData : public UnitTestData
{
    RayIntersectTestData();

    virtual void logReport() const;
};

class RayIntersectTest : public UnitTest
{
public:
    RayIntersectTest();

    virtual bool evaluateTest(UnitTestData& testLog) const;
};
