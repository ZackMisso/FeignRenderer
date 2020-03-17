#include <tests/tester.h>
#include <imedit/image.h>
#include <imedit/im_util.h>
#include <feign/core/api.h>
#include <feign/parser/json_parser.h>

bool evaluate_unit_test(UnitTestData& testLog)
{
    Imagef image = Imagef(256, 256, 3);

    JsonParser::parse("../scenes/unit_tests/scenes/"+testLog.test_name,
                      &image);

    int loc = testLog.test_name.find_last_of(".");

    std::string result_image_name = testLog.test_name.substr(0, loc) + ".exr";

    Imagef ref_image = Imagef("../scenes/unit_tests/scenes/" +
                              testLog.test_name.substr(0, loc) + "_ref.exr");

    testLog.image_error = imedit::mean_sqr_error(image, ref_image);

    if (testLog.does_it_fail())
    {
        return false;
    }

    image.write("../scenes/unit_tests/scenes/" + result_image_name);

    return true;
}

bool replace_reference(UnitTestData& testLog)
{
    Imagef image = Imagef(256, 256, 3);

    JsonParser::parse("../scenes/unit_tests/scenes/"+testLog.test_name,
                      &image);

    int loc = testLog.test_name.find_last_of(".");

    std::string result_image_name = testLog.test_name.substr(0, loc) + "_ref.exr";

    image.write("../scenes/unit_tests/scenes/" + result_image_name);

    return true;
}
