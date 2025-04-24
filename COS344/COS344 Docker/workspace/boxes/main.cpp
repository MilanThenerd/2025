#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <thread>
#include <random>
#include <chrono>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "shader.hpp"
#include "shapes.h"

#define timeDT std::chrono::_V2::steady_clock::time_point

using namespace glm;
using namespace std;

const char *getError()
{
    const char *errorDescription;
    glfwGetError(&errorDescription);
    return errorDescription;
}

inline void startUpGLFW()
{
    glewExperimental = true; // Needed for core profile
    if (!glfwInit())
    {
        throw getError();
    }
}

inline void startUpGLEW()
{
    glewExperimental = true; // Needed in core profile
    if (glewInit() != GLEW_OK)
    {
        glfwTerminate();
        throw getError();
    }
}

inline GLFWwindow *setUp()
{
    startUpGLFW();
    glfwWindowHint(GLFW_SAMPLES, 4);               // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL
    GLFWwindow *window;                                            // (In the accompanying source code, this variable is global for simplicity)
    window = glfwCreateWindow(1000, 1000, "Experiment", NULL, NULL);
    if (window == NULL)
    {
        cout << getError() << endl;
        glfwTerminate();
        throw "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n";
    }
    glfwMakeContextCurrent(window); // Initialize GLEW
    startUpGLEW();
    return window;
}

int main()
{
    // This is the normal setup function calls.
    GLFWwindow *window;
    try
    {
        window = setUp();
    }
    catch (const char *e)
    {
        cout << e << endl;
        throw;
    }

    // Here we set the background color to a shade of gray.
    glClearColor(0.2, 0.2, 0.2, 0.2);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_NEAREST);

    // Here we create a VAO
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // This is needed for sticky keys
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Here we compile and load the shaders. First we pass the vertex shader then the fragment shader.
    GLuint programID = LoadShaders("vertexShader.glsl", "fragmentShader.glsl");

    timeDT lastChanged = chrono::steady_clock::now();

    // Here we create two VBOs
    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    GLuint colorbuffer;
    glGenBuffers(1, &colorbuffer);

    double lastTime;
    lastTime = glfwGetTime();

    // Here we create a the boxes object which consists of two boxes
    vec3 centers[2] = {
        vec3(0, 0, 0),
        vec3(-0.1, -0.1, -0.1)};
    double heights[2] = {
        0.2,
        0.2,
    };
    double widths[2] = {
        0.2,
        0.2,
    };
    double lengths[2] = {
        0.2,
        0.2,
    };
    vec3 colors[2] = {
        vec3(0, 0, 1),
        vec3(1, 0, 0)};

    Shape *shp = new Boxes(2, centers, heights, widths, lengths, colors);

    do
    {
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;

        // Here we clear the color and depth buffer bits.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(programID);

        // Here we obtain the vertices and colors for the house as two dynamic arrays.
        GLfloat *vertices = shp->toVertexArray();
        GLfloat *colors = shp->toColorArray();
        
        //  Here we bind the VBOs
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat[shp->numVertices()]), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat[shp->numColors()]), colors, GL_STATIC_DRAW);

        // Here we enable the VAO and populate it.
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
            0,        // location 0 in the vertex shader.
            3,        // size
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0,        // stride
            (void *)0 // array buffer offset
        );

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glVertexAttribPointer(
            1,        // location 1 in the vertex shader.
            3,        // size
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0,        // stride
            (void *)0 // array buffer offset
        );

        glDrawArrays(GL_TRIANGLES, 0, shp->numPoints()); // Starting from vertex 0; 3 vertices total -> 1 triangle

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        // Here we swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

        // Reminder: The examples use GLM but for the practicals you may not use GLM and all the matrix calculations needs to be done in the application not the shaders.
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        {
            mat4x4 rotationX = mat4x4(0.0f);

            rotationX[0].x = 1;
            rotationX[1].y = cos(0.02);
            rotationX[1].z = -sin(0.02);
            rotationX[2].y = sin(0.02);
            rotationX[2].z = cos(0.02);
            rotationX[3].w = 1;

            mat4x4 rotationY = mat4x4(0.0f);

            rotationY[0].x = cos(0.04);
            rotationY[0].z = -sin(0.04);
            rotationY[1].y = 1;
            rotationY[2].x = sin(0.04);
            rotationY[2].z = cos(0.04);
            rotationY[3].w = 1;

            mat4x4 rot = rotationX * rotationY;

            shp->applyMatrix(transpose(rot));
        }
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        {
            mat4x4 rotationX = mat4x4(0.0f);

            rotationX[0].x = 1;
            rotationX[1].y = cos(-0.02);
            rotationX[1].z = -sin(-0.02);
            rotationX[2].y = sin(-0.02);
            rotationX[2].z = cos(-0.02);
            rotationX[3].w = 1;

            mat4x4 rotationY = mat4x4(0.0f);

            rotationY[0].x = cos(0.04);
            rotationY[0].z = -sin(0.04);
            rotationY[1].y = 1;
            rotationY[2].x = sin(0.04);
            rotationY[2].z = cos(0.04);
            rotationY[3].w = 1;

            mat4x4 rot = rotationX * rotationY;

            shp->applyMatrix(transpose(rot));
        }

        // delete[] vertices;
        // delete[] colors;

        lastTime = currentTime;
        cout << "FPS: " << 1 / deltaTime << endl;

    } while (glfwGetKey(window, GLFW_KEY_SPACE) != GLFW_PRESS &&
             glfwWindowShouldClose(window) == 0);

    delete shp;
}
