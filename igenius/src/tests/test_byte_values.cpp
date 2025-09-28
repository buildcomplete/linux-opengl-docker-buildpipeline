#include "test_framework.h"
#include <cstdint>

// Test byte value handling
void test_uint8_values() {
    test_section("uint8_t Value Tests");
    
    std::uint8_t value1 = 255;
    std::uint8_t value2 = 128;
    std::uint8_t value3 = 0;
    
    // These should pass
    test_equals(value1, static_cast<std::uint8_t>(255), "Max uint8_t value");
    test_equals(value2, static_cast<std::uint8_t>(128), "Mid uint8_t value");
    test_equals(value3, static_cast<std::uint8_t>(0), "Min uint8_t value");
    
    // This should fail to demonstrate the output format
    test_equals(value1, static_cast<std::uint8_t>(200), "Intentional uint8_t failure");
}

void test_int8_values() {
    test_section("int8_t Value Tests");
    
    std::int8_t value1 = 127;
    std::int8_t value2 = -128;
    std::int8_t value3 = 0;
    
    // These should pass
    test_equals(value1, static_cast<std::int8_t>(127), "Max int8_t value");
    test_equals(value2, static_cast<std::int8_t>(-128), "Min int8_t value");
    test_equals(value3, static_cast<std::int8_t>(0), "Zero int8_t value");
    
    // This should fail to demonstrate the output format
    test_equals(value1, static_cast<std::int8_t>(100), "Intentional int8_t failure");
}

void test_unsigned_char_values() {
    test_section("unsigned char Value Tests");
    
    unsigned char value1 = 255;
    unsigned char value2 = 65; // 'A'
    
    // These should pass
    test_equals(value1, static_cast<unsigned char>(255), "Max unsigned char value");
    test_equals(value2, static_cast<unsigned char>(65), "ASCII 'A' value");
    
    // This should fail to demonstrate the output format
    test_equals(value1, static_cast<unsigned char>(100), "Intentional unsigned char failure");
}

void test_regular_int_values() {
    test_section("Regular int Value Tests (should work as before)");
    
    int value1 = 42;
    int value2 = -100;
    
    // These should pass and show normal int output
    test_equals(value1, 42, "Regular int value");
    test_equals(value2, -100, "Negative int value");
    
    // This should fail with normal int output
    test_equals(value1, 50, "Intentional int failure");
}

// Function to run all byte value tests
void run_byte_value_tests() {
    test_uint8_values();
    test_int8_values();
    test_unsigned_char_values();
    test_regular_int_values();
}
