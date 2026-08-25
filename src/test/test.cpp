/**
 * Author:    Zackary Misso
 * Version:   0.1.0
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

bool evaluate_unit_test(UnitTestData &test_log)
{
    Imagef image = Imagef(512, 512);

    LOG("running test: " + test_log.test_name);

#if CLOCKING
    JsonParser::parse_and_run("../scenes/unit_tests/scenes/" + test_log.test_name,
                              &image,
                              &(testLog.clockings));
#else
    JsonParser::parse_and_run("../scenes/unit_tests/scenes/" + test_log.test_name,
                              &image);
#endif

    int loc = test_log.test_name.find_last_of(".");

    std::string result_image_name = test_log.test_name.substr(0, loc);

    Imagef ref_image = Imagef("../scenes/unit_tests/images/" +
                              test_log.test_name.substr(0, loc) + "_ref.exr");

    test_log.image_error = imedit::mean_absolute_difference(image, ref_image);
    test_log.image_rel_error = imedit::mean_absolute_relative_difference(image, ref_image);
    test_log.image_sqr_error = imedit::mean_sqr_error(image, ref_image);

    image.write("../scenes/unit_tests/images/" + result_image_name + ".exr");
    image.write("../scenes/unit_tests/images/" + result_image_name + ".png");

    Imagef pos_error_image = Imagef(512, 512);
    Imagef neg_error_image = Imagef(512, 512);

    for (int i = 0; i < 512; ++i)
    {
        for (int j = 0; j < 512; ++j)
        {
            imedit::Pixel val = ref_image(j, i) - image(j, i);

            // compute relative error for positive and negative values independently
            pos_error_image(j, i) = val.positives(); // / ref_image(j, i);
            neg_error_image(j, i) = val.negatives(); // / ref_image(j, i);
        }
    }

    pos_error_image.write("../scenes/unit_tests/images/" + result_image_name + "_pos_error.png");
    neg_error_image.write("../scenes/unit_tests/images/" + result_image_name + "_neg_error.png");

    if (test_log.does_it_fail())
    {
        LOG("IMAGE ERROR: " + std::to_string(test_log.image_error));
        return false;
    }

    return true;
}

bool replace_reference(UnitTestData &test_log)
{
    Imagef image = Imagef(512, 512);

    LOG("running test: " + test_log.test_name);
    LOG("test path: " + test_log.test_path);

    JsonParser::parse_and_run(test_log.test_path,
                              &image);

    int loc = test_log.test_name.find_last_of(".");

    std::string result_image_name = test_log.test_name.substr(0, loc);

    image.write("../scenes/unit_tests/images/" + result_image_name + "_ref.exr");
    image.write("../scenes/unit_tests/images/" + result_image_name + "_ref.png");

    return true;
}

FEIGN_END()
