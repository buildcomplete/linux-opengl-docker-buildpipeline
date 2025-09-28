#include "test_framework.h"

// Include test function declarations
void run_basic_tests();
void run_canvas_tests();
void run_engine_tests();

int main() {
    std::cout << "Running igenius unit tests..." << std::endl;
    std::cout << "===============================" << std::endl;
    
    // Run tests from different modules
    run_basic_tests();
    run_canvas_tests();
    run_engine_tests();
    
    // Add more test modules here as you create them:
    // run_component_tests();
    // run_ui_tests();
    // run_network_tests();
    
    std::cout << "\n===============================" << std::endl;
    std::cout << "All tests completed!" << std::endl;
    
    return 0;
}
