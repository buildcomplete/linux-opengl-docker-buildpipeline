#include "test_framework.h"
#include "canvas/Canvas.h" // Uncomment when you want to test actual Canvas class

void test_basic_canvas_operations()
{
    test_section("Basic Canvas Operations Tests");
    {
        Canvas c;
        test_equals(c.GetCellInfo(0, 0).componentId, (std::uint8_t)0, "Test cell has zero component id");
    }
    {
        Canvas c;
        test_assert(
            c.AddComponent(ComponentFactory::GetBluePrint(CMPNAMES::DRAWING_COMPONENT), 0, 0),
            "test adding component",
            "c.AddComponent(...");
        test_equals(c.GetCellInfo(0, 0).componentId, (std::uint8_t)1, "Test cell has component id 1");
    }

    {
        Canvas c;
        c.AddComponent(ComponentFactory::GetBluePrint(CMPNAMES::DRAWING_COMPONENT), 0, 0);
        test_assert(
            !(c.AddComponent(ComponentFactory::GetBluePrint(CMPNAMES::DRAWING_COMPONENT), 0, 0)),
            "test adding component on top of component fails",
            "c.AddComponent(...");
        test_equals(c.GetCellInfo(0, 0).componentId, (std::uint8_t)1, "Test cell has component id 1 (After add on top)");
    }

    {
        Canvas c;
        c.AddComponent(ComponentFactory::GetBluePrint(CMPNAMES::DRAWING_COMPONENT), 0, 0);
        test_assert(
            c.ReleaseComponent(1),
            "test remove component thats there (0,0)",
            "c.AddComponent(...");
        test_equals(c.GetCellInfo(0, 0).componentId, (std::uint8_t)0, "Test cell has component id 0 (after remove)");
    }
    {
        Canvas c;
        test_assert(
            !(c.ReleaseComponent(1)),
            "test remove component thats not there (0,0)",
            "c.AddComponent(...");
        test_equals(c.GetCellInfo(0, 0).componentId, (std::uint8_t)0, "Test cell has component id 0 (after remove)");
    }
}

void test_capacity_canvas_operations()
{
    test_section("canvas capacity Operations Tests");
    {
        Canvas c;
        char text[] = "Test adding --- of 255 components (---, ---)";
        int i=0, x,y;
        // Go to the limit, all should be ok, but I dont want to print one million times
        bool allPassedTowardsTheLimit = true;
        
        do
        {
            x = (i % 20) * 5, y = (i / 20) * 5;
            sprintf(text, "Test adding %d of 255 components (%d, %d)", i+1, x, y);
            allPassedTowardsTheLimit &= c.AddComponent(ComponentFactory::GetBluePrint(CMPNAMES::DRAWING_COMPONENT), x, y);
            ++i;
        } while (i < 255 && allPassedTowardsTheLimit);
        test_assert(
            allPassedTowardsTheLimit,
            text, "test overflow, building up, adding 255 component");
        
        // Buffer is full now,
        // Adding to the corner should fail
        x = (i % 20) * 5, y = (i / 20) * 5;
        sprintf(text, "Test adding %d of 255 components (%d, %d)", i+1, x, y);
        test_assert(
            !(c.AddComponent(ComponentFactory::GetBluePrint(CMPNAMES::DRAWING_COMPONENT), 200, 200)),
            text, "test overflow, c.AddComponent(...). should not add component beyond allocated space");

        test_equals(c.GetCellInfo(0, 0).componentId, (std::uint8_t)1, "Test cell has component id 1");
    }
}

void test_moving_component_canvas_operations()
{
    test_section("Component movement canvas Operations Tests");
    {
        Canvas c;
        c.AddComponent(ComponentFactory::GetBluePrint(CMPNAMES::DRAWING_COMPONENT), 1,1);
        auto component = c.GetComponent(1);
        test_assert(false, "Test moving component", "not implemented yet");
    }
}

