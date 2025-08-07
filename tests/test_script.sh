#!/bin/bash
# Comprehensive OpenGL and Texture Testing Script

echo "=== OpenGL Texture Testing Suite ==="
echo

# Function to check if command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Test 1: Basic OpenGL Information
echo "1. Checking OpenGL Information..."
echo "----------------------------------------"
if command_exists glxinfo; then
    echo "OpenGL Version:"
    glxinfo | grep -i "opengl version" || echo "Could not get OpenGL version"
    echo
    echo "Direct Rendering:"
    glxinfo | grep "direct rendering" || echo "Could not check direct rendering"
    echo
    echo "OpenGL Renderer:"
    glxinfo | grep -i "opengl renderer" || echo "Could not get renderer info"
    echo
else
    echo "ERROR: glxinfo not found!"
    exit 1
fi

# Test 2: Check for texture-related extensions
echo "2. Checking Texture Extensions..."
echo "----------------------------------------"
glxinfo | grep -i texture | head -5 || echo "No texture extensions found"
echo

# Test 3: Build texture test program
echo "3. Building Texture Test Program..."
echo "----------------------------------------"
if make build; then
    echo "✓ Texture test program built successfully"
else
    echo "✗ Failed to build texture test program"
    echo "This might indicate missing development libraries"
    exit 1
fi
echo

# Test 4: Check for required libraries
echo "4. Checking Required Libraries..."
echo "----------------------------------------"
echo "Checking for OpenGL libraries:"
ldconfig -p | grep -i gl | head -5 || echo "OpenGL libraries not found in ldconfig"
echo
echo "Checking for GLEW:"
ldconfig -p | grep -i glew || echo "GLEW library not found"
echo
echo "Checking for FreeGLUT:"
ldconfig -p | grep -i glut || echo "GLUT library not found"
echo

# Test 5: Quick visual test
echo "5. Quick Visual Tests..."
echo "----------------------------------------"
echo "Testing glxgears (should open window for 5 seconds):"
if command_exists glxgears; then
    timeout 5s glxgears >/dev/null 2>&1 && echo "✓ glxgears test passed" || echo "✗ glxgears test failed"
else
    echo "✗ glxgears not available"
fi
echo

# Test 6: Texture test instructions
echo "6. Texture Test Instructions..."
echo "----------------------------------------"
echo "To test texture rendering:"
echo "  make test-textures"
echo
echo "This will:"
echo "- Open a window with a black and white checkerboard pattern"
echo "- If you see the checkerboard, texture rendering is working!"
echo "- Press ESC or Q to close the window"
echo
echo "If the texture test fails, common issues are:"
echo "- Missing graphics drivers"
echo "- Insufficient OpenGL version"
echo "- Missing texture libraries"
echo

# Test 7: System summary
echo "7. System Summary..."
echo "----------------------------------------"
echo "DISPLAY: $DISPLAY"
echo "X11 Socket: $(ls /tmp/.X11-unix/ 2>/dev/null | wc -l) socket(s) found"
echo "GPU Devices: $(ls /dev/dri/ 2>/dev/null | wc -l) device(s) found"
echo "Container User: $(whoami)"
echo

echo "=== Test Suite Complete ==="
echo
echo "Next steps:"
echo "1. Run 'make test-textures' to test texture rendering"
echo "2. If textures don't work, check the troubleshooting guide"
echo "3. Use 'make help' to see all available commands"

