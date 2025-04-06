#include <stdio.h>
#include <stdlib.h>
#include <iostream>
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
    axis = glm::normalize(axis); // You need this for a proper rotation

    float c = cos(angle);
    float s = sin(angle);
    float t = 1.0f - c;

    glm::mat4 rotation(1.0f); // Identity matrix

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

class Polyhedrons
{
private:
  GLfloat *vertices;
  GLfloat *colors;
  GLuint VAO = 0, VBO = 0, CBO = 0;
  GLfloat *transformedVertices;
  float position[3] = {0.0f, 0.0f, 0.0f};
  float scale[3] = {1.0f, 1.0f, 1.0f};
  float rotation[3] = {0.0f, 0.0f, 0.0f};
  int vertexCount = 0;

  void calculateCenter(float &centerX, float &centerY, float &centerZ)
  {
    centerX = 0.0f;
    centerY = 0.0f;
    centerZ = 0.0f;

    for (int i = 0; i < vertexCount * 3; i += 3)
    {
      centerX += vertices[i];
      centerY += vertices[i + 1];
      centerZ += vertices[i + 2];
    }

    centerX /= vertexCount;
    centerY /= vertexCount;
    centerZ /= vertexCount;
  }

  void applyTransform()
  {
    float centerX, centerY, centerZ;
    calculateCenter(centerX, centerY, centerZ);

    for (int i = 0; i < vertexCount * 3; i += 3)
    {
      float localX = vertices[i] - centerX;
      float localY = vertices[i + 1] - centerY;
      float localZ = vertices[i + 2] - centerZ;

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
      transformedVertices[i + 1] = rotatedY + centerY + position[1];
      transformedVertices[i + 2] = rotatedZ + centerZ + position[2];
    }

    if (VBO != 0)
    {
      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferSubData(GL_ARRAY_BUFFER, 0, vertexCount * 3 * sizeof(GLfloat), transformedVertices);
    }
  }

public:
  Polyhedrons(GLfloat *v, GLfloat *c, int vc)
      : vertices(v), colors(c), vertexCount(vc)
  {
    if (vertexCount <= 0)
    {
      throw std::runtime_error("Vertex count must be positive");
    }

    transformedVertices = new GLfloat[vertexCount * 3];
    for (int i = 0; i < vertexCount * 3; i++)
    {
      transformedVertices[i] = vertices[i];
    }
  }

