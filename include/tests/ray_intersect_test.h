/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

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
