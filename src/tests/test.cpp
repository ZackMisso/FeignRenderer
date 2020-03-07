#include <tests/tester.h>
#include <imedit/image.h>
#include <feign/core/api.h>
#include <feign/parser/json_parser.h>

bool evaluate_unit_test(UnitTestData& testLog)
{
    FeignRenderer::initialize();

    JsonParser::parse("../scenes/unit_tests/scenes/"+testLog.test_name);

    return true;
}

bool replace_reference(UnitTestData& testLog)
{
    FeignRenderer::initialize();

    JsonParser::parse("../scenes/unit_tests/refs/"+testLog.test_name);

    return true;
}
