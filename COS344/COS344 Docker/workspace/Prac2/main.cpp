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

inline GLFWwindow *setUp(int width = 1000, int height = 1000, float red = 0.0f, float green = 0.0f , float blue = 0.0f , float alpha = 1.0f)
{
    startUpGLFW();
    glfwWindowHint(GLFW_SAMPLES, 4);               // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL
    GLFWwindow *window;                                            // (In the accompanying source code, this variable is global for simplicity)
    window = glfwCreateWindow(width, height, "Experiment", NULL, NULL);
    if (window == NULL)
    {
        cout << getError() << endl;
        glfwTerminate();
        throw "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n";
    }
    glfwMakeContextCurrent(window); // Initialize GLEW
    glClearColor(red, green, blue, alpha);
    startUpGLEW();
    return window;
}

std::vector<GLfloat> getColor(float red, float green, float blue, int vertices)
{
  std::vector<GLfloat> result(vertices * 3);
  for (int i = 0; i < vertices; i++) {
    result[3*i] = red;
    result[3*i+1] = green;
    result[3*i+2] = blue;
  }
  return result;
}

struct Triangle 
{
  std::vector<GLfloat> vertices;
  std::vector<GLfloat> colors;
  GLfloat x, y;
  Triangle(std::vector<GLfloat> v, std::vector<GLfloat> c) 
      : vertices(v), colors(c) {}

  void move(GLfloat dx, GLfloat dy) 
  {
    x += dx;
    y += dy;
        for (size_t i = 0; i < vertices.size(); i += 3) 
        {
            vertices[i] += dx;
            vertices[i+1] += dy;
        }
  }

  void setColor(const std::vector<GLfloat> color) 
  {
    colors = color;
  }
};

struct TriangleGL {
  GLuint VAO, VBO, CBO;
};

std::vector<TriangleGL> uploadTriangles(std::vector<Triangle>& triangles) {
  std::vector<TriangleGL> triangleGLs;
  
  for (auto& tri : triangles) {
      TriangleGL tgl;

      glGenVertexArrays(1, &tgl.VAO);
      glGenBuffers(1, &tgl.VBO);
      glGenBuffers(1, &tgl.CBO);

      glBindVertexArray(tgl.VAO);

      glBindBuffer(GL_ARRAY_BUFFER, tgl.VBO);
      glBufferData(GL_ARRAY_BUFFER, tri.vertices.size() * sizeof(GLfloat), tri.vertices.data(), GL_STATIC_DRAW);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

      glBindBuffer(GL_ARRAY_BUFFER, tgl.CBO);
      glBufferData(GL_ARRAY_BUFFER, tri.colors.size() * sizeof(GLfloat), tri.colors.data(), GL_STATIC_DRAW);
      glEnableVertexAttribArray(1);
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

      glBindVertexArray(0);

      triangleGLs.push_back(tgl);
  }

  return triangleGLs;
}

struct Square
{
  std::vector<GLfloat> vertices;
  std::vector<GLfloat> colors;
  GLfloat x, y;
  Square(std::vector<GLfloat> v, std::vector<GLfloat> c) 
      : vertices(v), colors(c) {}

  void move(GLfloat dx, GLfloat dy) 
  {
    x += dx;
    y += dy;
        for (size_t i = 0; i < vertices.size(); i += 4) 
        {
            vertices[i] += dx;
            vertices[i+1] += dy;
        }
  }

  void setColor(const std::vector<GLfloat> color) 
  {
    colors = color;
  }
};

struct SquareGL {
  GLuint VAO, VBO, CBO;
};

std::vector<SquareGL> uploadSquares(std::vector<Square>& squares) {
  std::vector<SquareGL> squareGLs;
  
  for (auto& sqr : squares) {
      SquareGL sgl;

      glGenVertexArrays(1, &sgl.VAO);
      glGenBuffers(1, &sgl.VBO);
      glGenBuffers(1, &sgl.CBO);

      glBindVertexArray(sgl.VAO);

      glBindBuffer(GL_ARRAY_BUFFER, sgl.VBO);
      glBufferData(GL_ARRAY_BUFFER, sqr.vertices.size() * sizeof(GLfloat), sqr.vertices.data(), GL_STATIC_DRAW);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

      glBindBuffer(GL_ARRAY_BUFFER, sgl.CBO);
      glBufferData(GL_ARRAY_BUFFER, sqr.colors.size() * sizeof(GLfloat), sqr.colors.data(), GL_STATIC_DRAW);
      glEnableVertexAttribArray(1);
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

      glBindVertexArray(0);

      squareGLs.push_back(sgl);
  }

  return squareGLs;
}


