/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author.
 **/

#include <tests/tester.h>
#include <tests/vector_test.h>
#include <tests/matrix_test.h>
#include <tests/transform_test.h>
#include <tests/ray_intersect_test.h>
#include <tests/json_test.h>

UnitTestManager::UnitTestManager() {
    // does nothing for now
}

bool UnitTestManager::runUnitTests() {
    TransformTestData transformTestData = TransformTestData();
    TransformTest transformTest = TransformTest();
    bool transformTestPassed = transformTest.evaluateTest(transformTestData);

    VectorTestData vectorTestData = VectorTestData();
    VectorTest vectorTest = VectorTest();
    bool vectorTestPassed = vectorTest.evaluateTest(vectorTestData);

    MatrixTestData matrixTestData = MatrixTestData();
    MatrixTest matrixTest = MatrixTest();
    bool matrixTestPassed = matrixTest.evaluateTest(matrixTestData);

    RayIntersectTestData rayIntersectTestData = RayIntersectTestData();
    RayIntersectTest rayTest = RayIntersectTest();
    bool rayTestPassed = rayTest.evaluateTest(rayIntersectTestData);

    JsonTestData jsonTestData = JsonTestData();
    JsonTest jsonTest = JsonTest();
    bool jsonTestPassed = jsonTest.evaluateTest(jsonTestData);

    transformTestData.logReport();
    vectorTestData.logReport();
    matrixTestData.logReport();
    rayIntersectTestData.logReport();
    jsonTestData.logReport();

    return transformTestPassed &&
           vectorTestPassed &&
           matrixTestPassed &&
           rayTestPassed &&
           jsonTestPassed;
}
