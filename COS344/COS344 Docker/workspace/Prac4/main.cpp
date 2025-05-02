#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
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
  glfwMakeContextCurrent(window);
  startUpGLEW();
  return window;
}

namespace Math
{
  glm::mat4 translate(const glm::mat4 &mat, const glm::vec3 &v)
  {
    glm::mat4 result = mat;
    result[3] = mat[0] * v[0] + mat[1] * v[1] + mat[2] * v[2] + mat[3];
    return result;
  }

  glm::mat4 scale(const glm::mat4 &mat, const glm::vec3 &v)
  {
    glm::mat4 result = mat;

    result[0][0] *= v.x;
    result[1][1] *= v.y;
    result[2][2] *= v.z;

    return result;
  }

  glm::mat4 rotate(const glm::mat4 &mat, float angle, glm::vec3 axis)
  {
    axis = glm::normalize(axis);

    float c = cos(angle);
    float s = sin(angle);
    float t = 1.0f - c;

    glm::mat4 rotation(1.0f);

    rotation[0][0] = c + axis.x * axis.x * t;
    rotation[0][1] = axis.x * axis.y * t - axis.z * s;
    rotation[0][2] = axis.x * axis.z * t + axis.y * s;

    rotation[1][0] = axis.y * axis.x * t + axis.z * s;
    rotation[1][1] = c + axis.y * axis.y * t;
    rotation[1][2] = axis.y * axis.z * t - axis.x * s;

    rotation[2][0] = axis.z * axis.x * t - axis.y * s;
    rotation[2][1] = axis.z * axis.y * t + axis.x * s;
    rotation[2][2] = c + axis.z * axis.z * t;

    return mat * rotation;
  }

  glm::mat4 lookAt(glm::vec3 eye, glm::vec3 center, glm::vec3 up)
  {
    glm::vec3 f = glm::normalize(center - eye);
    glm::vec3 s = glm::normalize(glm::cross(f, up));
    glm::vec3 u = glm::cross(s, f);

    glm::mat4 result(1.0f);

    result[0][0] = s.x;
    result[1][0] = s.y;
    result[2][0] = s.z;

    result[0][1] = u.x;
    result[1][1] = u.y;
    result[2][1] = u.z;

    result[0][2] = -f.x;
    result[1][2] = -f.y;
    result[2][2] = -f.z;

    result[3][0] = -glm::dot(s, eye);
    result[3][1] = -glm::dot(u, eye);
    result[3][2] = glm::dot(f, eye);

    return result;
  }

  glm::mat4 perspective(float fovy, float aspect, float near, float far)
  {
    float tanHalfFovy = tan(fovy / 2.0f);

    glm::mat4 result(0.0f);

    result[0][0] = 1.0f / (aspect * tanHalfFovy);
    result[1][1] = 1.0f / (tanHalfFovy);
    result[2][2] = -far / (far - near);
    result[2][3] = -1.0f;
    result[3][2] = -(2 * far * near) / (far - near);

    return result;
  }
}

bool wireframeMode = false;

class Polyhedrons {
  private:
      std::vector<GLfloat> vertices;
      std::vector<GLuint> indices;
      std::vector<GLfloat> colors;  // Now stores RGBA colors
      std::vector<GLfloat> transformedVertices;
      
      GLuint VAO = 0, VBO = 0, CBO = 0, EBO = 0;
      float position[3] = {0.0f, 0.0f, 0.0f};
      float scale[3] = {1.0f, 1.0f, 1.0f};
      float rotation[3] = {0.0f, 0.0f, 0.0f};
      bool wireframeMode = false;
  
      void calculateCenter(float& centerX, float& centerY, float& centerZ) {
          centerX = 0.0f;
          centerY = 0.0f;
          centerZ = 0.0f;
  
          for (size_t i = 0; i < vertices.size(); i += 3) {
              centerX += vertices[i];
              centerY += vertices[i+1];
              centerZ += vertices[i+2];
          }
  
          centerX /= (vertices.size() / 3);
          centerY /= (vertices.size() / 3);
          centerZ /= (vertices.size() / 3);
      }
  
