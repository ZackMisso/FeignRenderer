/**
* Author:    Zackary Misso
* Version:   0.2.0
*
* Anyone has permission to use the following code as long as proper
* acknowledgement is provided to the original author(s).
**/

#include <feign/test/tester.h>
#include <imedit/image.h>
#include <imedit/im_util.h>
#include <feign/core/api.h>
#include <feign/parsers/json_parser.h>

FEIGN_BEGIN()

bool evaluate_unit_test(UnitTestData &testLog)
{
    Imagef image = Imagef(256, 256);

    LOG("running test: " + testLog.test_name);

    JsonParser::parse("../scenes/unit_tests/scenes/" + testLog.test_name,
                      &image);

    int loc = testLog.test_name.find_last_of(".");

    std::string result_image_name = testLog.test_name.substr(0, loc) + ".exr";

    Imagef ref_image = Imagef("../scenes/unit_tests/images/" +
                              testLog.test_name.substr(0, loc) + "_ref.exr");

    testLog.image_error = imedit::mean_sqr_error(image, ref_image);

    image.write("../scenes/unit_tests/images/" + result_image_name);

    if (testLog.does_it_fail())
    {
        LOG("IMAGE ERROR: " + std::to_string(testLog.image_error));
        return false;
    }

    return true;
}

bool replace_reference(UnitTestData &testLog)
{
    Imagef image = Imagef(256, 256);

    LOG("running test: " + testLog.test_name);

    JsonParser::parse("../scenes/unit_tests/scenes/" + testLog.test_name,
                      &image);

    int loc = testLog.test_name.find_last_of(".");

    std::string result_image_name = testLog.test_name.substr(0, loc) + "_ref.exr";

    image.write("../scenes/unit_tests/images/" + result_image_name);

    return true;
}

FEIGN_END()
