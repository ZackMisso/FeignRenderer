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

    string logInfo;
    string errInfo;
    string timeInfo; // maybe use this later ??

    int numberOfTests;
    int numberPassed;
    int numberFailed;
};

class UnitTest {
public:
    bool evaluateTest(UnitTestData& testLog) = 0;
};
