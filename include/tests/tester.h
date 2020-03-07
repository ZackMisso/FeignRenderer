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
    UnitTestData(std::string test_name)
        : test_name(test_name) { }

    void logReport() const;

    std::string test_name;
    float image_variance;
    float threshold;
};

struct UnitTestManager
{
    UnitTestManager();

    bool run_all_tests();

    bool reference_run;            // replace the current references
};

// test methods are implemented in test.cpp
extern bool evaluate_unit_test(UnitTestData& testLog);
extern bool replace_reference(UnitTestData& testLog);
