#pragma once

#include <feign/common.h>

struct UnitTestData {
    UnitTestData() {
        logInfo = "";
        errInfo = "";
        timeInfo = "";

        numberOfTests = 0;
        numberPassed = 0;
        numberFailed = 0;
    }

    virtual void logReport() const = 0;

    string logInfo;
    string errInfo;
    string timeInfo; // maybe use this later ??

    int numberOfTests;
    int numberPassed;
    int numberFailed;
};

class UnitTest {
public:
    virtual bool evaluateTest(UnitTestData& testLog) const = 0;
};