void test_canvas_network_operations()
{
    test_section("test_canvas_network_operations");
    {
        Canvas c;
        std::vector<CellPosition> anchors = {{1,1},{5,1}};
        std::uint32_t segmentId = c.AddNetworkSegment(anchors, 0);
        test_equals(segmentId, (std::uint32_t)1, "test create network 1:1->5:1 returns valid id");
        test_equals(c.GetCellInfo(0,1).networkId, (std::uint8_t)0, "test create network 1:1->5:1 inspecting position border (before start) has no network");
        test_equals(c.GetCellInfo(1,1).networkId, (std::uint8_t)1, "test create network 1:1->5:1 inspecting position border (at start) has network id");
        test_equals(c.GetCellInfo(3,1).networkId, (std::uint8_t)1, "test create network 1:1->5:1 inspecting position border (at middle) has network id");
        test_equals(c.GetCellInfo(5,1).networkId, (std::uint8_t)1, "test create network 1:1->5:1 inspecting position border (at end) has network id");
        test_equals(c.GetCellInfo(6,1).networkId, (std::uint8_t)0, "test create network 1:1->5:1 inspecting position border (after end) has no network");        
    }

    {
        Canvas c;
        std::vector<CellPosition> anchors = {{1,1},{5,1}};
        std::uint32_t segmentId1 = c.AddNetworkSegment(anchors, 0);
        test_equals(segmentId1, (std::uint32_t)1, "test create network 1:1->5:1 returns valid id (overlap test, no overlap)");
        
        std::uint32_t segmentId2 = c.AddNetworkSegment(anchors, 0);
        test_equals(segmentId2, (std::uint32_t)0, "test create network 1:1->5:1 returns zero (overlap test, full overlap)");

        std::vector<CellPosition> anchors2 = {{0,1},{2,1}};
        std::uint32_t segmentId3 = c.AddNetworkSegment(anchors2, 0);
        test_equals(segmentId3, (std::uint32_t)0, "test create network 1:1->5:1 returns zero (overlap test, partial overlap start+1)");
        
        std::vector<CellPosition> anchors3 = {{0,1},{1,1}};
        std::uint32_t segmentId4 = c.AddNetworkSegment(anchors3, 0);
        test_assert(segmentId4>0, "test create network", "1:1->5:1 + 0:1->1:1  returns gt 0 (extend network, overlap=start)");
        test_equals(c.GetCellInfo(0,1).networkId, c.GetCellInfo(1,1).networkId, "Check id of attached matches, 1:1->5:1 + 0:1->1:1 (extend network, overlap=start)" );
        test_equals(c.GetCellInfo(0,1).networkId, (std::uint32_t)1, "Check id=1 of attached matches, 1:1->5:1 + 0:1->1:1 (extend network, overlap=start)" );
    }
    {
       // A+B should be possible   , A+C should not be possible
        //   1  2  3                ,   1  2  3
        // 0                        , 0    C
        // 1 A  BA  A               , 1 A  CA  A
        // 2    B                   , 2    C
        // 3    B                   , 3
        //// Notice I consider changing behavior of A+B so it is only possible to extend at anchors
        std::vector<CellPosition> anchorsA = {{1,1},{3,1}};
        std::vector<CellPosition> anchorsB = {{2,1},{2,3}};
        std::vector<CellPosition> anchorsC = {{2,0},{2,2}};
        {
            Canvas c;
            std::uint32_t segmentIdA = c.AddNetworkSegment(anchorsA, 0);
            std::uint32_t segmentIdB = c.AddNetworkSegment(anchorsB, 0);
            test_equals(segmentIdA, (std::uint32_t)1, "canvas network overlap test, section A Added ok");
            test_equals(segmentIdB, (std::uint32_t)2, "canvas network overlap test, section B Added ok");
        }
        {
            Canvas c;
            std::uint32_t segmentIdA = c.AddNetworkSegment(anchorsA, 0);
            std::uint32_t segmentIdC = c.AddNetworkSegment(anchorsC, 0);
            test_equals(segmentIdA, (std::uint32_t)1, "canvas network overlap test, section A Added ok (fresh canvas)");
            test_equals(segmentIdC, (std::uint32_t)0, "canvas network overlap test, section C NOT Added ok");   
        }
    }

    {
       // A+B should not be possible, A+C should not be possible
        //   0  1  2   3            ,   0  1  2  3
        // 0 A     B                , 0 A        C
        // 1    BA                  , 1    A  C
        // 2 B     A                , 2    C  A
        // 3           A            , 3 C        A  
        std::vector<CellPosition> anchorsA = {{0,0},{3,3}};
        std::vector<CellPosition> anchorsB = {{0,2},{2,0}};
        std::vector<CellPosition> anchorsC = {{0,3},{3,0}};
        {
            Canvas c;
            std::uint32_t segmentIdA = c.AddNetworkSegment(anchorsA, 0);
            std::uint32_t segmentIdB = c.AddNetworkSegment(anchorsB, 0);
            std::uint32_t segmentIdC = c.AddNetworkSegment(anchorsC, 0);
            test_equals(segmentIdA, (std::uint32_t)1, "canvas network Diagonal overlap test, section A Added ok");
            test_equals(segmentIdB, (std::uint32_t)0, "canvas network Diagonal overlap test, section B should not be Added");
            test_equals(segmentIdC, (std::uint32_t)0, "canvas network Diagonal overlap test, section C should not be Added");
        }
    }
}

// Function to run all canvas tests
void run_canvas_tests()
{
    test_basic_canvas_operations();
    test_capacity_canvas_operations();
    test_moving_component_canvas_operations();
    test_canvas_network_operations();
}
