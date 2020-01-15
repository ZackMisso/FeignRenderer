/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

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

    std::string logInfo;
    std::string errInfo;
    std::string timeInfo; // maybe use this later ??

    int numberOfTests;
    int numberPassed;
    int numberFailed;
};

class UnitTest {
public:
    virtual bool evaluateTest(UnitTestData& testLog) const = 0;
};
