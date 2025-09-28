#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <cstdint>
#include <iomanip>

// ANSI color codes
#define COLOR_GREEN "\033[32m"
#define COLOR_RED "\033[31m"
#define COLOR_RESET "\033[0m"

// Simple test helper function with colored output
inline void test_assert(bool condition, const std::string& test_name, const std::string& condition_str) {
    if (condition) {
        std::cout << COLOR_GREEN << "PASS" << COLOR_RESET << ": " << test_name << " - " << condition_str << std::endl;
    } else {
        std::cout << COLOR_RED << "FAIL" << COLOR_RESET << ": " << test_name << " - " << condition_str << std::endl;
    }
}

// Enhanced test helper function that shows actual vs expected values
template<typename T>
inline void test_equals(const T& actual, const T& expected, const std::string& test_name) {
    if (actual == expected) {
        std::cout << COLOR_GREEN << "PASS" << COLOR_RESET << ": " << test_name << std::endl;
    } else {
        std::cout << COLOR_RED << "FAIL" << COLOR_RESET << ": " << test_name << std::endl;
        std::cout << "  Expected: " << expected << std::endl;
        std::cout << "  Actual:   " << actual << std::endl;
    }
}

// Specialized version for unsigned char to display as numeric values instead of characters
inline void test_equals(const unsigned char& actual, const unsigned char& expected, const std::string& test_name) {
    if (actual == expected) {
        std::cout << COLOR_GREEN << "PASS" << COLOR_RESET << ": " << test_name << std::endl;
    } else {
        std::cout << COLOR_RED << "FAIL" << COLOR_RESET << ": " << test_name << std::endl;
        std::cout << "  Expected: " << static_cast<unsigned int>(expected) << " (0x" << std::hex << static_cast<unsigned int>(expected) << std::dec << ")" << std::endl;
        std::cout << "  Actual:   " << static_cast<unsigned int>(actual) << " (0x" << std::hex << static_cast<unsigned int>(actual) << std::dec << ")" << std::endl;
    }
}

// Specialized version for signed char to display as numeric values instead of characters
inline void test_equals(const signed char& actual, const signed char& expected, const std::string& test_name) {
    if (actual == expected) {
        std::cout << COLOR_GREEN << "PASS" << COLOR_RESET << ": " << test_name << std::endl;
    } else {
        std::cout << COLOR_RED << "FAIL" << COLOR_RESET << ": " << test_name << std::endl;
        std::cout << "  Expected: " << static_cast<int>(expected) << " (0x" << std::hex << static_cast<unsigned int>(static_cast<unsigned char>(expected)) << std::dec << ")" << std::endl;
        std::cout << "  Actual:   " << static_cast<int>(actual) << " (0x" << std::hex << static_cast<unsigned int>(static_cast<unsigned char>(actual)) << std::dec << ")" << std::endl;
    }
}

// Specialized version for string comparisons to handle better formatting
inline void test_equals_string(const std::string& actual, const std::string& expected, const std::string& test_name) {
    if (actual == expected) {
        std::cout << COLOR_GREEN << "PASS" << COLOR_RESET << ": " << test_name << std::endl;
    } else {
        std::cout << COLOR_RED << "FAIL" << COLOR_RESET << ": " << test_name << std::endl;
        std::cout << "  Expected: \"" << expected << "\"" << std::endl;
        std::cout << "  Actual:   \"" << actual << "\"" << std::endl;
    }
}

// Helper function to print test section headers
inline void test_section(const std::string& section_name) {
    std::cout << "\n--- " << section_name << " ---" << std::endl;
}
