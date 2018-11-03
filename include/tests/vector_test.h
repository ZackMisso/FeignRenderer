#pragma once

#include <tests/test.h>

struct VectorTestData : public UnitTestData
{
    VectorTestData();

    virtual void logReport() const;
};

class VectorTest : public UnitTest
{
public:
    VectorTest();

    virtual bool evaluateTest(UnitTestData& testLog) const;
};
