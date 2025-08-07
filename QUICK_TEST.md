# Quick Texture Test

## 1. Start Container
- Open folder in VS Code
- Click "Reopen in Container"
- Wait for build to complete

## 2. Run Texture Test
```bash
cd tests
make test-textures
```

## 3. What You Should See
- Window opens with black and white checkerboard pattern
- Pattern should be clear and well-defined
- Press ESC or Q to close

## 4. Results

### ✅ SUCCESS: Textures Working
- Clear checkerboard pattern visible
- No error messages in console
- Window responds normally

### ❌ PROBLEM: Textures Not Working
- Solid color or blank window
- Error messages about textures
- Program crashes

## 5. If Problems Occur
```bash
# Run diagnostic script
./test_script.sh

# Check basic OpenGL
glxgears

# Check system info
make info
```

## 6. Quick Fixes
- Ensure `xhost +local:docker` was run on host
- Rebuild container if needed
- Check that glxgears works first

