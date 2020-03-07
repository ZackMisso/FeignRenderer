/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <tests/tester.h>
#include <dirent.h>

void UnitTestData::logReport() const
{
    // TODO
}

// NOTE: this is a renderer, my unit tests are going to be converted to all
//       running and comparing renders instead of these individual unit tests

UnitTestManager::UnitTestManager()
{
    reference_run = false;
    // does nothing for now
}

bool UnitTestManager::run_all_tests()
{
    // parse all of the scenes in the test scenes folder

    std::vector<std::string> paths = std::vector<std::string>();

    DIR *dir;
    struct dirent* ent;

    if ((dir = opendir("../scenes/unit_tests/scenes/")) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            std::string str = std::string(ent->d_name);
            if (str.length() > 5 && str.substr(str.length()-5) == ".json")
                paths.push_back(str);
        }
    }

    for (int i = 0; i < paths.size(); ++i)
    {
        UnitTestData testLog = UnitTestData(paths[i]);

        if (reference_run)
        {
            replace_reference(testLog);
        }
        else
        {
            evaluate_unit_test(testLog);
            testLog.logReport();
        }
    }
}
