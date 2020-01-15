/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <tests/test.h>

struct TransformTestData : public UnitTestData
{
    TransformTestData();

    virtual void logReport() const;
};

class TransformTest : public UnitTest
{
public:
    TransformTest();

    virtual bool evaluateTest(UnitTestData& testLog) const;
};
