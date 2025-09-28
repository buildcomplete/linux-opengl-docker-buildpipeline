#include <iostream>
#include <string>
#include <cassert>
#include <sstream>

// ANSI color codes
#define COLOR_GREEN "\033[32m"
#define COLOR_RED "\033[31m"
#define COLOR_RESET "\033[0m"

// Simple test helper function with colored output
void test_assert(bool condition, const std::string& test_name, const std::string& condition_str) {
    if (condition) {
        std::cout << COLOR_GREEN << "PASS" << COLOR_RESET << ": " << test_name << " - " << condition_str << std::endl;
    } else {
        std::cout << COLOR_RED << "FAIL" << COLOR_RESET << ": " << test_name << " - " << condition_str << std::endl;
    }
}

// Enhanced test helper function that shows actual vs expected values
template<typename T>
void test_equals(const T& actual, const T& expected, const std::string& test_name) {
    if (actual == expected) {
        std::cout << COLOR_GREEN << "PASS" << COLOR_RESET << ": " << test_name << std::endl;
    } else {
        std::cout << COLOR_RED << "FAIL" << COLOR_RESET << ": " << test_name << std::endl;
        std::cout << "  Expected: " << expected << std::endl;
        std::cout << "  Actual:   " << actual << std::endl;
    }
}

// Specialized version for string comparisons to handle better formatting
void test_equals_string(const std::string& actual, const std::string& expected, const std::string& test_name) {
    if (actual == expected) {
        std::cout << COLOR_GREEN << "PASS" << COLOR_RESET << ": " << test_name << std::endl;
    } else {
        std::cout << COLOR_RED << "FAIL" << COLOR_RESET << ": " << test_name << std::endl;
        std::cout << "  Expected: \"" << expected << "\"" << std::endl;
        std::cout << "  Actual:   \"" << actual << "\"" << std::endl;
    }
}

// Simple test functions - just regular C++ functions
void test_basic_arithmetic() {
    std::cout << "\n--- Basic Arithmetic Tests ---" << std::endl;
    
    // Using the simple test_assert function
    test_assert(2 + 2 == 4, "Addition", "2 + 2 == 4");
    test_assert(5 * 3 == 15, "Multiplication", "5 * 3 == 15");
    test_assert(10 / 2 == 5, "Division", "10 / 2 == 5");
    test_assert(7 - 3 == 4, "Subtraction", "7 - 3 == 4");
    
    // Using the enhanced test_equals function
    test_equals(2 + 2, 4, "Addition with equals check");
    test_equals(5 * 3, 15, "Multiplication with equals check");
    
    // Example of a failing test to show the output format
    test_equals(2 + 2, 5, "Intentional failure example");
}

void test_boolean_operations() {
    std::cout << "\n--- Boolean Operations Tests ---" << std::endl;
    
    test_assert(true == true, "Boolean equality", "true == true");
    test_assert(false == false, "Boolean equality", "false == false");
    test_assert((true && false) == false, "Boolean AND", "(true && false) == false");
    test_assert((true || false) == true, "Boolean OR", "(true || false) == true");
    test_assert(!false == true, "Boolean NOT", "!false == true");
    
    // Using test_equals for boolean values
    test_equals(true && true, true, "Boolean AND with equals check");
    test_equals(false || false, false, "Boolean OR with equals check");
}

void test_string_operations() {
    std::cout << "\n--- String Operations Tests ---" << std::endl;
    
    std::string test_string = "Hello World";
    
    // Using test_equals for numeric comparisons
    test_equals(test_string.length(), static_cast<size_t>(11), "String length check");
    
    // Using test_equals_string for string comparisons
    test_equals_string(test_string.substr(0, 5), "Hello", "String substring check");
    
    // Regular assertion for find operations
    test_assert(test_string.find("World") != std::string::npos, "String find", "test_string.find(\"World\") found");
    
    std::string empty_string = "";
    test_assert(empty_string.empty(), "Empty string", "empty_string.empty()");
    
    // Example of a failing string test
    test_equals_string(test_string.substr(0, 5), "Hi", "Intentional string failure example");
}

void test_numeric_comparisons() {
    std::cout << "\n--- Numeric Comparison Tests ---" << std::endl;
    
    // Test various numeric types
    test_equals(42, 42, "Integer equality");
    test_equals(3.14, 3.14, "Double equality");
    test_equals(100L, 100L, "Long equality");
    
    // Examples of failures to show the output
    test_equals(10, 20, "Integer inequality example");
    test_equals(2.5, 3.5, "Double inequality example");
}

// Example of how you could test actual project components
// void test_engine_components() {
//     std::cout << "\n--- Engine Component Tests ---" << std::endl;
//     
//     // Example using test_equals:
//     // SomeEngineClass engine;
//     // test_equals(engine.getSomeValue(), expectedValue, "Engine value test");
//     
//     // Example using test_equals_string:
//     // test_equals_string(engine.getName(), "ExpectedName", "Engine name test");
// }

int main() {
    std::cout << "Running igenius unit tests..." << std::endl;
    std::cout << "===============================" << std::endl;
    
    // Just call the test functions directly - simple and clear
    test_basic_arithmetic();
    test_boolean_operations();
    test_string_operations();
    test_numeric_comparisons();
    
    // Add more test function calls here as needed:
    // test_engine_components();
    
    std::cout << "\n===============================" << std::endl;
    std::cout << "All tests completed!" << std::endl;
    
    return 0;
}
