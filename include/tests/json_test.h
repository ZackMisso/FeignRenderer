#pragma once

#include <tests/test.h>

struct JsonTestData : public UnitTestData
{
    JsonTestData();

    virtual void logReport() const;
};

class JsonTest : public UnitTest
{
public:
    JsonTest();

    virtual bool evaluateTest(UnitTestData& testLog) const;

    // the tutorial from the RapidJSON devs
    bool jsonTutorial() const;
    bool createBaseSceneTest() const;
    bool readBaseSceneTest() const;

protected:
    bool testing;
};
