#include "test_framework.h"
#include "canvas/Canvas.h" // Uncomment when you want to test actual Canvas class

void test_basic_canvas_operations()
{
    test_section("Basic Canvas Operations Tests");
    {
        Canvas c;
        test_equals(c.GetCellInfo({0, 0}).componentId, (std::uint8_t)0, "Test cell has zero component id");
    }
    {
        Canvas c;
        test_assert(
            c.AddComponent(ComponentFactory::GetBluePrint(CMPNAMES::DRAWING_COMPONENT), 0, 0),
            "test adding component",
            "c.AddComponent(...");
        test_equals(c.GetCellInfo({0, 0}).componentId, (std::uint8_t)1, "Test cell has component id 1");
    }

    {
        Canvas c;
        c.AddComponent(ComponentFactory::GetBluePrint(CMPNAMES::DRAWING_COMPONENT), 0, 0);
        test_assert(
            !(c.AddComponent(ComponentFactory::GetBluePrint(CMPNAMES::DRAWING_COMPONENT), 0, 0)),
            "test adding component on top of component fails",
            "c.AddComponent(...");
        test_equals(c.GetCellInfo({0, 0}).componentId, (std::uint8_t)1, "Test cell has component id 1 (After add on top)");
    }

    {
        Canvas c;
        c.AddComponent(ComponentFactory::GetBluePrint(CMPNAMES::DRAWING_COMPONENT), 0, 0);
        test_assert(
            c.ReleaseComponent(1),
            "test remove component thats there (0,0)",
            "c.AddComponent(...");
        test_equals(c.GetCellInfo({0, 0}).componentId, (std::uint8_t)0, "Test cell has component id 0 (after remove)");
    }
    {
        Canvas c;
        test_assert(
            !(c.ReleaseComponent(1)),
            "test remove component thats not there (0,0)",
            "c.AddComponent(...");
        test_equals(c.GetCellInfo({0, 0}).componentId, (std::uint8_t)0, "Test cell has component id 0 (after remove)");
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

        test_equals(c.GetCellInfo({0, 0}).componentId, (std::uint8_t)1, "Test cell has component id 1");
    }
}

void test_moving_component_canvas_operations()
{
    test_section("Component movement canvas Operations Tests");
    {
        Canvas c;
        c.AddComponent(ComponentFactory::GetBluePrint(CMPNAMES::DRAWING_COMPONENT), 1,1);
        auto component = c.GetComponent(1);


    }
}

// Function to run all canvas tests
void run_canvas_tests()
{
    test_basic_canvas_operations();
    test_capacity_canvas_operations();
    test_moving_component_canvas_operations();
}