  ~Polyhedrons()
  {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &CBO);
    delete[] transformedVertices;
  }

  Polyhedrons(const Polyhedrons &) = delete;
  Polyhedrons &operator=(const Polyhedrons &) = delete;

  void draw()
  {

    if (VAO == 0)
    {
      throw std::runtime_error("Polyhedron not uploaded to GPU. Call upload() first.");
    }
    glBindVertexArray(VAO);
    if (wireframeMode)
    {
      glDrawArrays(GL_LINES, 0, vertexCount);
    }
    else
    {
      glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    }
  }

  void upload()
  {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &CBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(GLfloat), transformedVertices, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, CBO);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindVertexArray(0);
  }

  glm::mat4 getTransformationMatrix() const
  {
    glm::mat4 model = glm::mat4(1.0f);
    model = Math::translate(model, glm::vec3(position[0], position[1], position[2]));

    model = Math::rotate(model, glm::radians(rotation[0]), glm::vec3(1.0f, 0.0f, 0.0f));
    model = Math::rotate(model, glm::radians(rotation[1]), glm::vec3(0.0f, 1.0f, 0.0f));
    model = Math::rotate(model, glm::radians(rotation[2]), glm::vec3(0.0f, 0.0f, 1.0f));
    model = Math::scale(model, glm::vec3(scale[0], scale[1], scale[2]));
    return model;
  }

  void applyLocalRotation(float angleDegrees, const glm::vec3 &axis)
  {
    glm::mat4 rotation = Math::rotate(glm::mat4(1.0f), glm::radians(angleDegrees), axis);
    for (int i = 0; i < vertexCount * 3; i += 3)
    {
      glm::vec4 vertex(vertices[i], vertices[i + 1], vertices[i + 2], 1.0f);
      glm::vec4 transformed = rotation * vertex;
      transformedVertices[i] = transformed.x;
      transformedVertices[i + 1] = transformed.y;
      transformedVertices[i + 2] = transformed.z;
    }
    if (VBO != 0)
    {
      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferSubData(GL_ARRAY_BUFFER, 0, vertexCount * 3 * sizeof(GLfloat), transformedVertices);
    }

    applyTransform();
  }

  void setPosition(float x, float y, float z)
  {
    position[0] = x;
    position[1] = y;
    position[2] = z;
    applyTransform();
  }

  void setScale(float x, float y, float z)
  {
    scale[0] = x;
    scale[1] = y;
    scale[2] = z;
    applyTransform();
  }

  void setRotation(float x, float y, float z)
  {
    rotation[0] = x;
    rotation[1] = y;
    rotation[2] = z;
    applyTransform();
  }

  void move(float x, float y, float z)
  {
    position[0] += x;
    position[1] += y;
    position[2] += z;
    applyTransform();
  }

  void rotate(float x, float y, float z)
  {
    rotation[0] += x;
    rotation[1] += y;
    rotation[2] += z;
    applyTransform();
  }

  void changeScale(float x, float y, float z)
  {
    scale[0] *= x;
    scale[1] *= y;
    scale[2] *= z;
    applyTransform();
  }

  void getPosition(float &x, float &y, float &z) const
  {
    x = position[0];
    y = position[1];
    z = position[2];
  }

  void getScale(float &x, float &y, float &z) const
  {
    x = scale[0];
    y = scale[1];
    z = scale[2];
  }

  void getRotation(float &x, float &y, float &z) const
  {
    x = rotation[0];
    y = rotation[1];
    z = rotation[2];
  }

  void setTransformation(const glm::mat4 &transform)
  {
    for (int i = 0; i < vertexCount * 3; i += 3)
    {
      glm::vec4 vertex(vertices[i], vertices[i + 1], vertices[i + 2], 1.0f);
      glm::vec4 transformed = transform * vertex;

      transformedVertices[i] = transformed.x;
      transformedVertices[i + 1] = transformed.y;
      transformedVertices[i + 2] = transformed.z;
    }

    if (VBO != 0)
    {
      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferSubData(GL_ARRAY_BUFFER, 0, vertexCount * 3 * sizeof(GLfloat), transformedVertices);
    }
  }
};

GLfloat *getColor(float red, float green, float blue, int vertices)
{
  GLfloat *result = new GLfloat[vertices * 3];
  for (int i = 0; i < vertices * 3; i += 3)
  {
    result[i] = red;
    result[i + 1] = green;
    result[i + 2] = blue;
  }
  return result;
}

