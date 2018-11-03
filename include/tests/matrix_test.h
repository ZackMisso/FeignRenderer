#pragma once

#include <tests/test.h>

struct MatrixTestData : public UnitTestData
{
    MatrixTestData();

    virtual void logReport() const;
};

class MatrixTest : public UnitTest
{
public:
    MatrixTest();

    virtual bool evaluateTest(UnitTestData& testLog) const;

    bool matrix4inverseITest() const;
    bool matrix3inverseITest() const;
    bool matrix4transposeITest() const;
    bool matrix3transposeITest() const;
    bool matrix4multITest() const;
    bool matrix3multITest() const;

    bool matrix4inverseSTest() const;
    bool matrix3inverseSTest() const;
    bool matrix4transposeSTest() const;
    bool matrix3transposeSTest() const;
    bool matrix4multSTest() const;
    bool matrix3multSTest() const;

    bool matrix4inverseSTTest() const;
    bool matrix3inverseSTTest() const;
    bool matrix4transposeSTTest() const;
    bool matrix3transposeSTTest() const;
    bool matrix4multSTTest() const;
    bool matrix3multSTTest() const;

    bool matrix4inverseSTRTest() const;
    bool matrix3inverseSTRTest() const;
    bool matrix4transposeSTRTest() const;
    bool matrix3transposeSTRTest() const;
    bool matrix4multSTRTest() const;
    bool matrix3multSTRTest() const;
};
