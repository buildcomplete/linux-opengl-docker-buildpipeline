#include "test_framework.h"
#include <string>

// Basic functionality tests (arithmetic, strings, etc.)
void test_basic_arithmetic() {
    test_section("Basic Arithmetic Tests");
    
    // Using the simple test_assert function
    test_assert(2 + 2 == 4, "Addition", "2 + 2 == 4");
    test_assert(5 * 3 == 15, "Multiplication", "5 * 3 == 15");
    test_assert(10 / 2 == 5, "Division", "10 / 2 == 5");
    test_assert(7 - 3 == 4, "Subtraction", "7 - 3 == 4");
    
    // Using the enhanced test_equals function
    test_equals(2 + 2, 4, "Addition with equals check");
    test_equals(5 * 3, 15, "Multiplication with equals check");
    test_equals(8 / 4, 2, "Division with equals check");
}

void test_boolean_operations() {
    test_section("Boolean Operations Tests");
    
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
    test_section("String Operations Tests");
    
    std::string test_string = "Hello World";
    
    // Using test_equals for numeric comparisons
    test_equals(test_string.length(), static_cast<size_t>(11), "String length check");
    
    // Using test_equals_string for string comparisons
    test_equals_string(test_string.substr(0, 5), "Hello", "String substring check");
    test_equals_string(test_string.substr(6), "World", "String substring from position");
    
    // Regular assertion for find operations
    test_assert(test_string.find("World") != std::string::npos, "String find", "test_string.find(\"World\") found");
    test_assert(test_string.find("xyz") == std::string::npos, "String not found", "test_string.find(\"xyz\") not found");
    
    std::string empty_string = "";
    test_assert(empty_string.empty(), "Empty string", "empty_string.empty()");
}

void test_numeric_comparisons() {
    test_section("Numeric Comparison Tests");
    
    // Test various numeric types
    test_equals(42, 42, "Integer equality");
    test_equals(3.14, 3.14, "Double equality");
    test_equals(100L, 100L, "Long equality");
    
    // Test some calculations
    test_equals(10 * 5, 50, "Integer multiplication");
    test_equals(100 - 25, 75, "Integer subtraction");
}

// Function to run all basic tests
void run_basic_tests() {
    test_basic_arithmetic();
    test_boolean_operations();
    test_string_operations();
    test_numeric_comparisons();
}
