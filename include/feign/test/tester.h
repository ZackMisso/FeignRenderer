/**
 * Author:    Zackary Misso
 * Version:   0.1.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/common.h>

FEIGN_BEGIN()

struct UnitTestData
{
    UnitTestData(std::string test_path)
        : test_path(test_path),
          test_dir(""),
          test_name(test_path),
          image_error(0.f),
          threshold(1.f) {}

    UnitTestData(std::string test_path,
                 std::string test_dir,
                 std::string test_name)
        : test_path(test_path),
          test_dir(test_dir),
          test_name(test_name),
          image_error(0.f),
          threshold(1.f) {}

    void logReport() const;
    bool does_it_fail() const { return image_error > threshold; }

    std::string test_path;
    std::string test_dir;
    std::string test_name;
    float image_sqr_error;
    float image_rel_error;
    float image_error;
    float threshold;
};

// this class only contains static methods
struct UnitTestSiteAssembler
{
    static void create_global_html_page();
    static void create_test_html_page(UnitTestData &testLog);
};

struct UnitTestManager
{
    UnitTestManager();

    bool run_all_tests();
    bool run_test(int index);
    // i dont think i need this anymore
    // void parse_scene_files(std::string main_dir,
    //                        std::string base_dir,
    //                        std::vector<std::string> &paths,
    //                        std::vector<std::string> &dirs,
    //                        std::vector<std::string> &names);

    bool reference_run; // replace the current references
    bool log_stats;     // if stats should be logged
};

// test methods are implemented in test.cpp
extern bool evaluate_unit_test(UnitTestData &testLog);
extern bool replace_reference(UnitTestData &testLog);

FEIGN_END()
