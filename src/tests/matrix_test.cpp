#include <tests/matrix_test.h>
#include <feign/math/vector.h>

MatrixTestData::MatrixTestData() : UnitTestData()
{
    numberOfTests = 24;
}

void MatrixTestData::logReport() const
{
    std::cout << logInfo << std::endl;
}

MatrixTest::MatrixTest() { }

bool MatrixTest::evaluateTest(UnitTestData& testLog) const
{
    // cout << testLog.numberOfTests << endl;
    if(!matrix4inverseITest())
    {
        testLog.numberFailed++;
        testLog.logInfo += "Matrix 4 Inverse I Test Failed\n";
    }
    if(!matrix3inverseITest())
    {
        testLog.numberFailed++;
        testLog.logInfo += "Matrix 3 Inverse I Test Failed\n";
    }
    if(!matrix4transposeITest())
    {
        testLog.numberFailed++;
        testLog.logInfo += "Matrix 4 Transpose I Test Failed\n";
    }
    if(!matrix3transposeITest())
    {
        testLog.numberFailed++;
        testLog.logInfo += "Matrix 3 Transpose I Test Failed\n";
    }
    if(!matrix4multITest())
    {
        testLog.numberFailed++;
        testLog.logInfo += "Matrix 4 Mult I Test Failed\n";
    }
    if(!matrix3multITest())
    {
        testLog.numberFailed++;
        testLog.logInfo += "Matrix 3 Mult I Test Failed\n";
    }
    if(!matrix4inverseSTest())
    {
        testLog.numberFailed++;
        testLog.logInfo += "Matrix 4 Inverse S Test Failed\n";
    }
    if(!matrix3inverseSTest())
    {
        testLog.numberFailed++;
        testLog.logInfo += "Matrix 3 Inverse S Test Failed\n";
    }
    if(!matrix4transposeSTest())
    {
        testLog.numberFailed++;
        testLog.logInfo += "Matrix 4 Transpose S Test Failed\n";
    }
    if(!matrix3transposeSTest())
    {
        testLog.numberFailed++;
        testLog.logInfo += "Matrix 3 Transpose S Test Failed\n";
    }
    if(!matrix4multSTest())
    {
        testLog.numberFailed++;
        testLog.logInfo += "Matrix 4 Mult S Test Failed\n";
    }
    if(!matrix3multSTest())
    {
        testLog.numberFailed++;
        testLog.logInfo += "Matrix 3 Mult S Test Failed\n";
    }
    if(!matrix4inverseSTTest())
    {
        testLog.numberFailed++;
        testLog.logInfo += "Matrix 4 Inverse ST Test Failed\n";
    }
    if(!matrix3inverseSTTest())
    {
        testLog.numberFailed++;
        testLog.logInfo += "Matrix 3 Inverse ST Test Failed\n";
    }
    if(!matrix4transposeSTTest())
    {
        testLog.numberFailed++;
        testLog.logInfo += "Matrix 4 Transpose ST Test Failed\n";
    }
    if(!matrix3transposeSTTest())
    {
        testLog.numberFailed++;
        testLog.logInfo += "Matrix 3 Transpose ST Test Failed\n";
    }
    if(!matrix4multSTTest())
    {
        testLog.numberFailed++;
        testLog.logInfo += "Matrix 4 Mult ST Test Failed\n";
    }
    if(!matrix3multSTTest())
    {
        testLog.numberFailed++;
        testLog.logInfo += "Matrix 3 Mult ST Test Failed\n";
    }
    if(!matrix4inverseSTRTest())
    {
        testLog.numberFailed++;
        testLog.logInfo += "Matrix 4 Inverse STR Test Failed\n";
    }
    if(!matrix3inverseSTRTest())
    {
        testLog.numberFailed++;
        testLog.logInfo += "Matrix 3 Inverse STR Test Failed\n";
    }
    if(!matrix4transposeSTRTest())
    {
        testLog.numberFailed++;
        testLog.logInfo += "Matrix 4 Transpose STR Test Failed\n";
    }
    if(!matrix3transposeSTRTest())
    {
        testLog.numberFailed++;
        testLog.logInfo += "Matrix 3 Transpose STR Test Failed\n";
    }
    if(!matrix4multSTRTest())
    {
        testLog.numberFailed++;
        testLog.logInfo += "Matrix 4 Mult STR Test Failed\n";
    }
    if(!matrix3multSTRTest())
    {
        testLog.numberFailed++;
        testLog.logInfo += "Matrix 3 Mult STR Test Failed\n";
    }

    testLog.numberPassed = testLog.numberOfTests - testLog.numberFailed;

    // cout << testLog.numberPassed << endl;

    return testLog.numberOfTests == testLog.numberPassed;
}

