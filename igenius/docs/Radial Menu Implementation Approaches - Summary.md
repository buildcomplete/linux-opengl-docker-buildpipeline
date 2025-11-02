# Radial Menu Implementation Approaches - Summary

## Overview

Comparison of four approaches for implementing type-safe radial menu segments with data.


## Detailed Comparison Table
TL;DR;

Notice, all numbers are directional estimates, would love to have real numbers :)

| Aspect | Index-Only | **KISS (int)** ✅ | Template | Hybrid |
|--------|------------|-------------------|----------|--------|
| **Complexity** | Lowest | Low | High | Highest |
| **Binary Size** | 2.5KB | 3KB | 9KB | 3.5KB |
| **Memory/Menu** | 60 bytes | 80 bytes | 80 bytes | 80 bytes |
| **Type Safety** | None | Manual | Compile-time | Compile-time |
| **Casting Required** | No | Yes | No | No |
| **Code in .cpp** | Yes | Yes | No | Partial |
| **Self-Documenting** | No | Somewhat | Yes | Yes |
| **Compilation Issues** | No | No | Yes | Yes |
| **Virtual Calls** | No | No | No | Yes |
| **Lines of Code** | ~100 | ~120 | ~180 | ~220 |
| **Files Required** | 2 | 2 | 3 | 4 |
| **Learning Curve** | Easy | Easy | Medium | Hard |

---

## Performance Comparison

### Runtime Performance (per frame when menu visible)

| Approach | Click Handler | Draw Icons | Total |
|----------|---------------|------------|-------|
| **Index-Only** | ~20 cycles | ~50 cycles | ~70 cycles |
| **KISS (int)** | ~25 cycles | ~55 cycles | ~80 cycles |
| **Template** | ~25 cycles | ~55 cycles | ~80 cycles |
| **Hybrid** | ~30 cycles | ~80 cycles | ~110 cycles |

**Difference**: Negligible (all < 0.00001ms on 3GHz CPU)

### Binary Size (3 menu types: Mode, Convolution, Settings)

| Approach | Code Size | Savings vs Template |
|----------|-----------|---------------------|
| **Index-Only** | 2.5KB | 72% smaller |
| **KISS (int)** | 3KB | 67% smaller |
| **Template** | 9KB | baseline |
| **Hybrid** | 3.5KB | 61% smaller |

---

## Decision Matrix

### Choose Index-Only If:
- ❌ Very simple menus (2-3 segments)
- ❌ Don't need data in drawing
- ❌ Actions are trivial

### Choose KISS (int data) If: ✅
- ✅ **Embedded system** (binary size matters)
- ✅ **Few menus** (3-10 types)
- ✅ **Simplicity preferred**
- ✅ **Controlled codebase**
- ✅ **Enums fit in int**

### Choose Template If:
- ❌ Large project (many developers)
- ❌ Type safety critical
- ❌ Complex data types
- ❌ Binary size not critical

### Choose Hybrid If:
- ❌ Need type safety AND small binary
- ❌ Many menu types (>10)
- ❌ Willing to accept complexity
- ❌ Virtual call overhead acceptable

---

## Selected Approach: KISS (int data) ✅

### Why Selected

1. **Project Requirements**
   - Embedded system → binary size critical
   - Few menus → simplicity preferred
   - Controlled codebase → type safety less critical

2. **Technical Benefits**
   - Small binary (3KB vs 9KB template)
   - Simple implementation (no templates)
   - All code in .cpp files (no compilation issues)
   - Flexible (works with any enum)

3. **Trade-offs Accepted**
   - Manual casting (acceptable for controlled codebase)
   - No compile-time type safety (mitigated by code review)

**Decision**: **KISS (int data) approach selected** ✅

---

## The Four Approaches

### 1. Index-Only (Original)
No data storage, use segment index to determine action.

### 2. KISS: int data (Selected ✅)
Store enum as integer, cast when needed.

### 3. Template: Typed data
Use templates for compile-time type safety.

### 4. Hybrid: Template + Non-Templated Base
Optimize template approach to reduce binary bloat.

---

## Approach 1: Index-Only

### Implementation

```cpp
class UI_RadialMenu : public UI_Selector {
public:
    using ClickHandler = std::function<void(int segmentIndex)>;
    // No data storage
};

// Usage
auto menu = std::make_unique<UI_RadialMenu>(
    [this](int idx) {
        switch (idx) {
            case 0: SetInputMode(IG_INPUT_MODE_SELECTING); break;
            case 1: SetInputMode(IG_INPUT_MODE_DRAW_NETWORK); break;
            case 2: SetInputMode(IG_INPUT_MODE_PLACE_COMPONENT); break;
            // ...
        }
    },
    { /* segments with just colors */ }
);
```

### Characteristics

| Aspect | Rating | Notes |
|--------|--------|-------|
| **Complexity** | ⭐⭐⭐⭐⭐ | Simplest possible |
| **Binary Size** | ⭐⭐⭐⭐⭐ | Smallest (~2.5KB) |
| **Type Safety** | ⭐ | None |
| **Maintainability** | ⭐⭐ | Positional dependency |
| **Flexibility** | ⭐⭐ | Can't access data in DrawSegmentIcon |

