#pragma once

#include <tests/test.h>

struct BBoxTestData : public UnitTestData
{
    BBoxTestData();

    virtual void logReport();
};

class BBoxTest : public UnitTest
{
public:
    BBoxTest();

    virtual bool evaluateTest(BBoxTestData& testLog) const;

    bool volumeTest() const;
    bool surfaceAreaTest() const;
    bool centerTest() const;
    bool containsTest() const;
    bool overlapTest() const;
    bool extentsTest() const;
    bool expandTest() const;
    bool rayIntersectTest() const;
};
