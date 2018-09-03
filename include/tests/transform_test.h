#pragma once

#include <tests/test.h>

struct TransformTestData
{
    TransformTestData();

    // TODO
};

class TransformTest
{
public:
    TransformTest();

    bool evaluateTest(TransformTestData& testLog) const;
};