GLfloat *createTriangularPrism(float length = 2.0f, float height = 1.0f, float width = 1.0f, float offset = 0.0f)
{
  GLfloat *vertices = new GLfloat[72];

  float center = width / 2 + offset;
  vertices[0] = 0.0f;
  vertices[1] = 0.0f;
  vertices[2] = 0.0f;

  vertices[3] = width;
  vertices[4] = 0.0f;
  vertices[5] = 0.0f;

  vertices[6] = center;
  vertices[7] = 0.0f;
  vertices[8] = height;

  vertices[9] = 0.0f;
  vertices[10] = length;
  vertices[11] = 0.0f;

  vertices[12] = width;
  vertices[13] = length;
  vertices[14] = 0.0f;

  vertices[15] = center;
  vertices[16] = length;
  vertices[17] = height;

  vertices[18] = 0.0f;
  vertices[19] = 0.0f;
  vertices[20] = 0.0f;

  vertices[21] = center;
  vertices[22] = 0.0f;
  vertices[23] = height;

  vertices[24] = 0.0f;
  vertices[25] = length;
  vertices[26] = 0.0f;

  vertices[27] = center;
  vertices[28] = 0.0f;
  vertices[29] = height;

  vertices[30] = 0.0f;
  vertices[31] = length;
  vertices[32] = 0.0f;

  vertices[33] = center;
  vertices[34] = length;
  vertices[35] = height;

  vertices[36] = width;
  vertices[37] = 0.0f;
  vertices[38] = 0.0f;

  vertices[39] = center;
  vertices[40] = 0.0f;
  vertices[41] = height;

  vertices[42] = width;
  vertices[43] = length;
  vertices[44] = 0.0f;

  vertices[45] = center;
  vertices[46] = 0.0f;
  vertices[47] = height;

  vertices[48] = width;
  vertices[49] = length;
  vertices[50] = 0.0f;

  vertices[51] = center;
  vertices[52] = length;
  vertices[53] = height;

  vertices[54] = 0.0f;
  vertices[55] = 0.0f;
  vertices[56] = 0.0f;

  vertices[57] = width;
  vertices[58] = 0.0f;
  vertices[59] = 0.0f;

  vertices[60] = 0.0f;
  vertices[61] = length;
  vertices[62] = 0.0f;

  vertices[63] = width;
  vertices[64] = 0.0f;
  vertices[65] = 0.0f;

  vertices[66] = 0.0f;
  vertices[67] = length;
  vertices[68] = 0.0f;

  vertices[69] = width;
  vertices[70] = length;
  vertices[71] = 0.0f;

  return vertices;
}

