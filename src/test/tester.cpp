/**
 * Author:    Zackary Misso
 * Version:   0.1.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/test/tester.h>
#include <dirent.h>
#include <iostream>
#include <iomanip>
#include <sstream>

FEIGN_BEGIN()

void UnitTestData::logReport() const
{
    // TODO
}

UnitTestManager::UnitTestManager()
{
    reference_run = false;
}

std::string zero_padded_num(int num)
{
    std::ostringstream ss;
    ss << std::setw(3) << std::setfill('0') << num;
    return ss.str();
}

bool UnitTestManager::run_test(int index)
{
    std::string path = "test_" + zero_padded_num(index) + ".json";
    UnitTestData testLog = UnitTestData(path);
    LOG("evaluating test: " + path);

    if (reference_run)
    {
        replace_reference(testLog);
    }
    else
    {
        evaluate_unit_test(testLog);
        testLog.logReport();
    }

    return testLog.does_it_fail();
}

bool UnitTestManager::run_all_tests()
{
    // parse all of the scenes in the test scenes folder
    std::vector<std::string> paths = std::vector<std::string>();
    std::vector<std::string> dirs = std::vector<std::string>();
    std::vector<std::string> names = std::vector<std::string>();

    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir("../scenes/unit_tests/scenes/")) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            std::string str = std::string(ent->d_name);
            if (str.length() > 5 && str.substr(str.length() - 5) == ".json")
            {
                names.push_back(str);
                paths.push_back("../scenes/unit_tests/scenes/" + str);
            }
        }
    }

    bool passes = true;

    for (int i = 0; i < paths.size(); ++i)
    {
        LOG("RUNNING TEST: " + std::to_string(i));
        UnitTestData testLog = UnitTestData(paths[i], "../scenes/unit_tests/scenes/", names[i]);

        if (reference_run)
        {
            replace_reference(testLog);
        }
        else
        {
            evaluate_unit_test(testLog);
            testLog.logReport();
            if (testLog.does_it_fail())
                passes = false;
            
            #if RECORD
                // if record is enabled, we want to append the various
                // clockings to the test's permanent record.
                UnitTestSiteAssembler::append_to_test_records(testLog);

                // TODO: create python scripts to generate plots for
                // the tests' records and call them from here.
            #endif
        }

        // TODO: autogenerate comparision webpage
        UnitTestSiteAssembler::create_test_html_page(testLog);
    }

    UnitTestSiteAssembler::create_global_html_page();

    return passes;
}

FEIGN_END()