      void applyTransform() {
          float centerX, centerY, centerZ;
          calculateCenter(centerX, centerY, centerZ);
  
          for (size_t i = 0; i < vertices.size(); i += 3) {
              float localX = vertices[i] - centerX;
              float localY = vertices[i+1] - centerY;
              float localZ = vertices[i+2] - centerZ;
  
              localX *= scale[0];
              localY *= scale[1];
              localZ *= scale[2];
  
              const float degToRad = static_cast<float>(M_PI / 180.0);
  
              float radians = rotation[2] * degToRad;
              float cosTheta = cosf(radians);
              float sinTheta = sinf(radians);
              float rotatedX = localX * cosTheta - localY * sinTheta;
              float rotatedY = localX * sinTheta + localY * cosTheta;
              float rotatedZ = localZ;
  
              radians = rotation[1] * degToRad;
              cosTheta = cosf(radians);
              sinTheta = sinf(radians);
              float tempX = rotatedX * cosTheta + rotatedZ * sinTheta;
              rotatedZ = -rotatedX * sinTheta + rotatedZ * cosTheta;
              rotatedX = tempX;
  
              radians = rotation[0] * degToRad;
              cosTheta = cosf(radians);
              sinTheta = sinf(radians);
              float tempY = rotatedY * cosTheta - rotatedZ * sinTheta;
              rotatedZ = rotatedY * sinTheta + rotatedZ * cosTheta;
              rotatedY = tempY;
  
              transformedVertices[i] = rotatedX + centerX + position[0];
              transformedVertices[i+1] = rotatedY + centerY + position[1];
              transformedVertices[i+2] = rotatedZ + centerZ + position[2];
          }
  
          if (VBO != 0) {
              glBindBuffer(GL_ARRAY_BUFFER, VBO);
              glBufferSubData(GL_ARRAY_BUFFER, 0, 
                  transformedVertices.size() * sizeof(GLfloat), 
                  transformedVertices.data());
          }
      }
  
  public:
      Polyhedrons(const std::vector<GLfloat>& v, 
                  const std::vector<GLuint>& i, 
                  const std::vector<GLfloat>& c)
          : vertices(v), indices(i), colors(c) {
          
          if (vertices.empty() || indices.empty()) {
              throw std::runtime_error("Vertices and indices must not be empty");
          }
  
          // Ensure we have 4 components (RGBA) for each vertex
          if (colors.size() != vertices.size() / 3 * 4) {
              // If we got 3 components, add alpha (default to 1.0)
              if (colors.size() == vertices.size() / 3 * 3) {
                  std::vector<GLfloat> newColors;
                  for (size_t i = 0; i < colors.size(); i += 3) {
                      newColors.push_back(colors[i]);
                      newColors.push_back(colors[i+1]);
                      newColors.push_back(colors[i+2]);
                      newColors.push_back(1.0f); // Alpha
                  }
                  colors = newColors;
              } else {
                  throw std::runtime_error("Colors must have either 3 or 4 components per vertex");
              }
          }
  
          transformedVertices.resize(vertices.size());
          std::copy(vertices.begin(), vertices.end(), transformedVertices.begin());
      }
  
      ~Polyhedrons() {
          glDeleteVertexArrays(1, &VAO);
          glDeleteBuffers(1, &VBO);
          glDeleteBuffers(1, &CBO);
          glDeleteBuffers(1, &EBO);
      }
  
      void upload() {
          glGenVertexArrays(1, &VAO);
          glGenBuffers(1, &VBO);
          glGenBuffers(1, &CBO);
          glGenBuffers(1, &EBO);
  
          glBindVertexArray(VAO);
  
          // Vertex positions
          glBindBuffer(GL_ARRAY_BUFFER, VBO);
          glBufferData(GL_ARRAY_BUFFER, 
                      transformedVertices.size() * sizeof(GLfloat),
                      transformedVertices.data(), 
                      GL_DYNAMIC_DRAW);
          glEnableVertexAttribArray(0);
          glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
  
          // Vertex colors (now with 4 components)
          glBindBuffer(GL_ARRAY_BUFFER, CBO);
          glBufferData(GL_ARRAY_BUFFER, 
                      colors.size() * sizeof(GLfloat),
                      colors.data(), 
                      GL_STATIC_DRAW);
          glEnableVertexAttribArray(1);
          glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, nullptr);  // Changed to 4 components
  
