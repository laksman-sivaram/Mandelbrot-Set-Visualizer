#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>

// Window dimensions
int WIDTH = 800;
int HEIGHT = 600;

// Mandelbrot parameters
float zoom = 1.0f;
float shadingIntensity = 1.0f;
float offsetX = 0.0f;
float offsetY = 0.0f;

// Function prototypes
void initOpenGL();
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void handleKeys(GLFWwindow *window, int key, int scancode, int action, int mods);
void getColor(float t, float *r, float *g, float *b);
void drawMandelbrot();

int main()
{
    // GLFW initialization
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        exit(EXIT_FAILURE);
    }

    // Create GLFW window
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Mandelbrot Set", NULL, NULL);
    if (!window)
    {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Set GLFW context and callbacks
    glfwMakeContextCurrent(window);
    initOpenGL();
    glfwSetKeyCallback(window, handleKeys);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Center the Mandelbrot set at the start
    offsetX = -0.5f;
    offsetY = 0.0f;

    // Main rendering loop
    while (!glfwWindowShouldClose(window))
    {
        drawMandelbrot();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup and exit
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

// Function to initialize OpenGL
void initOpenGL()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WIDTH, HEIGHT, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

// Function to handle window resizing
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    WIDTH = width;
    HEIGHT = height;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WIDTH, HEIGHT, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
}

// Function to handle keyboard input
void handleKeys(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        switch (key)
        {
        case GLFW_KEY_LEFT:
            shadingIntensity -= 0.1f;
            break;
        case GLFW_KEY_RIGHT:
            shadingIntensity += 0.1f;
            break;
        case GLFW_KEY_W:
            offsetY -= 0.1f / zoom;
            break;
        case GLFW_KEY_S:
            offsetY += 0.1f / zoom;
            break;
        case GLFW_KEY_A:
            offsetX -= 0.1f / zoom;
            break;
        case GLFW_KEY_D:
            offsetX += 0.1f / zoom;
            break;
        case GLFW_KEY_UP:
            zoom *= 1.1f;
            break;
        case GLFW_KEY_DOWN:
            zoom /= 1.1f;
            break;
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GLFW_TRUE);
            break;
        }
    }
}

// Function to convert iteration count to a color using a visually appealing color scheme
void getColor(float t, float *r, float *g, float *b)
{
    *r = 0.5f + 0.5f * cos(3.0f + t);
    *g = 0.5f + 0.5f * sin(3.0f + t);
    *b = 0.5f + 0.5f * cos(3.0f + t);
}

// Function to draw the Mandelbrot set
void drawMandelbrot()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_POINTS);

    for (int y = 0; y < HEIGHT; ++y)
    {
        for (int x = 0; x < WIDTH; ++x)
        {
            float zx, zy, cx, cy;
            zx = zy = 0.0f;

            // Adjust the Mandelbrot set coordinates based on the zoom level
            cx = (x - WIDTH / 2.0f) / (WIDTH / 4.0f * zoom) + offsetX;
            cy = (y - HEIGHT / 2.0f) / (HEIGHT / 4.0f * zoom) + offsetY;

            int iteration = 0;
            while (iteration < 100 && zx * zx + zy * zy < 4.0f)
            {
                float xtemp = zx * zx - zy * zy + cx;
                zy = 2.0f * zx * zy + cy;
                zx = xtemp;
                iteration++;
            }

            float intensity = (float)iteration / 100.0f;

            // Apply shading intensity only to the Mandelbrot set region
            if (iteration < 100)
            {
                intensity = pow(intensity, shadingIntensity);
            }

            // Convert iteration count to a color using a different coloring scheme
            float r, g, b;
            getColor(intensity, &r, &g, &b);

            glColor3f(r, g, b);
            glVertex2f(x, y);
        }
    }

    glEnd();
}