### Pros ✅
- Simplest implementation
- Smallest binary
- No data storage overhead

### Cons ❌
- Manual switch in callback
- Positional dependency (fragile)
- Can't access data when drawing icons
- Not self-documenting

### Use When
- Very simple menus (2-3 segments)
- Actions are trivial
- Don't need to draw based on data

---

## Approach 2: KISS - int data (Selected ✅)

### Implementation

```cpp
struct RadialMenuSegment {
    Color edgeColor, backgroundColor, hoverColor;
    int data;  // Enum stored as int
};

class UI_RadialMenu : public UI_Selector {
public:
    using ClickHandler = std::function<void(int segmentIndex, int data)>;
    
    virtual void DrawSegmentIcon(..., int segmentIndex, int data, ...) const {
        // Override in derived classes, cast data to enum
    }

protected:
    std::vector<RadialMenuSegment> segments;
};

// Usage
auto menu = std::make_unique<Engine_ModeSelector>(
    [this](int idx, int data) {
        SetInputMode(static_cast<INPUT_STATE_FLAGS>(data));
    }
);

// Drawing
void DrawSegmentIcon(..., int data, ...) const override {
    INPUT_STATE_FLAGS mode = static_cast<INPUT_STATE_FLAGS>(data);
    switch (mode) {
        case IG_INPUT_MODE_SELECTING: DrawText("Select", ...); break;
        // ...
    }
}
```

### Characteristics

| Aspect | Rating | Notes |
|--------|--------|-------|
| **Complexity** | ⭐⭐⭐⭐ | Simple, minimal casting |
| **Binary Size** | ⭐⭐⭐⭐⭐ | Small (~3KB) |
| **Type Safety** | ⭐⭐ | Manual casting required |
| **Maintainability** | ⭐⭐⭐⭐ | Data with segments |
| **Flexibility** | ⭐⭐⭐⭐ | Works with any enum |

### Pros ✅
- Simple - no templates
- Small binary - single implementation
- Flexible - works with any enum that fits in int
- Data available in callbacks and drawing
- Easy to understand
- All code in .cpp files (no template issues)

### Cons ❌
- Manual casting required
- No compile-time type safety
- Can accidentally use wrong enum type
- Less self-documenting than templates

### Use When ✅
- **Embedded systems** (binary size matters)
- **Few menu types** (3-10)
- **Simplicity preferred** over type safety
- **You control all code** (less risk of wrong types)
- **Enums fit in int** (most cases)

### Why Selected for This Project ✅

1. **Embedded system** - binary size critical
2. **Few menus** - mode selector, convolution, etc.
3. **Controlled codebase** - low risk of type errors
4. **Simplicity** - easier to maintain
5. **No template complexity** - no compilation issues

---

## Approach 3: Template - Typed Data

### Implementation

```cpp
template<typename T>
struct RadialMenuSegment {
    Color edgeColor, backgroundColor, hoverColor;
    T data;  // Typed data
};

template<typename T>
class UI_RadialMenu : public UI_Selector {
public:
    using ClickHandler = std::function<void(int segmentIndex, const T& data)>;
    
    virtual void DrawSegmentIconTyped(..., int segmentIndex, const T& data, ...) const {
        // Override in derived classes, data is typed!
    }

protected:
    std::vector<RadialMenuSegment<T>> segments;
};

// Usage
class Engine_ModeSelector : public UI_RadialMenu<INPUT_STATE_FLAGS> {
    // ...
};

auto menu = std::make_unique<Engine_ModeSelector>(
    [this](int idx, INPUT_STATE_FLAGS mode) {  // Type-safe!
        SetInputMode(mode);  // No cast needed
    }
);

// Drawing
void DrawSegmentIconTyped(..., const INPUT_STATE_FLAGS& mode, ...) const override {
    switch (mode) {  // No cast needed!
        case IG_INPUT_MODE_SELECTING: DrawText("Select", ...); break;
        // ...
    }
}
```

### Characteristics

| Aspect | Rating | Notes |
|--------|--------|-------|
| **Complexity** | ⭐⭐ | Templates, header-only |
| **Binary Size** | ⭐⭐ | Larger (~9KB for 3 menus) |
| **Type Safety** | ⭐⭐⭐⭐⭐ | Compile-time checking |
| **Maintainability** | ⭐⭐⭐⭐ | Self-documenting |
| **Flexibility** | ⭐⭐⭐ | Works with any type |

### Pros ✅
- Full compile-time type safety
- No casting needed
- Self-documenting (T shows type)
- Compiler catches wrong types
- Works with complex types (not just enums)

### Cons ❌
- Template complexity
- Larger binary (code duplicated per type)
- All code must be in headers
- Compilation issues (template instantiation)
- More complex to understand

### Use When
- Large projects (many developers)
- Type safety critical
- Complex data types (not just enums)
- Binary size not critical

---

## Approach 4: Hybrid - Template + Non-Templated Base