          // Element buffer
          glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
          glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                      indices.size() * sizeof(GLuint),
                      indices.data(),
                      GL_STATIC_DRAW);
  
          glBindVertexArray(0);
      }
  
      void draw() {
          if (VAO == 0) {
              throw std::runtime_error("Polyhedron not uploaded to GPU. Call upload() first.");
          }
  
          glBindVertexArray(VAO);
          GLenum mode = wireframeMode ? GL_LINES : GL_TRIANGLES;
          glDrawElements(mode, indices.size(), GL_UNSIGNED_INT, 0);
          glBindVertexArray(0);
      }
  
      glm::mat4 getTransformationMatrix() const {
          glm::mat4 model = glm::mat4(1.0f);
          model = Math::translate(model, glm::vec3(position[0], position[1], position[2]));
  
          model = Math::rotate(model, glm::radians(rotation[0]), glm::vec3(1.0f, 0.0f, 0.0f));
          model = Math::rotate(model, glm::radians(rotation[1]), glm::vec3(0.0f, 1.0f, 0.0f));
          model = Math::rotate(model, glm::radians(rotation[2]), glm::vec3(0.0f, 0.0f, 1.0f));
          model = Math::scale(model, glm::vec3(scale[0], scale[1], scale[2]));
          return model;
      }
  
      void applyLocalRotation(float angleDegrees, const glm::vec3 &axis) {
          glm::mat4 rotation = Math::rotate(glm::mat4(1.0f), glm::radians(angleDegrees), axis);
          for (size_t i = 0; i < vertices.size(); i += 3) {
              glm::vec4 vertex(vertices[i], vertices[i+1], vertices[i+2], 1.0f);
              glm::vec4 transformed = rotation * vertex;
              transformedVertices[i] = transformed.x;
              transformedVertices[i+1] = transformed.y;
              transformedVertices[i+2] = transformed.z;
          }
          if (VBO != 0) {
              glBindBuffer(GL_ARRAY_BUFFER, VBO);
              glBufferSubData(GL_ARRAY_BUFFER, 0, transformedVertices.size() * sizeof(GLfloat), transformedVertices.data());
          }
  
          applyTransform();
      }
  
      void setPosition(float x, float y, float z) {
          position[0] = x;
          position[1] = y;
          position[2] = z;
          applyTransform();
      }
  
      void setScale(float x, float y, float z) {
          scale[0] = x;
          scale[1] = y;
          scale[2] = z;
          applyTransform();
      }
  
      void setRotation(float x, float y, float z) {
          rotation[0] = x;
          rotation[1] = y;
          rotation[2] = z;
          applyTransform();
      }
  
      void move(float x, float y, float z) {
          position[0] += x;
          position[1] += y;
          position[2] += z;
          applyTransform();
      }
  
      void rotate(float x, float y, float z) {
          rotation[0] += x;
          rotation[1] += y;
          rotation[2] += z;
          applyTransform();
      }
  
      void changeScale(float x, float y, float z) {
          scale[0] *= x;
          scale[1] *= y;
          scale[2] *= z;
          applyTransform();
      }
  
      void getPosition(float &x, float &y, float &z) const {
          x = position[0];
          y = position[1];
          z = position[2];
      }
  
      void getScale(float &x, float &y, float &z) const {
          x = scale[0];
          y = scale[1];
          z = scale[2];
      }
  
      void getRotation(float &x, float &y, float &z) const {
          x = rotation[0];
          y = rotation[1];
          z = rotation[2];
      }
  
      void setTransformation(const glm::mat4 &transform) {
          for (size_t i = 0; i < vertices.size(); i += 3) {
              glm::vec4 vertex(vertices[i], vertices[i+1], vertices[i+2], 1.0f);
              glm::vec4 transformed = transform * vertex;
  
              transformedVertices[i] = transformed.x;
              transformedVertices[i+1] = transformed.y;
              transformedVertices[i+2] = transformed.z;
          }
  
          if (VBO != 0) {
              glBindBuffer(GL_ARRAY_BUFFER, VBO);
              glBufferSubData(GL_ARRAY_BUFFER, 0, transformedVertices.size() * sizeof(GLfloat), transformedVertices.data());
          }
      }
  
      void printVertices() {
          for (size_t i = 0; i < vertices.size(); i += 3) {
              std::cout << vertices[i] << " " << vertices[i+1] << " " << vertices[i+2] << "\n";
          }
      }
  
      void setColor(const std::vector<GLfloat>& newColors) {
          if (newColors.size() == colors.size()) {
              colors = newColors;
              if (CBO != 0) {
                  glBindBuffer(GL_ARRAY_BUFFER, CBO);
                  glBufferSubData(GL_ARRAY_BUFFER, 0, colors.size() * sizeof(GLfloat), colors.data());
              }
          } else {
              throw std::runtime_error("New colors must have same size as existing colors");
          }
      }
  };
  
