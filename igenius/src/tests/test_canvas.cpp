#include "test_framework.h"
#include "canvas/Canvas.h"  // Uncomment when you want to test actual Canvas class


void test_canvas_operations() {
    test_section("Canvas Operations Tests");

    {
        Canvas c;
        test_equals(c.GetCellInfo({0,0}).componentId, (std::uint8_t)0, "Test cell has zero component id");
    }
    {
        Canvas c;
        test_assert(
            c.AddComponent(ComponentFactory::GetBluePrint(CMPNAMES::DRAWING_COMPONENT), 0,0),
            "test adding component", 
            "c.AddComponent(ComponentFactory::GetBluePrint(CMPNAMES::DRAWING_COMPONENT), 0,0)" );
        test_equals(c.GetCellInfo({0,0}).componentId, (std::uint8_t)1, "Test cell has component id 1");    
    }

    {
        Canvas c;
        c.AddComponent(ComponentFactory::GetBluePrint(CMPNAMES::DRAWING_COMPONENT), 0,0);
        test_assert(
            !(c.AddComponent(ComponentFactory::GetBluePrint(CMPNAMES::DRAWING_COMPONENT), 0,0)),
            "test adding component on top of component fails", 
            "c.AddComponent(ComponentFactory::GetBluePrint(CMPNAMES::DRAWING_COMPONENT), 0,0)" );
        test_equals(c.GetCellInfo({0,0}).componentId, (std::uint8_t)1, "Test cell has component id 1 (After add on top)");
    }

    {
        Canvas c;
        c.AddComponent(ComponentFactory::GetBluePrint(CMPNAMES::DRAWING_COMPONENT), 0,0);
        
        test_assert(
            c.ReleaseComponent(1),
            "test remove component thats there", 
            "c.AddComponent(ComponentFactory::GetBluePrint(CMPNAMES::DRAWING_COMPONENT), 0,0)" );
        test_equals(c.GetCellInfo({0,0}).componentId, (std::uint8_t)0, "Test cell has component id 0 (after remove)");    
    }
       {
        Canvas c;
        test_assert(
            !(c.ReleaseComponent(1)),
            "test remove component thats not there", 
            "c.AddComponent(ComponentFactory::GetBluePrint(CMPNAMES::DRAWING_COMPONENT), 0,0)" );
        test_equals(c.GetCellInfo({0,0}).componentId, (std::uint8_t)0, "Test cell has component id 0 (after remove)");    
    }
}


// Function to run all canvas tests
void run_canvas_tests() {
    test_canvas_operations();
}