### Implementation

```cpp
// Non-templated base (compiled once in .cpp)
class UI_RadialMenuBase : public UI_Selector {
public:
    virtual RadialMenuSegmentColors GetSegmentColors(int idx) const = 0;
    virtual int GetSegmentCount() const = 0;

protected:
    void DrawRadialMenu(...) const;  // Heavy code, compiled once!
};

// Templated derived (lightweight, in header)
template<typename T>
class UI_RadialMenu : public UI_RadialMenuBase {
public:
    using ClickHandler = std::function<void(int segmentIndex, const T& data)>;
    
    virtual RadialMenuSegmentColors GetSegmentColors(int idx) const override {
        return {segments[idx].edgeColor, segments[idx].backgroundColor, segments[idx].hoverColor};
    }
    
    virtual int GetSegmentCount() const override {
        return segments.size();
    }

protected:
    std::vector<RadialMenuSegment<T>> segments;
};

// Usage (same as Approach 3)
auto menu = std::make_unique<Engine_ModeSelector>(
    [this](int idx, INPUT_STATE_FLAGS mode) {
        SetInputMode(mode);
    }
);
```

### Characteristics

| Aspect | Rating | Notes |
|--------|--------|-------|
| **Complexity** | ⭐ | Most complex |
| **Binary Size** | ⭐⭐⭐⭐ | Optimized (~3.5KB for 3 menus) |
| **Type Safety** | ⭐⭐⭐⭐⭐ | Compile-time checking |
| **Maintainability** | ⭐⭐⭐ | More files, more layers |
| **Flexibility** | ⭐⭐⭐⭐ | Works with any type |

### Pros ✅
- Type-safe like Approach 3
- Smaller binary than Approach 3 (60% reduction)
- Heavy code compiled once
- No code duplication

### Cons ❌
- Most complex architecture
- Multiple class layers
- Virtual call overhead (negligible)
- More files to maintain
- Still requires templates

### Use When
- Need type safety AND small binary
- Many menu types (>10)
- Large project with strict size constraints
- Performance not critical (virtual calls acceptable)

---

### Implementation Summary

```cpp
// Data structure
struct RadialMenuSegment {
    Color edgeColor, backgroundColor, hoverColor;
    int data;  // Enum as int
};

// Base class
class UI_RadialMenu : public UI_Selector {
public:
    using ClickHandler = std::function<void(int segmentIndex, int data)>;
    virtual void DrawSegmentIcon(..., int data, ...) const;
};

// Concrete class
class Engine_ModeSelector : public UI_RadialMenu {
    // Constructor initializes segments with enum values
    // DrawSegmentIcon casts int to enum
};

// Usage
auto menu = std::make_unique<Engine_ModeSelector>(
    [this](int idx, int data) {
        SetInputMode(static_cast<INPUT_STATE_FLAGS>(data));
    }
);
```

---

## Migration Path

### Current State
- Using index-only approach
- Manual switch statements in callbacks

### Step 1: Add int data to segments
- Update `RadialMenuSegment` struct
- Add `data` parameter to click handler
- Pass data to `DrawSegmentIcon`

### Step 2: Update concrete classes
- Store enum values in segment data
- Cast in callbacks and drawing
- Remove manual switch on index

### Step 3: Cleanup
- Remove index-based logic
- Simplify callbacks
- Update documentation

**Estimated effort**: 2-3 hours

---

## Future Considerations

### If Project Grows
- More than 10 menu types → consider Hybrid approach
- More developers → consider Template approach
- Complex data types → consider Template approach

### If Binary Size Becomes Critical
- Profile actual binary impact
- Consider Hybrid approach if templates needed
- Optimize other areas first (likely bigger wins)

### If Type Safety Issues Arise
- Add helper functions to reduce casting
- Add runtime assertions
- Consider migrating to Template approach

---

## Conclusion

**Selected**: **KISS (int data) approach** ✅

**Rationale**: 
- Best balance of simplicity, binary size, and functionality
- Appropriate for embedded system with few menus
- Manual casting is acceptable trade-off for simplicity
- Can migrate to Template/Hybrid later if needed

**Key Principle**: 
> "Make it work, make it right, make it fast" - Kent Beck

For this project, KISS approach makes it work AND right. Template approach would be premature optimization of type safety at the cost of complexity.

---

## References

### Implementation Files
- `src/components/RadialMenuTypes.h` - Segment structure
- `src/components/UI_RadialMenu.h` - Base menu class
- `src/components/UI_RadialMenu.cpp` - Implementation
- `src/Engine_ModeSelector.h` - Concrete mode selector
- `src/Engine_ModeSelector.cpp` - Mode selector implementation

### Related Documents
- `template_issues_solutions.md` - Template bloat analysis
- `virtual_vs_copy_comparison.md` - Memory optimization
- `kiss_no_template_approach.md` - KISS approach details
- `radial_menu_template_migration_guide.md` - Template approach guide

### Design Principles
- KISS (Keep It Simple, Stupid)
- YAGNI (You Aren't Gonna Need It)
- Premature optimization is the root of all evil