GLfloat *createCuboid(float width = 1.0f, float height = 1.0f, float length = 1.0f, float offsetX = 0.0f, float offsetY = 0.0f, float offsetZ = 0.0f, float scaleX = 1.0f , float scaleY = 1.0f , float scaleZ = 1.0f)
{
  GLfloat *vertices = new GLfloat[108];

  //bottom panel
  vertices[0] = 0.0f;
  vertices[1] = 0.0f;
  vertices[2] = 0.0f;

  vertices[3] = length;
  vertices[4] = 0.0f;
  vertices[5] = 0.0f;

  vertices[6] = length;
  vertices[7] = height;
  vertices[8] = 0.0f;

  vertices[9] = 0.0f;
  vertices[10] = 0.0f;
  vertices[11] = 0.0f;

  vertices[12] = length;
  vertices[13] = height;
  vertices[14] = 0.0f;

  vertices[15] = 0.0f;
  vertices[16] = height;
  vertices[17] = 0.0f;

  //top panel
  vertices[18] = 0.0f+offsetX;
  vertices[19] = 0.0f+offsetY;
  vertices[20] = width+offsetZ;

  vertices[21] = length;
  vertices[22] = height;
  vertices[23] = width;

  vertices[24] = length;
  vertices[25] = 0.0f;
  vertices[26] = width;

  vertices[27] = 0.0f+offsetX;
  vertices[28] = 0.0f+offsetY;
  vertices[29] = width+offsetZ;

  vertices[30] = 0.0f+offsetX;
  vertices[31] = height+offsetY;
  vertices[32] = width+offsetZ;

  vertices[33] = length;
  vertices[34] = height;
  vertices[35] = width;

  //sidepanel

  vertices[36] = 0.0f;
  vertices[37] = 0.0f;
  vertices[38] = 0.0f;

  vertices[39] = 0.0f;
  vertices[40] = height;
  vertices[41] = 0.0f;

  vertices[42] = 0.0f+offsetX;
  vertices[43] = height+offsetY;
  vertices[44] = width+offsetZ;

  vertices[45] = 0.0f;
  vertices[46] = 0.0f;
  vertices[47] = 0.0f;

  vertices[48] = 0.0f+offsetX;
  vertices[49] = height+offsetY;
  vertices[50] = width+offsetZ;

  vertices[51] = 0.0f+offsetX;
  vertices[52] = 0.0f+offsetY;
  vertices[53] = width+offsetZ;

  // //sidepanel 3

  vertices[54] = length;
  vertices[55] = 0.0f;
  vertices[56] = 0.0f;

  vertices[57] = length;
  vertices[58] = 0.0f;
  vertices[59] = width;

  vertices[60] = length;
  vertices[61] = height;
  vertices[62] = width;

  vertices[63] = length;
  vertices[64] = 0.0f;
  vertices[65] = 0.0f;

  vertices[66] = length;
  vertices[67] = height;
  vertices[68] = width;

  vertices[69] = length;
  vertices[70] = height;
  vertices[71] = 0.0f;

  // //sidepanel 4

  vertices[72] = 0.0f;
  vertices[73] = 0.0f;
  vertices[74] = 0.0f;

  vertices[75] = length;
  vertices[76] = 0.0f;
  vertices[77] = width;

  vertices[78] = length;
  vertices[79] = 0.0f;
  vertices[80] = 0.0f;

  vertices[81] = 0.0f;
  vertices[82] = 0.0f;
  vertices[83] = 0.0f;

  vertices[84] = 0.0f+offsetX;
  vertices[85] = 0.0f+offsetY;
  vertices[86] = width+offsetZ;

  vertices[87] = length;
  vertices[88] = 0.0f;
  vertices[89] = width;


  // //top

  vertices[90] = 0.0f;
  vertices[91] = height;
  vertices[92] = 0.0f;

  vertices[93] = length;
  vertices[94] = height;
  vertices[95] = 0.0f;

  vertices[96] = length;
  vertices[97] = height;
  vertices[98] = width;

  vertices[99] = 0.0f;
  vertices[100] = height;
  vertices[101] = 0.0f;

  vertices[102] = length;
  vertices[103] = height;
  vertices[104] = width;

  vertices[105] = 0.0f+offsetX;
  vertices[106] = height+offsetY;
  vertices[107] = width+offsetZ;

  return vertices;
}
GLfloat *createCone(float radius = 0.5f, float height = 1.0f, int segments = 36)
{
  int totalVertices = segments * 6;
  GLfloat *vertices = new GLfloat[totalVertices * 3];

  float angleStep = 2.0f * M_PI / segments;
  int index = 0;

  for (int i = 0; i < segments; i++)
  {
    float angle1 = i * angleStep;
    float angle2 = (i + 1) * angleStep;

    vertices[index++] = 0.0f;
    vertices[index++] = 0.0f;
    vertices[index++] = 0.0f;

    vertices[index++] = radius * cosf(angle1);
    vertices[index++] = radius * sinf(angle1);
    vertices[index++] = 0.0f;

    vertices[index++] = radius * cosf(angle2);
    vertices[index++] = radius * sinf(angle2);
    vertices[index++] = 0.0f;

    vertices[index++] = radius * cosf(angle1);
    vertices[index++] = radius * sinf(angle1);
    vertices[index++] = 0.0f;

    vertices[index++] = radius * cosf(angle2);
    vertices[index++] = radius * sinf(angle2);
    vertices[index++] = 0.0f;

    vertices[index++] = 0.0f;
    vertices[index++] = 0.0f;
    vertices[index++] = height;
  }

  return vertices;
}

GLfloat *createCylinder(float radius = 0.5f, float height = 1.0f, float scale=1.0 , int segments = 36)
{
  int totalVertices = segments * 18;
  GLfloat *vertices = new GLfloat[totalVertices * 3];

  float angleStep = 2.0f * M_PI / segments;
  int index = 0;
  float halfHeight = height / 2.0f;

  for (int i = 0; i < segments; i++)
  {
    float angle1 = i * angleStep;
    float angle2 = (i + 1) * angleStep;

    vertices[index++] = 0.0f;
    vertices[index++] = -halfHeight;
    vertices[index++] = 0.0f;

    vertices[index++] = radius * cosf(angle1);
    vertices[index++] = -halfHeight;
    vertices[index++] = radius * sinf(angle1);

    vertices[index++] = radius * cosf(angle2);
    vertices[index++] = -halfHeight;
    vertices[index++] = radius * sinf(angle2);

    vertices[index++] = 0.0f;
    vertices[index++] = halfHeight;
    vertices[index++] = 0.0f;

    vertices[index++] = radius * cosf(angle2) * scale;
    vertices[index++] = halfHeight;
    vertices[index++] = radius * sinf(angle2) * scale;

    vertices[index++] = radius * cosf(angle1) * scale;
    vertices[index++] = halfHeight;
    vertices[index++] = radius * sinf(angle1) * scale;

    vertices[index++] = radius * cosf(angle1);
    vertices[index++] = -halfHeight;
    vertices[index++] = radius * sinf(angle1);

    vertices[index++] = radius * cosf(angle2);
    vertices[index++] = -halfHeight;
    vertices[index++] = radius * sinf(angle2);

    vertices[index++] = radius * cosf(angle1) * scale;
    vertices[index++] = halfHeight;
    vertices[index++] = radius * sinf(angle1) * scale;

    vertices[index++] = radius * cosf(angle2);
    vertices[index++] = -halfHeight;
    vertices[index++] = radius * sinf(angle2);

    vertices[index++] = radius * cosf(angle2) * scale;
    vertices[index++] = halfHeight;
    vertices[index++] = radius * sinf(angle2) * scale;

    vertices[index++] = radius * cosf(angle1) * scale;
    vertices[index++] = halfHeight;
    vertices[index++] = radius * sinf(angle1) * scale;
  }

  return vertices;
}