class Scene {
  private:
      Polyhedrons *bottomSheet;
      Polyhedrons *glass;
      Polyhedrons *backsheet;
      Polyhedrons *light;
  
      int numRectangles = 10;
      float glassAlpha = 0.5f;

      int backSheetSize;
      int glassSize;
      int lightSize;
  
      std::vector<glm::vec4> backsheetColors = {
          glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), // Red
          glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), // Green
          glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), // Blue
          glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), // White
          glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), // Black
          glm::vec4(1.0f, 0.5f, 0.0f, 1.0f), // Orange
          glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), // Purple
          glm::vec4(0.0f, 1.0f, 1.0f, 1.0f), // Cyan
          glm::vec4(0.5f, 0.5f, 0.0f, 1.0f), // Olive
          glm::vec4(0.5f, 0.0f, 0.5f, 1.0f)  // Dark purple
      };
      int currentBacksheetColor = 0;
  
      std::vector<glm::vec4> glassColors = {
          glm::vec4(1.0f, 0.0f, 0.0f, glassAlpha), // Red
          glm::vec4(0.0f, 1.0f, 0.0f, glassAlpha), // Green
          glm::vec4(0.0f, 0.0f, 1.0f, glassAlpha), // Blue
          glm::vec4(1.0f, 1.0f, 1.0f, glassAlpha), // White
          glm::vec4(0.0f, 0.0f, 0.0f, glassAlpha), // Black
          glm::vec4(0.5f, 1.0f, 0.5f, glassAlpha), // Light green
          glm::vec4(0.5f, 0.5f, 1.0f, glassAlpha), // Light blue
          glm::vec4(1.0f, 0.5f, 0.5f, glassAlpha), // Light red
          glm::vec4(0.8f, 0.8f, 0.2f, glassAlpha), // Yellowish
          glm::vec4(0.2f, 0.8f, 0.8f, glassAlpha)  // Teal
      };
      int currentGlassColor = 1; // Start with green
  
      std::vector<glm::vec3> lightColors = {
          glm::vec3(1.0f, 0.0f, 0.0f), // Red
          glm::vec3(0.0f, 1.0f, 0.0f), // Green
          glm::vec3(0.0f, 0.0f, 1.0f), // Blue
          glm::vec3(1.0f, 1.0f, 1.0f), // White
          glm::vec3(1.0f, 1.0f, 0.0f), // Yellow
          glm::vec3(1.0f, 0.5f, 0.0f), // Orange
          glm::vec3(0.5f, 0.0f, 1.0f), // Purple
          glm::vec3(0.0f, 1.0f, 1.0f), // Cyan
          glm::vec3(1.0f, 0.0f, 1.0f), // Magenta
          glm::vec3(0.5f, 1.0f, 0.5f)  // Light green
      };
      int currentLightColor = 2;
  
      glm::vec3 position = glm::vec3(0.0f);
      glm::vec3 forward = glm::vec3(0.0f, 0.0f, -1.0f);
      glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
      glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);
  
      glm::vec3 bottomSheetOffset = glm::vec3(0.0f, 0.0f, 0.0f);
      glm::vec3 glassOffset = glm::vec3(0.0f, 0.0f, -0.05f);
      glm::vec3 backsheetOffset = glm::vec3(0.0f, 0.0f, 0.05f);
      glm::vec3 lightOffset = glm::vec3(0.0f, 0.0f, -5.0f);
  
      std::vector<GLfloat> getColor(float r, float g, float b, float a, int n) {
          std::vector<GLfloat> colorData;
          for (size_t i = 0; i < (n / 3); i++) {
              colorData.push_back(r);
              colorData.push_back(g);
              colorData.push_back(b);
              colorData.push_back(a);
          }
          return colorData;
      }
  
      void createBacksheet(GLfloat* vertices, GLuint* indices, int numRectangles, int &vertexOffset, int &indexOffset) {
          GLfloat width = 10.0f;
          GLfloat height = 10.0f;
          GLfloat depth = 0.1f;
      
          GLfloat rectWidth = width / numRectangles;
          GLfloat rectHeight = height / numRectangles;
      
          for (int i = 0; i < numRectangles; i++) {
              for (int j = 0; j < numRectangles; j++) {
                  GLfloat x0 = -width / 2 + i * rectWidth;
                  GLfloat x1 = x0 + rectWidth;
                  GLfloat y0 = -height / 2 + j * rectHeight;
                  GLfloat y1 = y0 + rectHeight;
                  GLfloat z = 0.0f;
      
                  // Add 4 vertices
                  vertices[vertexOffset++] = x0; vertices[vertexOffset++] = y0; vertices[vertexOffset++] = z;
                  vertices[vertexOffset++] = x1; vertices[vertexOffset++] = y0; vertices[vertexOffset++] = z;
                  vertices[vertexOffset++] = x1; vertices[vertexOffset++] = y1; vertices[vertexOffset++] = z;
                  vertices[vertexOffset++] = x0; vertices[vertexOffset++] = y1; vertices[vertexOffset++] = z;
      
                  GLuint baseIndex = (vertexOffset / 3) - 4;
      
                  indices[indexOffset++] = baseIndex;
                  indices[indexOffset++] = baseIndex + 1;
                  indices[indexOffset++] = baseIndex + 2;
                  indices[indexOffset++] = baseIndex;
                  indices[indexOffset++] = baseIndex + 2;
                  indices[indexOffset++] = baseIndex + 3;
              }
          }
      }
      
      void createGlass(GLfloat* vertices, GLuint* indices, int &vertexOffset, int &indexOffset) {
          GLfloat width = 10.0f;
          GLfloat height = 10.0f;
          GLfloat z = -5.0f;
      
          GLuint baseIndex = vertexOffset / 3;
      
          vertices[vertexOffset++] = -width / 2; vertices[vertexOffset++] = -height / 2; vertices[vertexOffset++] = z;
          vertices[vertexOffset++] =  width / 2; vertices[vertexOffset++] = -height / 2; vertices[vertexOffset++] = z;
          vertices[vertexOffset++] =  width / 2; vertices[vertexOffset++] =  height / 2; vertices[vertexOffset++] = z;
          vertices[vertexOffset++] = -width / 2; vertices[vertexOffset++] =  height / 2; vertices[vertexOffset++] = z;
      
          indices[indexOffset++] = baseIndex;
          indices[indexOffset++] = baseIndex + 1;
          indices[indexOffset++] = baseIndex + 2;
          indices[indexOffset++] = baseIndex;
          indices[indexOffset++] = baseIndex + 2;
          indices[indexOffset++] = baseIndex + 3;
      }
      
      void createBottomsheet(GLfloat* vertices, GLuint* indices, int &vertexOffset, int &indexOffset) {
          GLfloat width = 10.0f;
          GLfloat length = 10.0f;
          GLfloat y = -5.0f;
      
          GLuint baseIndex = vertexOffset / 3;
      
          vertices[vertexOffset++] = -width / 2; vertices[vertexOffset++] = y; vertices[vertexOffset++] = -length;
          vertices[vertexOffset++] =  width / 2; vertices[vertexOffset++] = y; vertices[vertexOffset++] = -length;
          vertices[vertexOffset++] =  width / 2; vertices[vertexOffset++] = y; vertices[vertexOffset++] =  0.0f;
          vertices[vertexOffset++] = -width / 2; vertices[vertexOffset++] = y; vertices[vertexOffset++] =  0.0f;
      
          indices[indexOffset++] = baseIndex;
          indices[indexOffset++] = baseIndex + 1;
          indices[indexOffset++] = baseIndex + 2;
          indices[indexOffset++] = baseIndex;
          indices[indexOffset++] = baseIndex + 2;
          indices[indexOffset++] = baseIndex + 3;
      }
      
      void createLight(GLfloat* vertices, GLuint* indices, int &vertexOffset, int &indexOffset) {
          GLfloat size = 0.5f;
          GLfloat z = -10.0f;
      
          GLuint baseIndex = vertexOffset / 3;
      
          vertices[vertexOffset++] = -size / 2; vertices[vertexOffset++] = -size / 2; vertices[vertexOffset++] = z;
          vertices[vertexOffset++] =  size / 2; vertices[vertexOffset++] = -size / 2; vertices[vertexOffset++] = z;
          vertices[vertexOffset++] =  size / 2; vertices[vertexOffset++] =  size / 2; vertices[vertexOffset++] = z;
          vertices[vertexOffset++] = -size / 2; vertices[vertexOffset++] =  size / 2; vertices[vertexOffset++] = z;
      
          indices[indexOffset++] = baseIndex;
          indices[indexOffset++] = baseIndex + 1;
          indices[indexOffset++] = baseIndex + 2;
          indices[indexOffset++] = baseIndex;
          indices[indexOffset++] = baseIndex + 2;
          indices[indexOffset++] = baseIndex + 3;
      }
  
      void normalize(glm::vec3 &vec) {
          float len = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
          if (len > 0.0f) {
              vec /= len;
          }
      }
  
      void rebuildOrientation() {
          right = glm::cross(up, forward);
          normalize(right);
          up = glm::cross(forward, right);
          normalize(up);
          normalize(forward);
      }
  
      void rotateLocal(const glm::vec3 &axis, float degrees) {
          float radians = glm::radians(degrees);
          float cosA = cos(radians);
          float sinA = sin(radians);
  
          glm::vec3 newForward = forward * cosA + glm::cross(up, axis) * sinA;
          glm::vec3 newUp = up * cosA + glm::cross(forward, axis) * sinA;
  
          forward = newForward;
          up = newUp;
          rebuildOrientation();
      }
  
      glm::mat4 buildTransformMatrix() {
          rebuildOrientation();
          return glm::mat4(
              glm::vec4(right, 0.0f),
              glm::vec4(up, 0.0f),
              glm::vec4(forward, 0.0f),
              glm::vec4(position, 1.0f));
      }
  
      void updateTransforms() {
          glm::mat4 transform = buildTransformMatrix();
  
          glm::mat4 bottomSheetTransform = transform;
          bottomSheetTransform = Math::translate(bottomSheetTransform, bottomSheetOffset);
          bottomSheetTransform = Math::rotate(bottomSheetTransform, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
          bottomSheet->setTransformation(bottomSheetTransform);
  
          glm::mat4 glassTransform = Math::translate(transform, glassOffset);
          glass->setTransformation(glassTransform);
  
          glm::mat4 bsheetTransform = Math::translate(transform, backsheetOffset);
          backsheet->setTransformation(bsheetTransform);
  
          glm::mat4 lightTransform = Math::translate(transform, lightOffset);
          light->setTransformation(lightTransform);
      }
  
  public:
      Scene(int numRects = 10) : numRectangles(numRects) {
          // Create bottom sheet
          std::vector<GLfloat> bottomSheetVertices;
          std::vector<GLuint> bottomSheetIndices;
          int vOffset = 0, iOffset = 0;
          bottomSheetVertices.resize(6 * 4);
          bottomSheetIndices.resize(6);
          createBottomsheet(bottomSheetVertices.data(), bottomSheetIndices.data(), vOffset, iOffset);
          bottomSheet = new Polyhedrons(bottomSheetVertices, bottomSheetIndices, getColor(0.2f, 0.2f, 0.2f, 1.0f, bottomSheetVertices.size()));

  
          // Create glass
          vOffset = iOffset = 0;
          std::vector<GLfloat> glassVertices(6 * 4);
          std::vector<GLuint> glassIndices(6);
          createGlass(glassVertices.data(), glassIndices.data(), vOffset, iOffset);
          glass = new Polyhedrons(glassVertices, glassIndices, getColor(glassColors[currentGlassColor].r, 
                                glassColors[currentGlassColor].g, glassColors[currentGlassColor].b, glassAlpha, glassVertices.size()));

          glassSize = glassVertices.size();
  
          // Create light
          vOffset = iOffset = 0;
          std::vector<GLfloat> lightVertices(6 * 4);
          std::vector<GLuint> lightIndices(6);
          createLight(lightVertices.data(), lightIndices.data(), vOffset, iOffset);
          light = new Polyhedrons(lightVertices, lightIndices, getColor(lightColors[currentLightColor].r, 
                                lightColors[currentLightColor].g, lightColors[currentLightColor].b, 1.0f, lightVertices.size()));

          lightSize = lightVertices.size();
  
          // Create backsheet
          vOffset = iOffset = 0;
          std::vector<GLfloat> backsheetVertices(6 * 4 * numRectangles * numRectangles);
          std::vector<GLuint> backsheetIndices(6 * numRectangles * numRectangles);
          createBacksheet(backsheetVertices.data(), backsheetIndices.data(), numRectangles, vOffset, iOffset);
          backsheet = new Polyhedrons(backsheetVertices, backsheetIndices, 
                                    getColor(backsheetColors[currentBacksheetColor].r, 
                                    backsheetColors[currentBacksheetColor].g, 
                                    backsheetColors[currentBacksheetColor].b, 
                                    1.0f, backsheetVertices.size()));

          backSheetSize = backsheetVertices.size();
  
          updateTransforms();
      }
  
      void upload() {
          bottomSheet->upload();
          glass->upload();
          light->upload();
          backsheet->upload();
      }
  
      void draw(GLuint ProgramID) 
      {
        
        glUniform3f(glGetUniformLocation(ProgramID, "lightColor"), 1.0f, 1.0f, 1.0f);
        float lx, ly, lz;
        light->getPosition(lx, ly, lz);
        
        float gx, gy, gz;
        glass->getPosition(gx, gy, gz);
        
        // Direction = Target - Origin (glass - light)
        float dx = gx - lx;
        float dy = gy - ly;
        float dz = gz - lz;

        float length = sqrt(dx * dx + dy * dy + dz * dz);
        dx /= length;
        dy /= length;
        dz /= length;


        glUniform3f(glGetUniformLocation(ProgramID, "viewPos"), 0, 20, 20);
        glUniform3f(glGetUniformLocation(ProgramID, "lightDir"), dx, dy, dz);
        glUniform3f(glGetUniformLocation(ProgramID, "lightPos"), lx, ly, lz);
        glUniform1f(glGetUniformLocation(ProgramID, "cutOff"), cos(glm::radians(12.5f)));
        glUniform1f(glGetUniformLocation(ProgramID, "outerCutOff"), cos(glm::radians(17.5f)));
        
        glUniform1i(glGetUniformLocation(ProgramID, "isLight"), GL_TRUE);
        light->draw();
        glUniform1i(glGetUniformLocation(ProgramID, "isLight"), GL_FALSE);

        glUniform1i(glGetUniformLocation(ProgramID, "isBottomSheet"), GL_TRUE);
        bottomSheet->draw();
        glUniform1i(glGetUniformLocation(ProgramID, "isBottomSheet"), GL_FALSE);
        
        glUniform1i(glGetUniformLocation(ProgramID, "isGlass"), GL_TRUE);
        glass->draw();
        glUniform1i(glGetUniformLocation(ProgramID, "isGlass"), GL_FALSE);

        glUniform1i(glGetUniformLocation(ProgramID, "isBackSheet"), GL_TRUE);
        backsheet->draw();
        glUniform1i(glGetUniformLocation(ProgramID, "isBackSheet"), GL_FALSE);
 
      }
  
      void pitch(float degrees) {
          float radians = glm::radians(degrees);
          glm::mat4 rotation = Math::rotate(glm::mat4(1.0f), radians, right);
  
          forward = glm::vec3(rotation * glm::vec4(forward, 0.0f));
          up = glm::vec3(rotation * glm::vec4(up, 0.0f));
  
          updateTransforms();
      }
  
      void yaw(float degrees) {
          rotateLocal(forward, degrees);
          updateTransforms();
      }
  
      void roll(float degrees) {
          rotateLocal(up, degrees);
          updateTransforms();
      }
  
      void resetOrientation() {
          forward = glm::vec3(0.0f, 0.0f, -1.0f);
          up = glm::vec3(0.0f, 1.0f, 0.0f);
          right = glm::vec3(1.0f, 0.0f, 0.0f);
          updateTransforms();
      }
  
      void cycleBacksheetColor() {
          currentBacksheetColor = (currentBacksheetColor + 1) % backsheetColors.size();
          backsheet->setColor(getColor(backsheetColors[currentBacksheetColor].r, 
                                     backsheetColors[currentBacksheetColor].g, 
                                     backsheetColors[currentBacksheetColor].b, 
                                     backsheetColors[currentBacksheetColor].a, backSheetSize));
      }
  
      void cycleGlassColor() {
          currentGlassColor = (currentGlassColor + 1) % glassColors.size();
          glass->setColor(getColor(glassColors[currentGlassColor].r, 
                         glassColors[currentGlassColor].g, 
                         glassColors[currentGlassColor].b, 
                         glassColors[currentGlassColor].a, glassSize));
      }
  
      void cycleLightColor() {
          currentLightColor = (currentLightColor + 1) % lightColors.size();
          light->setColor(getColor(lightColors[currentLightColor].r, 
                                 lightColors[currentLightColor].g, 
                                 lightColors[currentLightColor].b, 
                                 1.0f, lightSize));
      }

      glm::vec3 getLightColor()
      {
        return lightColors[currentLightColor];
      }
  };
