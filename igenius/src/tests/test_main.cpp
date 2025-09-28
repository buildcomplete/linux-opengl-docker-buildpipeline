#include "test_framework.h"

// Include test function declarations
void run_basic_tests();
void run_canvas_tests();
void run_engine_tests();
void run_byte_value_tests();

int main() {
    // Reset counters at the start
    TestCounters::instance().reset();
    
    std::cout << "Running igenius unit tests..." << std::endl;
    std::cout << "===============================" << std::endl;
    
    // Run tests from different modules
    run_basic_tests();
    run_canvas_tests();
    run_engine_tests();
    run_byte_value_tests();
    
    // Add more test modules here as you create them:
    // run_component_tests();
    // run_ui_tests();
    // run_network_tests();
    
    // Print test summary
    TestCounters::instance().print_summary();
    
    // Return non-zero exit code if any tests failed
    return (TestCounters::instance().get_failed() > 0) ? 1 : 0;
}
