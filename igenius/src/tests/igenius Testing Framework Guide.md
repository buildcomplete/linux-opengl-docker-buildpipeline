# igenius Testing Framework Guide

This document describes the simple but powerful testing framework integrated into the igenius project.

## Features

The testing framework provides the following features:

### **Colored Output**
- **PASS** results are displayed in **green**
- **FAIL** results are displayed in **red**
- Uses ANSI color codes that work in most modern terminals

### **Multiple Test Functions**

#### `test_assert(condition, test_name, condition_str)`
Basic boolean assertion testing with colored output.

```cpp
test_assert(2 + 2 == 4, "Addition", "2 + 2 == 4");
```

#### `test_equals<T>(actual, expected, test_name)`
Generic template function that compares actual vs expected values. When a test fails, it shows both the expected and actual values.

```cpp
test_equals(2 + 2, 4, "Addition with equals check");
// On failure, shows:
// FAIL: Addition with equals check
//   Expected: 4
//   Actual:   5
```

#### `test_equals_string(actual, expected, test_name)`
Specialized version for string comparisons with proper quote formatting.

```cpp
test_equals_string(text.substr(0, 5), "Hello", "String substring check");
// On failure, shows:
// FAIL: String substring check
//   Expected: "Hello"
//   Actual:   "Hi"
```

## How to Add New Tests

### 1. Create a New Test Function

```cpp
void test_my_feature() {
    std::cout << "\n--- My Feature Tests ---" << std::endl;
    
    // Add your tests here
    test_equals(my_function(5), 10, "My function test");
    test_assert(my_condition(), "My condition", "my_condition()");
}
```

### 2. Call Your Test Function in main()

```cpp
int main() {
    std::cout << "Running igenius unit tests..." << std::endl;
    std::cout << "===============================" << std::endl;
    
    test_basic_arithmetic();
    test_boolean_operations();
    test_string_operations();
    test_numeric_comparisons();
    test_my_feature();  // Add your test function here
    
    std::cout << "\n===============================" << std::endl;
    std::cout << "All tests completed!" << std::endl;
    
    return 0;
}
```

## Testing Your Project Components

To test actual igenius components, you can include the necessary headers and test them:

```cpp
#include "Engine.h"  // Include your project headers

void test_engine_components() {
    std::cout << "\n--- Engine Component Tests ---" << std::endl;
    
    // Example tests:
    Engine engine;
    test_equals(engine.getSomeValue(), expectedValue, "Engine initialization test");
    test_assert(engine.isValid(), "Engine validity", "engine.isValid()");
}
```

## Running the Tests

Use the make targets to build and run tests:

```bash
# Run debug tests
make test

# Run release tests  
make test-release

# Just build tests without running
make config=debug_x64 igenius_tests
```

## Best Practices

1. **Group related tests** into separate functions
2. **Use descriptive test names** that explain what is being tested
3. **Use `test_equals()`** when you want to see actual vs expected values on failure
4. **Use `test_assert()`** for simple boolean conditions
5. **Use `test_equals_string()`** for string comparisons to get better formatted output
6. **Add comments** to explain complex test scenarios

## Example Output

```
Running igenius unit tests...
===============================

--- Basic Arithmetic Tests ---
PASS: Addition - 2 + 2 == 4
PASS: Multiplication - 5 * 3 == 15
FAIL: Division with equals check
  Expected: 5
  Actual:   4

--- String Operations Tests ---
PASS: String length check
FAIL: String substring check
  Expected: "Hello"
  Actual:   "Hi"

===============================
All tests completed!
```

The framework is designed to be simple, require no external dependencies, and provide clear, actionable feedback when tests fail.
