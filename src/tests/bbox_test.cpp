/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author.
 **/
 
#include <tests/bbox_test.h>
#include <feign/math/bbox.h>
#include <feign/math/ray.h>

BBoxTestData::BBoxTestData() : UnitTestData()
{
    numberOfTests = 8;
}

void BBoxTestData::logReport() const
{
    std::cout << logInfo << std::endl;
}

BBoxTest::BBoxTest() { }

bool BBoxTest::evaluateTest(BBoxTestData& testLog) const
{
    if (!volumeTest())
    {
        testLog.numberFailed++;
        testLog.logInfo += "BBox Volume Test Failed\n";
    }
    if (!surfaceAreaTest())
    {
        testLog.numberFailed++;
        testLog.logInfo += "BBox Surface Area Test Failed\n";
    }
    if (!centerTest())
    {
        testLog.numberFailed++;
        testLog.logInfo += "BBox Center Test Failed\n";
    }
    if (!containsTest())
    {
        testLog.numberFailed++;
        testLog.logInfo += "BBox Contains Test Failed\n";
    }
    if (!overlapTest())
    {
        testLog.numberFailed++;
        testLog.logInfo += "BBox Overlap Test Failed\n";
    }
    if (!extentsTest())
    {
        testLog.numberFailed++;
        testLog.logInfo += "BBox Extents Test Failed\n";
    }
    if (!expandTest())
    {
        testLog.numberFailed++;
        testLog.logInfo += "BBox Expand Test Failed\n";
    }
    if (!rayIntersectTest())
    {
        testLog.numberFailed++;
        testLog.logInfo += "BBox Ray Intersect Test\n";
    }

    testLog.numberPassed = testLog.numberOfTests - testLog.numberFailed;

    return testLog.numberOfTests == testLog.numberPassed;
}
