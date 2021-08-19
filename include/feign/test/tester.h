/**
 * Author:    Zackary Misso
 * Version:   0.2.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/common.h>

FEIGN_BEGIN()

struct UnitTestData
{
    UnitTestData(std::string test_name)
        : test_name(test_name),
          image_error(0.f),
          threshold(1.f) {}

    void logReport() const;
    bool does_it_fail() const { return image_error > threshold; }

    std::string test_name;
    float image_error;
    float threshold;
};

struct UnitTestManager
{
    UnitTestManager();

    bool run_all_tests();
    bool run_test(int index);

    bool reference_run; // replace the current references
};

// test methods are implemented in test.cpp
extern bool evaluate_unit_test(UnitTestData &testLog);
extern bool replace_reference(UnitTestData &testLog);

FEIGN_END()
