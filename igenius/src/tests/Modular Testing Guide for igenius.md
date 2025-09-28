# Modular Testing Guide for igenius

This guide explains how to use the modular test structure that allows you to organize tests by class/component in separate files.

## File Structure

The test framework is now organized as follows:

```
src/tests/
├── test_framework.h     # Shared test helper functions
├── test_main.cpp        # Main test runner
├── test_basic.cpp       # Basic functionality tests
├── test_canvas.cpp      # Canvas class tests
├── test_engine.cpp      # Engine class tests
└── test_[component].cpp # Additional component tests
```

## Shared Test Framework (test_framework.h)

The `test_framework.h` header contains all the test helper functions:

### Available Functions

- **`test_assert(condition, test_name, condition_str)`** - Basic boolean assertions
- **`test_equals<T>(actual, expected, test_name)`** - Value comparisons with detailed failure output
- **`test_equals_string(actual, expected, test_name)`** - String comparisons with proper formatting
- **`test_section(section_name)`** - Prints formatted section headers

### Features

- **Colored output** (green PASS, red FAIL)
- **Detailed failure reporting** showing expected vs actual values
- **Header-only design** with `inline` functions to avoid linking issues

## Creating New Test Files

### 1. Create the Test File

Create a new file `src/tests/test_[component].cpp`:

```cpp
#include "test_framework.h"
#include "YourClass.h"  // Include the class you want to test

void test_your_class_creation() {
    test_section("YourClass Creation Tests");
    
    YourClass obj;
    test_assert(obj.isValid(), "Object validity", "obj.isValid()");
    test_equals(obj.getValue(), 42, "Default value check");
}

void test_your_class_operations() {
    test_section("YourClass Operations Tests");
    
    YourClass obj;
    obj.setValue(100);
    test_equals(obj.getValue(), 100, "Set value check");
    
    obj.doSomething();
    test_assert(obj.hasChanged(), "State change", "obj.hasChanged()");
}

// Main function to run all tests for this class
void run_your_class_tests() {
    test_your_class_creation();
    test_your_class_operations();
}
```

### 2. Add to Main Test Runner

Edit `src/tests/test_main.cpp` to include your new tests:

```cpp
#include "test_framework.h"

// Add declaration for your test function
void run_your_class_tests();

int main() {
    std::cout << "Running igenius unit tests..." << std::endl;
    std::cout << "===============================" << std::endl;
    
    run_basic_tests();
    run_canvas_tests();
    run_engine_tests();
    run_your_class_tests();  // Add your test function here
    
    std::cout << "\n===============================" << std::endl;
    std::cout << "All tests completed!" << std::endl;
    
    return 0;
}
```

### 3. Regenerate Build Files

After adding new test files, regenerate the build configuration:

```bash
cd build
./premake5 gmake2
cd ..
```

### 4. Run Tests

```bash
./run_tests.sh
```

## Example: Testing Canvas Class

Here's how you would modify `test_canvas.cpp` to test the actual Canvas class:

```cpp
#include "test_framework.h"
#include "Canvas.h"  // Include actual Canvas header

void test_canvas_creation() {
    test_section("Canvas Creation Tests");
    
    Canvas canvas;
    test_assert(canvas.isValid(), "Canvas validity", "canvas.isValid()");
    test_equals(canvas.getWidth(), 800, "Default canvas width");
    test_equals(canvas.getHeight(), 600, "Default canvas height");
}

void test_canvas_operations() {
    test_section("Canvas Operations Tests");
    
    Canvas canvas(1024, 768);
    canvas.clear();
    test_assert(canvas.isEmpty(), "Canvas clear", "canvas.isEmpty()");
    
    // Test drawing operations
    canvas.drawPixel(10, 10, Color::RED);
    test_assert(!canvas.isEmpty(), "Canvas draw pixel", "!canvas.isEmpty()");
}

void run_canvas_tests() {
    test_canvas_creation();
    test_canvas_operations();
}
```

## Best Practices

### 1. Organize by Component
- One test file per major class or component
- Group related functionality within each file
- Use descriptive function names like `test_[class]_[functionality]()`

### 2. Use Appropriate Test Functions
- `test_assert()` for boolean conditions
- `test_equals()` for value comparisons (shows actual vs expected on failure)
- `test_equals_string()` for string comparisons (better formatting)

### 3. Structure Your Tests
- Start each test group with `test_section()` for clear output
- Test creation/initialization first
- Then test operations and edge cases
- Include both positive and negative test cases

### 4. Include Headers Properly
- Always include `test_framework.h` first
- Include the headers for classes you're testing
- Use forward declarations when possible to reduce compile time

#### Forward Declarations Explained
Forward declarations allow you to declare that a class exists without including its full header file. This can significantly reduce compile times in larger projects.

**Example without forward declaration:**
```cpp
#include "test_framework.h"
#include "Engine.h"        // Full header - includes all Engine dependencies
#include "Canvas.h"        // Full header - includes all Canvas dependencies
#include "ComponentFactory.h"  // Full header - includes all ComponentFactory dependencies

void run_engine_tests() {
    // Tests here
}
```

**Example with forward declarations:**
```cpp
#include "test_framework.h"

// Forward declarations - just tell compiler these classes exist
class Engine;
class Canvas;
class ComponentFactory;

// Only include headers for classes you actually instantiate or call methods on
#include "Engine.h"  // Only include this one since we create Engine objects

void run_engine_tests() {
    Engine engine;  // We create an Engine, so we need the full header
    // Canvas* canvas = engine.getCanvas();  // Just using pointer, forward declaration is enough
    // ComponentFactory* factory = nullptr;  // Just declaring pointer, forward declaration is enough
}
```

**When to use forward declarations:**
- When you only use pointers or references to a class
- When you only pass objects as function parameters
- When you declare but don't instantiate objects in the header

**When you need full headers:**
- When you create objects (call constructors)
- When you call methods on objects
- When you access member variables
- When you inherit from a class

### 5. Naming Conventions
- Test files: `test_[component].cpp`
- Test functions: `test_[component]_[functionality]()`
- Runner functions: `run_[component]_tests()`

## Running Specific Test Categories

While the current setup runs all tests, you can easily modify `test_main.cpp` to run only specific test categories during development:

```cpp
int main() {
    std::cout << "Running igenius unit tests..." << std::endl;
    std::cout << "===============================" << std::endl;
    
    // Comment out tests you don't want to run during development
    // run_basic_tests();
    run_canvas_tests();  // Only run canvas tests
    // run_engine_tests();
    
    std::cout << "\n===============================" << std::endl;
    std::cout << "All tests completed!" << std::endl;
    
    return 0;
}
```

## Troubleshooting

### Build Errors After Adding New Files
- Make sure to regenerate build files: `cd build && ./premake5 gmake2`
- Check that function declarations match between files
- Ensure all `#include` statements are correct

### Linking Errors
- Verify that the `run_[component]_tests()` function is declared in `test_main.cpp`
- Make sure the function is defined in the corresponding test file
- Check that all functions are properly named and spelled correctly

This modular approach makes it easy to organize tests by component while sharing the same powerful testing framework across all test files.
