#include <tests/tester.h>
#include <tests/vector_test.h>
#include <tests/matrix_test.h>
#include <tests/transform_test.h>

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

    transformTestData.logReport();
    vectorTestData.logReport();
    matrixTestData.logReport();

    return transformTestPassed &&
           vectorTestPassed &&
           matrixTestPassed;
}