bool MatrixTest::matrix4inverseITest() const
{
    Matrix4d result = Matrix4d();

    result[0] = 1.0;
    result[5] = 1.0;
    result[10] = 1.0;
    result[15] = 1.0;

    Matrix4d start = result;
    start = ~start;

    return start == result;
}

bool MatrixTest::matrix3inverseITest() const
{
    // TODO
    return true;
}

bool MatrixTest::matrix4transposeITest() const
{
    // TODO
    return true;
}

bool MatrixTest::matrix3transposeITest() const
{
    // TODO
    return true;
}

bool MatrixTest::matrix4multITest() const
{
    // TODO
    return true;
}

bool MatrixTest::matrix3multITest() const
{
    // TODO
    return true;
}

bool MatrixTest::matrix4inverseSTest() const
{
    Matrix4d result = Matrix4d();

    result[0] = 0.1;
    result[5] = 1.0 / 3.0;
    result[10] = -0.2;
    result[15] = 1.0 / 6.0;

    Matrix4d start = Matrix4d();

    start[0] = 10.0;
    start[5] = 3.0;
    start[10] = -5.0;
    start[15] = 6.0;

    start = ~start;

    // for (int i = 0; i < 16; ++i)
    // {
    //     cout << "i: " << i << " = " << start[i] << endl;
    // }

    return start == result;
}

bool MatrixTest::matrix3inverseSTest() const
{
    // TODO
    return true;
}

bool MatrixTest::matrix4transposeSTest() const
{
    // TODO
    return true;
}

bool MatrixTest::matrix3transposeSTest() const
{
    // TODO
    return true;
}

bool MatrixTest::matrix4multSTest() const
{
    // TODO
    return true;
}

bool MatrixTest::matrix3multSTest() const
{
    // TODO
    return true;
}

bool MatrixTest::matrix4inverseSTTest() const
{
    Matrix4d result = Matrix4d();

    result[0] = 0.1;
    result[5] = 1.0 / 3.0;
    result[10] = -0.2;
    result[15] = 1.0;
    result[3] = -0.2;
    result[7] = 1.0;
    result[11] = 0.8;

    Matrix4d start = Matrix4d();

    start[0] = 10.0;
    start[5] = 3.0;
    start[10] = -5.0;
    start[15] = 1.0;
    start[3] = 2.0;
    start[7] = -3.0;
    start[11] = 4.0;

    start = ~start;

    // for (int i = 0; i < 16; ++i)
    // {
    //     cout << start[i] << " " << result[i] << endl;
    // }

    return start == result;
}

bool MatrixTest::matrix3inverseSTTest() const
{
    // TODO
    return true;
}

bool MatrixTest::matrix4transposeSTTest() const
{
    // TODO
    return true;
}

bool MatrixTest::matrix3transposeSTTest() const
{
    // TODO
    return true;
}

bool MatrixTest::matrix4multSTTest() const
{
    // TODO
    return true;
}

bool MatrixTest::matrix3multSTTest() const
{
    // TODO
    return true;
}

bool MatrixTest::matrix4inverseSTRTest() const
{
    Matrix4d result = Matrix4d();

    result[0] = 51.0 / 397.0;
    result[1] = 43.0 / 397.0;
    result[2] = -57.0 / 397.0;
    result[3] = 255.0 / 397.0;
    result[4] = 25.0 / 397.0;
    result[5] = 60.0 / 397.0;
    result[6] = -98.0 / 397.0;
    result[7] = 522.0 / 397.0;
    result[8] = 31.0 / 397.0;
    result[9] = -5.0 / 397.0;
    result[10] = -58.0 / 397.0;
    result[11] = 155.0 / 397.0;
    result[12] = 0.0;
    result[13] = 0.0;
    result[14] = 0.0;
    result[15] = 1.0;

    Matrix4d start = Matrix4d();

    start[0] = 10.0;
    start[1] = -7.0;
    start[2] = 2.0;
    start[3] = 2.0;
    start[4] = 4.0;
    start[5] = 3.0;
    start[6] = -9.0;
    start[7] = -3.0;
    start[8] = 5.0;
    start[9] = -4.0;
    start[10] = -5.0;
    start[11] = 4.0;
    start[12] = 0.0;
    start[13] = 0.0;
    start[14] = 0.0;
    start[15] = 1.0;

    start = ~start;

    // for (int i = 0; i < 16; ++i)
    // {
    //     cout << start[i] << " " << result[i] << endl;
    // }

    return start == result;
}

bool MatrixTest::matrix3inverseSTRTest() const
{
    // TODO
    return true;
}

bool MatrixTest::matrix4transposeSTRTest() const
{
    // TODO
    return true;
}

bool MatrixTest::matrix3transposeSTRTest() const
{
    // TODO
    return true;
}

bool MatrixTest::matrix4multSTRTest() const
{
    // TODO
    return true;
}

bool MatrixTest::matrix3multSTRTest() const
{
    // TODO
    return true;
}
