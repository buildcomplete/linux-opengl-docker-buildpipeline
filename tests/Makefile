# Makefile for OpenGL Texture Tests

CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LIBS = -lGL -lGLU -lglut -lGLEW -lm

# Executables
TEXTURE_TEST = texture_test

.PHONY: all build test-textures test-opengl clean help info

# Build all tests
all: build

build: $(TEXTURE_TEST)

$(TEXTURE_TEST): texture_test.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

# Test texture rendering (visual test)
test-textures: $(TEXTURE_TEST)
	@echo "=== Testing Texture Rendering ==="
	@echo "This will open a window with a checkerboard pattern"
	@echo "If you see black and white squares, textures are working!"
	@echo "Press ESC or Q to close the window"
	@echo ""
	./$(TEXTURE_TEST)

# Test basic OpenGL functionality
test-opengl:
	@echo "=== Testing Basic OpenGL ==="
	@echo "OpenGL Information:"
	@glxinfo | head -20
	@echo ""
	@echo "Direct Rendering:"
	@glxinfo | grep "direct rendering"
	@echo ""
	@echo "OpenGL Version:"
	@glxinfo | grep -i "opengl version"
	@echo ""
	@echo "Testing with glxgears (close window to continue):"
	@timeout 10s glxgears || echo "glxgears test completed"

# Show available commands
help:
	@echo "=== OpenGL Texture Test Commands ==="
	@echo "make build         - Build texture test program"
	@echo "make test-textures - Run visual texture test"
	@echo "make test-opengl   - Test basic OpenGL functionality"
	@echo "make info          - Show system information"
	@echo "make clean         - Remove built files"
	@echo "make help          - Show this help"

# Show system information
info:
	@echo "=== System Information ==="
	@echo "OpenGL Libraries:"
	@pkg-config --list-all | grep -i gl | head -10
	@echo ""
	@echo "Graphics Drivers:"
	@lspci | grep -i vga || echo "No VGA devices found"
	@echo ""
	@echo "Display Information:"
	@echo "DISPLAY = $$DISPLAY"
	@ls -la /tmp/.X11-unix/ 2>/dev/null || echo "No X11 sockets found"
	@echo ""
	@echo "GPU Devices:"
	@ls -la /dev/dri/ 2>/dev/null || echo "No GPU devices found"

# Clean up
clean:
	rm -f $(TEXTURE_TEST)

# Default target
.DEFAULT_GOAL := help

