#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>

// Simple texture test program
// Creates a procedural checkerboard texture and displays it

GLuint textureId;
int windowWidth = 800;
int windowHeight = 600;

// Create a simple checkerboard texture
void createCheckerboardTexture() {
    const int texWidth = 64;
    const int texHeight = 64;
    unsigned char* textureData = malloc(texWidth * texHeight * 3);
    
    // Generate checkerboard pattern
    for (int y = 0; y < texHeight; y++) {
        for (int x = 0; x < texWidth; x++) {
            int index = (y * texWidth + x) * 3;
            unsigned char color = ((x / 8) + (y / 8)) % 2 ? 255 : 0;
            textureData[index] = color;     // Red
            textureData[index + 1] = color; // Green  
            textureData[index + 2] = color; // Blue
        }
    }
    
    // Create OpenGL texture
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
    
    free(textureData);
    
    printf("Checkerboard texture created successfully!\n");
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Enable texturing
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureId);
    
    // Draw textured quad
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(-0.8f, -0.8f);
        glTexCoord2f(4.0f, 0.0f); glVertex2f( 0.8f, -0.8f);
        glTexCoord2f(4.0f, 4.0f); glVertex2f( 0.8f,  0.8f);
        glTexCoord2f(0.0f, 4.0f); glVertex2f(-0.8f,  0.8f);
    glEnd();
    
    glDisable(GL_TEXTURE_2D);
    
    glutSwapBuffers();
}

void reshape(int width, int height) {
    windowWidth = width;
    windowHeight = height;
    glViewport(0, 0, width, height);
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 27 || key == 'q' || key == 'Q') { // ESC or Q to quit
        printf("Texture test completed successfully!\n");
        exit(0);
    }
}

void printInfo() {
    printf("=== OpenGL Texture Test ===\n");
    printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
    printf("OpenGL Renderer: %s\n", glGetString(GL_RENDERER));
    printf("OpenGL Vendor: %s\n", glGetString(GL_VENDOR));
    
    // Check for texture support
    GLint maxTextureSize;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
    printf("Max Texture Size: %d x %d\n", maxTextureSize, maxTextureSize);
    
    printf("\nInstructions:\n");
    printf("- You should see a black and white checkerboard pattern\n");
    printf("- Press ESC or Q to quit\n");
    printf("- If you see the pattern, texture rendering is working!\n");
    printf("=============================\n\n");
}

int main(int argc, char** argv) {
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("OpenGL Texture Test");
    
    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        printf("ERROR: Failed to initialize GLEW\n");
        return 1;
    }
    
    // Print OpenGL information
    printInfo();
    
    // Set up OpenGL state
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    
    // Create test texture
    createCheckerboardTexture();
    
    // Set up callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    
    printf("Starting texture test... (Press ESC or Q to quit)\n");
    
    // Start main loop
    glutMainLoop();
    
    return 0;
}

