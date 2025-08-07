# OpenGL Texture Test Container

A minimal dev container specifically designed to test OpenGL texture rendering capabilities. This extends the basic OpenGL container with texture-specific testing tools.

## Quick Start

### 1. Host Setup
```bash
# Allow Docker containers to connect to X11
xhost +local:docker

# Verify basic setup
echo $DISPLAY        # Should show :0 or similar
glxinfo | head -5    # Should work without errors
```

### 2. Open in VS Code
1. Open this folder in VS Code
2. Click "Reopen in Container" when prompted
3. Wait for container to build and compile tests

### 3. Test Textures
```bash
# Run comprehensive test suite
./tests/test_script.sh

# Test texture rendering (visual)
cd tests && make test-textures
```

## What's Included

**Container Contents:**
- Ubuntu 22.04 with OpenGL support
- Texture libraries: GLEW, FreeGLUT, DevIL, SOIL
- Minimal development tools (gcc, make)
- Custom texture test program

**Test Programs:**
- `texture_test.c` - Visual checkerboard texture test
- `test_script.sh` - Comprehensive testing suite
- `Makefile` - Build and test commands

## Texture Test Program

The main test creates a procedural checkerboard texture and displays it:

```bash
cd tests
make test-textures
```

**Expected Result:**
- Window opens showing black and white checkerboard pattern
- If you see the pattern clearly, texture rendering works!
- Press ESC or Q to close

**If textures don't work:**
- You might see a solid color or blank window
- Check the console output for error messages
- Run `./test_script.sh` for diagnostic information

## Available Commands

| Command | Purpose |
|---------|---------|
| `make test-textures` | Visual texture rendering test |
| `make test-opengl` | Basic OpenGL functionality test |
| `make build` | Build texture test program |
| `make info` | Show system information |
| `./test_script.sh` | Run comprehensive test suite |

## Common Texture Issues

### Issue: Solid color instead of checkerboard
**Possible causes:**
- Missing GLEW initialization
- Insufficient OpenGL version
- Texture creation failed

**Check:**
```bash
glxinfo | grep -i "opengl version"
# Should be 2.0 or higher for basic textures
```

### Issue: Window opens but is blank/black
**Possible causes:**
- Graphics driver issues
- Missing texture libraries
- OpenGL context problems

**Check:**
```bash
glxinfo | grep "direct rendering"
# Should show "Yes" for hardware acceleration
```

### Issue: Program crashes or won't start
**Possible causes:**
- Missing development libraries
- X11 connection issues
- Compilation problems

**Check:**
```bash
ldd tests/texture_test
# Should show all required libraries found
```

## Troubleshooting

### 1. Rebuild if needed
```bash
cd tests
make clean
make build
```

### 2. Check system info
```bash
./test_script.sh
# Shows comprehensive system information
```

### 3. Test basic OpenGL first
```bash
glxgears
# Should work before testing textures
```

### 4. Check container logs
Look for error messages during container startup or program execution.

## Success Indicators

**Texture rendering is working when:**
- ✅ `glxinfo` shows hardware acceleration
- ✅ `glxgears` displays animated gears
- ✅ Texture test shows clear checkerboard pattern
- ✅ No error messages in console output

**Texture rendering has issues when:**
- ❌ Texture test shows solid color or blank window
- ❌ Console shows texture-related error messages
- ❌ Program crashes when loading textures

## Next Steps

Once texture rendering is confirmed working:
1. Add your own texture files to the `textures/` directory
2. Modify the test program to load custom textures
3. Add SDL2, SFML, or other graphics libraries as needed
4. Build your graphics applications with confidence

This container provides a verified base for texture-enabled OpenGL development.