void handleKeyPresses(GLFWwindow *window, Scene* scene)
{
  static auto lastSpacePressTime = std::chrono::steady_clock::now();

  const float rotationAmount = 2.0f;
  const float flapAmount = 10.0f;

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
  {
    scene->pitch(-rotationAmount);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
  {
    scene->pitch(rotationAmount);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
  {
    scene->yaw(rotationAmount);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
  {
    scene->yaw(-rotationAmount);
  }
  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
  {
      scene->roll(-rotationAmount);
  }
  if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
  {    
  scene->roll(rotationAmount);
  }
  if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
  {
    scene->cycleGlassColor();
  }
  if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
  {
    scene->cycleLightColor();
  }
  if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
  {
    auto now = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastSpacePressTime);

    if (duration.count() < 100)
      return;
    wireframeMode = !wireframeMode;
    lastSpacePressTime = now;
  }
}

int main()
{
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
  glfwSwapInterval(1);
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  GLuint programID = LoadShaders("vertex.glsl", "fragment.glsl");

  GLuint MatrixID = glGetUniformLocation(programID, "MVP");

  GLuint LightColorID = glGetUniformLocation(programID, "lightColor");

  glm::mat4 Projection = Math::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);

  glm::mat4 View = Math::lookAt(
      glm::vec3(0, 20, 20),
      glm::vec3(0, 0, 0),
      glm::vec3(0, 1, 0));

  glm::mat4 MVP = Projection * View;

  Scene* scene = new Scene();
  scene->upload();



  double lastTime;
  lastTime = glfwGetTime();
  do
  { 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(programID);
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

    scene->draw(programID);

    glfwSwapBuffers(window);
    glfwPollEvents();
    handleKeyPresses(window, scene);

  } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && !glfwWindowShouldClose(window));

  glDeleteProgram(programID);
  glfwTerminate();
  return 0;
}