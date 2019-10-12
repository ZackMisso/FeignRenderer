/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

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
    void jsonTutorial() const;
    bool createBaseSceneTest() const;
    bool readBaseSceneTest() const;

protected:
    bool testing;
};