class Airplane
{
private:
  Polyhedrons *body;

  Polyhedrons *wingLeft;
  Polyhedrons *wingFlapLeft;
  Polyhedrons *wingRight;
  Polyhedrons *wingFlapRight;

  Polyhedrons *tailCone;
  Polyhedrons *noseBody;
  Polyhedrons *tailBody;

  Polyhedrons *propellor1;
  Polyhedrons *propellor2;
  Polyhedrons *propellor3;
  Polyhedrons *propellor4;


  glm::vec3 position = glm::vec3(0.0f);
  glm::vec3 forward = glm::vec3(0.0f, 0.0f, -1.0f);
  glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);

  glm::vec3 bodyOffset = glm::vec3(-0.5f, 0.0f, -1.0f);

  glm::vec3 wingLeftOffset = glm::vec3(-2.0f, 0.5f, 0.0f);
  glm::vec3 wingLeftFlapOffset = glm::vec3(2.0f, 0.5f, 0.0f);
  glm::vec3 wingRightOffset = glm::vec3(2.0f, 0.5f, 0.0f);
  glm::vec3 wingRightFlapOffset = glm::vec3(2.0f, 0.5f, 0.0f);

  glm::vec3 tailConeOffset = glm::vec3(-0.5f, 0.5f, -1.0f);
  glm::vec3 noseBodyOffset = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 tailBodyOffset = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 propellorOffset1 = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 propellorOffset2 = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 propellorOffset3 = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 propellorOffset4 = glm::vec3(0.0f, 0.0f, 0.0f);


  float flapAngle = 0.0f;
  float maxFlapAngle = 30.0f;

  float propellorSpeed = 0.0f;
  float propellorAngle = 0.0f;

  void normalize(glm::vec3 &vec)
  {
    float len = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
    if (len > 0.0f)
    {
      vec /= len;
    }
  }

  void rebuildOrientation()
  {
    right = glm::cross(up, forward);
    normalize(right);
    up = glm::cross(forward, right);
    normalize(up);
    normalize(forward);
  }

  void rotateLocal(const glm::vec3 &axis, float degrees)
  {
    float radians = glm::radians(degrees);
    float cosA = cos(radians);
    float sinA = sin(radians);

    glm::vec3 newForward = forward * cosA + glm::cross(up, axis) * sinA;
    glm::vec3 newUp = up * cosA + glm::cross(forward, axis) * sinA;

    forward = newForward;
    up = newUp;
    rebuildOrientation();
  }

  glm::mat4 buildTransformMatrix()
  {
    rebuildOrientation();
    return glm::mat4(
        glm::vec4(right, 0.0f),
        glm::vec4(up, 0.0f),
        glm::vec4(forward, 0.0f),
        glm::vec4(position, 1.0f));
  }

  void updateTransforms()
  {
    glm::mat4 transform = buildTransformMatrix();

    glm::mat4 bodyTransform = transform;
    bodyTransform = Math::translate(bodyTransform, bodyOffset);
    bodyTransform = Math::rotate(bodyTransform, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 temp_1 = buildTransformMatrix();
    glm::mat4 temp_2 = temp_1;
    temp_2 = Math::translate(temp_2, bodyOffset);
    temp_2 = Math::rotate(temp_2, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    body->setTransformation(bodyTransform);

    glm::mat4 wingLeftTransform = transform;
    wingLeftTransform = Math::translate(wingLeftTransform, wingLeftOffset);
    wingLeftTransform = Math::rotate(wingLeftTransform, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    wingLeftTransform = Math::rotate(wingLeftTransform, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    wingLeft->setTransformation(wingLeftTransform);

    glm::mat4 wingFlapLeftTransform = transform;
    wingFlapLeftTransform = Math::translate(wingFlapLeftTransform, wingLeftFlapOffset);
    wingFlapLeftTransform = Math::rotate(wingFlapLeftTransform, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    wingFlapLeftTransform = Math::rotate(wingFlapLeftTransform, glm::radians(-10.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    wingFlapLeftTransform = Math::rotate(wingFlapLeftTransform, glm::radians(-flapAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    wingFlapLeft->setTransformation(wingFlapLeftTransform);

    glm::mat4 wingRightTransform = transform;
    wingRightTransform = Math::translate(wingRightTransform, wingRightOffset);
    wingRightTransform = Math::rotate(wingRightTransform, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    wingRightTransform = Math::rotate(wingRightTransform, glm::radians(180.0f), glm::vec3(0.0f, .0f, 1.0f));
    wingRight->setTransformation(wingRightTransform);

    glm::mat4 wingFlapRightTransform = transform;
    wingFlapRightTransform = Math::translate(wingFlapRightTransform, wingRightFlapOffset);
    wingFlapRightTransform = Math::rotate(wingFlapRightTransform, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    wingFlapRightTransform = Math::rotate(wingFlapRightTransform, glm::radians(-10.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    wingFlapRightTransform = Math::rotate(wingFlapRightTransform, glm::radians(flapAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    wingFlapRight->setTransformation(wingFlapRightTransform);

    glm::mat4 tailConeTransform = transform;
    tailConeTransform = Math::translate(tailConeTransform, tailConeOffset);
    tailConeTransform = Math::rotate(tailConeTransform, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    tailCone->setTransformation(tailConeTransform);

    glm::mat4 tailBodyTransform = transform;
    tailBodyTransform = Math::translate(tailBodyTransform, tailBodyOffset);
    tailBodyTransform = Math::rotate(tailBodyTransform, glm::radians(90.0f), glm::vec3(1.0f, .0f, 0.0f));
    tailBody->setTransformation(tailBodyTransform);

    glm::mat4 noseBodyTransform = transform;
    noseBodyTransform = Math::translate(noseBodyTransform, noseBodyOffset);
    noseBodyTransform = Math::rotate(noseBodyTransform, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    noseBody->setTransformation(noseBodyTransform);

    glm::mat4 propellorTransform1 = transform;
    propellorTransform1 = Math::translate(propellorTransform1, propellorOffset1);
    propellorTransform1 = Math::rotate(propellorTransform1, glm::radians(-propellorAngle), glm::vec3(0.0f, 0.0f, 1.0f));
    propellor1->setTransformation(propellorTransform1);

    glm::mat4 propellorTransform2 = transform;
    propellorTransform2 = Math::translate(propellorTransform2, propellorOffset2);
    propellorTransform2 = Math::rotate(propellorTransform2, glm::radians(-propellorAngle + 90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    propellor2->setTransformation(propellorTransform2);

    glm::mat4 propellorTransform3 = transform;
    propellorTransform3 = Math::translate(propellorTransform3, propellorOffset3);
    propellorTransform3 = Math::rotate(propellorTransform3, glm::radians(-propellorAngle + 180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    propellor3->setTransformation(propellorTransform3);

    glm::mat4 propellorTransform4 = transform;
    propellorTransform4 = Math::translate(propellorTransform4, propellorOffset4);
    propellorTransform4 = Math::rotate(propellorTransform4, glm::radians(-propellorAngle + 270.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    propellor4->setTransformation(propellorTransform4);
  }

public:
  Airplane(float bodyWidth, float bodyLength, GLfloat *bodyColor,
           float wingSpan, float wingWidth, float wingHeight, GLfloat *wingColor)
  {
    //P-51D Mustang specifications

    body = new Polyhedrons(createCylinder(bodyWidth, bodyLength), bodyColor, 36 * 12);
    bodyOffset = glm::vec3(0, 0, 0);


    wingLeft = new Polyhedrons(createCuboid(wingSpan, wingHeight , wingWidth, 2.0f, 0.0f, 0.0f), wingColor, 36);
    wingLeftOffset = glm::vec3(0, -bodyWidth, -bodyLength*0.1);

    wingFlapLeft = new Polyhedrons(createTriangularPrism(wingSpan, -(wingWidth * 0.2), wingHeight), getColor(1.0, 0.9, 0.9, 24), 24);
    wingLeftFlapOffset = glm::vec3(0, (-bodyWidth + wingHeight), -(wingWidth * 0.2));


    wingRight = new Polyhedrons(createCuboid(-wingSpan, wingHeight , wingWidth, 2.0f, 0.0f, 0.0f), wingColor, 36);
    wingRightOffset = glm::vec3(0, -bodyWidth, -bodyLength*0.1);

    wingFlapRight = new Polyhedrons(createTriangularPrism(wingSpan, -(wingWidth * 0.2), wingHeight), getColor(1.0, 0.9, 0.9, 24), 24);
    wingRightFlapOffset = glm::vec3(0, (-bodyWidth), -(wingWidth * 0.2));

    tailCone = new Polyhedrons(createCone(bodyWidth, bodyLength * 0.6), getColor(1.0, 0.0, 0.0, 36 * 6), 36 * 6);
    noseBody = new Polyhedrons(createCone(bodyWidth, bodyLength * 0.3), getColor(1.0, 0.0, 0.0, 36 * 6), 36 * 6);

    tailConeOffset = glm::vec3(0, 0, -bodyLength / 2);
    noseBodyOffset = glm::vec3(0, 0, bodyLength / 2);

    tailBody = new Polyhedrons(createCylinder(bodyWidth, bodyLength*0.5, 0.6) , bodyColor , 36*12); 
    tailBodyOffset = glm::vec3(0, 0, -bodyLength*0.7);

    propellor1 = new Polyhedrons(createCuboid(0.5f, 2.0f, 0.1f), getColor(0.0, 0.0, 0.0, 36), 36);
    propellor2 = new Polyhedrons(createCuboid(0.5f, 2.0f, 0.1f), getColor(0.0, 0.0, 0.0, 36), 36);
    propellor3 = new Polyhedrons(createCuboid(0.5f, 2.0f, 0.1f), getColor(0.0, 0.0, 0.0, 36), 36);
    propellor4 = new Polyhedrons(createCuboid(0.5f, 2.0f, 0.1f), getColor(0.0, 0.0, 0.0, 36), 36);

    propellorOffset1 = glm::vec3(0, 0, (bodyLength / 2 + bodyLength * 0.2));
    propellorOffset2 = glm::vec3(0, 0, (bodyLength / 2 + bodyLength * 0.2));
    propellorOffset3 = glm::vec3(0, 0, (bodyLength / 2 + bodyLength * 0.2));
    propellorOffset4 = glm::vec3(0, 0, (bodyLength / 2 + bodyLength * 0.2));


    updateTransforms();
  }

  void upload()
  {
    body->upload();
    wingLeft->upload();
    wingFlapLeft->upload();
    wingFlapRight->upload();
    tailCone->upload();
    noseBody->upload();
    wingRight->upload();
    propellor1->upload();
    propellor2->upload();
    propellor3->upload();
    propellor4->upload();
    tailBody->upload();
  }

  void draw()
  {
    body->draw();
    wingLeft->draw();
    wingFlapLeft->draw();
    // tailCone->draw();
    noseBody->draw();
    wingRight->draw();
    wingFlapRight->draw();
    propellor1->draw();
    propellor2->draw();
    propellor3->draw();
    propellor4->draw();
    tailBody->draw();
  }

  void addSpeed(float delta)
  {
    propellorSpeed += delta;
    if (propellorSpeed < 0)
    {
      propellorSpeed = 0;
    }
  }

  void run()
  {
    propellorAngle += propellorSpeed;
    updateTransforms();
  }

  void setFlapAngle(float angle)
  {
    flapAngle += glm::clamp(angle, -maxFlapAngle, maxFlapAngle);
    updateTransforms();
  }

  void adjustFlaps(float delta)
  {
    flapAngle = glm::clamp(flapAngle + delta, -maxFlapAngle, maxFlapAngle);
    updateTransforms();
  }

  float getFlapAngle() const { return flapAngle; }

  void pitch(float degrees)
  {
    float radians = glm::radians(degrees);
    glm::mat4 rotation = Math::rotate(glm::mat4(1.0f), radians, right);

    forward = glm::vec3(rotation * glm::vec4(forward, 0.0f));
    up = glm::vec3(rotation * glm::vec4(up, 0.0f));

    updateTransforms();
  }

  void yaw(float degrees)
  {
    rotateLocal(forward, degrees);
    updateTransforms();
  }

  void roll(float degrees)
  {
    rotateLocal(up, degrees);
    updateTransforms();
  }

  void resetOrientation()
  {
    forward = glm::vec3(0.0f, 0.0f, -1.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);
    right = glm::vec3(1.0f, 0.0f, 0.0f);
    updateTransforms();
  }

  void changeZ(float amount)
  {
    this->position.z += amount;
    updateTransforms();
  }

  void changeX(float amount)
  {
    this->position.x += amount;
    updateTransforms();
  }

  void changeY(float amount)
  {
    this->position.y += amount;
    updateTransforms();
  }
};

void handleKeyPresses(GLFWwindow *window, Airplane &plane)
{
  const float rotationAmount = 2.0f;
  const float flapAmount = 10.0f;

  if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS)
  {
    plane.addSpeed(1.0f);
  }
  if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS)
  {
    plane.addSpeed(-1.0f);
  }
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
  {
    plane.adjustFlaps(-flapAmount);
    plane.pitch(-rotationAmount);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
  {
    plane.adjustFlaps(flapAmount);
    plane.pitch(rotationAmount);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    plane.yaw(rotationAmount);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    plane.yaw(-rotationAmount);
  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    plane.roll(-rotationAmount);
  if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    plane.roll(rotationAmount);
  if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
    plane.changeY(1.0f);
  if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
    plane.changeY(-1.0f);
  if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    plane.changeX(1.0f);
  if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
    plane.changeX(-1.0f);
  if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
    plane.changeZ(1.0f);
  if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
    plane.changeZ(-1.0f);
  if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    plane.resetOrientation();
  if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
    wireframeMode = !wireframeMode;
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

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  GLuint programID = LoadShaders("vertex.glsl", "fragment.glsl");

  GLuint MatrixID = glGetUniformLocation(programID, "MVP");

  glm::mat4 Projection = Math::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);

  glm::mat4 View = Math::lookAt(
      glm::vec3(0, 20, 20),
      glm::vec3(0, 0, 0),
      glm::vec3(0, 1, 0));

  glm::mat4 MVP = Projection * View;

  Airplane plane(1.319784f, 9.83f, getColor(0.2f, 0.5f, 0.2f, 1000), 11.28f, 4.0f, 0.3f, getColor(0.2f, 0.6f, 0.2, 1000));
  plane.upload();

  do
  {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    MVP = Projection * View;
    glUseProgram(programID);
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

    plane.draw();
    plane.run();
    glfwSwapBuffers(window);
    glfwPollEvents();

    handleKeyPresses(window, plane);

  } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && !glfwWindowShouldClose(window));

  glDeleteProgram(programID);
  glfwTerminate();
  return 0;
}