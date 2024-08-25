/**
 * Author:    Zackary Misso
 * Version:   0.1.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/api.h>
#include <feign/parsers/json_parser.h>
#include <feign/test/tester.h>
#include <iostream>

#include <openvdb/openvdb.h>

// this is currently included here while I test out Pixar's USD
// #include <../scenes/usd/scene_creation/create.h>

using namespace feign;

int main(int argc, char *argv[])
{
#if OPENVDB
    openvdb::initialize();
#endif

    // TODO: Get this working later
    // LOG("");
    // LOG("testing USD scene creation tool");
    // USDUnitTest_CreationTool();
    // LOG("");

    std::string scene = "";

    for (int i = 1; i < argc; ++i)
    {
        if (strcmp(argv[i], "-u") == 0)
        {
            std::cout << "Beginning Unit Tests" << std::endl;
            UnitTestManager *unitTests = new UnitTestManager();

            if (!unitTests->run_all_tests())
            {
                delete unitTests;
                std::cout << "Unit Tests Failed -> terminating early" << std::endl;
                return -1;
            }
            delete unitTests;

            std::cout << "All Unit Tests Passed" << std::endl;
            return 0;
        }
        if (strcmp(argv[i], "-ui") == 0)
        {
            std::cout << "Beginning Unit Tests" << std::endl;
            UnitTestManager *unitTests = new UnitTestManager();

            int index = std::stoi(argv[i + 1]);

            if (!unitTests->run_test(index))
            {
                delete unitTests;
                std::cout << "Unit Tests Failed -> terminating early" << std::endl;
                return -1;
            }
            delete unitTests;

            std::cout << "All Unit Tests Passed" << std::endl;
            return 0;
        }
        if (strcmp(argv[i], "-ur") == 0)
        {
            UnitTestManager *unitTests = new UnitTestManager();
            unitTests->reference_run = true;

            if (!unitTests->run_all_tests())
            {
                delete unitTests;
                std::cout << "Unit Tests Failed -> terminating early" << std::endl;
                return -1;
            }
            delete unitTests;

            std::cout << "All Unit Tests Passed" << std::endl;
            return 0;
        }
        if (strcmp(argv[i], "-uri") == 0)
        {
            // is breaking
            UnitTestManager *unitTests = new UnitTestManager();
            unitTests->reference_run = true;

            int index = std::stoi(argv[i + 1]);

            if (!unitTests->run_test(index))
            {
                delete unitTests;
                std::cout << "Unit Tests Failed -> terminating early" << std::endl;
                return -1;
            }
            delete unitTests;

            std::cout << "All Unit Tests Passed" << std::endl;
            return 0;
        }
        if (strcmp(argv[i], "-s") == 0)
        {
            scene = std::string(argv[++i]);
        }
    }

    LOG("");
    LOG(scene);
    LOG("");

    JsonParser::parse_and_run(scene);

    return 0;
}
