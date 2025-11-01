#include "test_framework.h"
#include "canvas/Canvas.h"  // Uncomment when you want to test actual Canvas class


void test_canvas_operations() {
    test_section("Canvas Operations Tests");

    Canvas c;
    test_equals(c.GetCellInfo({0,0}).componentId, (std::uint8_t)0, "Test cell has zero component id");
    
}


// Function to run all canvas tests
void run_canvas_tests() {
    test_canvas_operations();
}
