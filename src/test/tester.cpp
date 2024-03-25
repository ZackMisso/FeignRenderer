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

// NOTE: this is a renderer, my unit tests are going to be converted to all
//       running and comparing renders instead of these individual unit tests

// TODO: remove the hacky directory structure in return for
//       autommatically generating a simple html page to see
//       unit test results

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

void UnitTestManager::parse_scene_files(
    std::string main_dir,
    std::string base_dir,
    std::vector<std::string> &paths,
    std::vector<std::string> &dirs,
    std::vector<std::string> &names)
{
    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir(base_dir.c_str())) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            std::string str = std::string(ent->d_name);

            if (str.find(".") == std::string::npos)
            {
                parse_scene_files(main_dir + str + "/",
                                  base_dir + str + "/",
                                  paths,
                                  dirs,
                                  names);
            }

            if (str.length() > 5 && str.substr(str.length() - 5) == ".json")
            {
                paths.push_back(base_dir + str);
                dirs.push_back(main_dir);
                names.push_back(str);
            }
        }
    }
}

bool UnitTestManager::run_all_tests()
{
    // parse all of the scenes in the test scenes folder
    std::vector<std::string> paths = std::vector<std::string>();
    std::vector<std::string> dirs = std::vector<std::string>();
    std::vector<std::string> names = std::vector<std::string>();

    // // parse all scenes and their directories
    // parse_scene_files("", "../scenes/unit_tests/scenes/", paths, dirs, names);

    // // create all the necessary subfolders for the results directory
    // for (int i = 0; i < dirs.size(); ++i)
    // {
    //     std::string str = "mkdir ../scenes/unit_tests/images/" + dirs[i];
    //     system(str.c_str());
    // }

    // // debug logic
    // // LOG("PATHS:");
    // // for (int i = 0; i < paths.size(); ++i)
    // // {
    // //     LOG(paths[i]);
    // // }

    // // LOG("DIRS:");
    // // for (int i = 0; i < dirs.size(); ++i)
    // // {
    // //     LOG(dirs[i]);
    // // }

    // bool passes = true;

    // // TODO: make this multithreaded
    // for (int i = 0; i < paths.size(); ++i)
    // {
    //     LOG("RUNNING TEST: " + std::to_string(i));
    //     UnitTestData testLog = UnitTestData(paths[i], dirs[i], names[i]);

    //     if (reference_run)
    //     {
    //         replace_reference(testLog);
    //     }
    //     else
    //     {
    //         evaluate_unit_test(testLog);
    //         testLog.logReport();
    //         if (testLog.does_it_fail())
    //             passes = false;
    //     }
    // }

    // return false;

    // TODO: rewrite this for the new directory structure

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

    // TODO: make this multithreaded
    for (int i = 0; i < paths.size(); ++i)
    {
        LOG("RUNNING TEST: " + std::to_string(i));
        UnitTestData testLog = UnitTestData(paths[i], "../scenes/unit_tests/scenes/", names[i]);

        if (reference_run)
        {
            LOG("Creating reference");
            replace_reference(testLog);
        }
        else
        {
            evaluate_unit_test(testLog);
            testLog.logReport();
            if (testLog.does_it_fail())
                passes = false;
        }
    }

    UnitTestSiteAssembler::create_global_html_page();

    return passes;
}

FEIGN_END()