int main() {
  GLFWwindow *window = setUp(1000, 1000 , 1.0f , 0.4f , 0.0f , 1.0f);
  GLuint programID = LoadShaders("vertex.glsl", "fragment.glsl");


  GLuint VertexArrayID;
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);
  
  std::vector<Triangle> triangles = {
      Triangle(
        { -0.05f, -0.05f, 0.0f,   0.05f, -0.05f, 0.0f,   0.0f,  0.05f, 0.0f },
        getColor(0.0f, 0.6f, 0.2f, 3)
      ),
      Triangle(
        { -0.05f, -0.05f, 0.0f,   0.05f, -0.05f, 0.0f,   0.0f,  0.05f, 0.0f },
        getColor(0.0f, 0.6f, 0.2f, 3)
      ),
      Triangle(
        { -0.05f, -0.05f, 0.0f,   0.05f, -0.05f, 0.0f,   0.0f,  0.05f, 0.0f },
        getColor(0.0f, 0.6f, 0.2f, 3)
      ),
      Triangle(
        { -0.05f, -0.05f, 0.0f,   0.05f, -0.05f, 0.0f,   0.0f,  0.05f, 0.0f }, 
        getColor(0.0f, 0.6f, 0.2f, 3)
     )
  };

  triangles[0].move(-0.5f, -0.5f);
  triangles[1].move(0.5f, 0.0f);
  triangles[2].move(0.0f, 0.5f);
  triangles[3].move(-0.5f, 0.5f);


  std::vector<Square> squares = {
    Square(
      { -0.05f, -0.05f, 0.0f,   0.05f, -0.05f, 0.0f,  -0.05f,  0.05f, 0.0f,   0.05f,  0.05f, 0.0f },
      getColor(0.0f, 0.0f, 0.0f, 4)
    )
  };

  int type = 0;
  int index = 0;

  double lastKeyPressTime = 0.0;
  const double keyPressCooldown = 0.5;

  do 
  {
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(programID);

    //TRIANGLES
    if(type == 1)
    {
      for(int i = 0 ; i < triangles.size() ; i++)
      {
        if( i == index)
        {
          triangles[i].colors = getColor(0.4f, 1.0f , 0.6f , 3);
        }
        else
        {
          triangles[i].colors = getColor(0.0f, 0.6f, 0.2f, 3);
        }
      }
    }
    std::vector<TriangleGL> triangleGLs = uploadTriangles(triangles);
    for (auto& tgl : triangleGLs) 
    {
      glBindVertexArray(tgl.VAO);
      glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    std::vector<SquareGL> squareGLs = uploadSquares(squares);
    for (auto& sgl : squareGLs)
    {
      glBindVertexArray(sgl.VAO);
      glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();

    double currentTime = glfwGetTime();
    if (currentTime - lastKeyPressTime >= keyPressCooldown) {
        if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) {
            type = 0;
            index = 0;
            lastKeyPressTime = currentTime;
        } else if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
            if (type != 1) {
                type = 1;
                index = 0;
            } else {
                if (index < triangles.size()) {
                    index++;
                }
                if (index >= triangles.size()) {
                    index = 0;
                }
            }
            lastKeyPressTime = currentTime;
        } else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
            type = 2;
            lastKeyPressTime = currentTime;
        } else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
            type = 3;
            lastKeyPressTime = currentTime;
        } else if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
            type = 4;
            lastKeyPressTime = currentTime;
        }
    }
    std::cout << "Type: " << type << std::endl;
  }
  while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && !glfwWindowShouldClose(window));
  
  glfwTerminate();
  
  return 0;
}
