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
