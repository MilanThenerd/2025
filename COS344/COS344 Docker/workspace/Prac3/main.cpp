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

GLfloat* vertices(const std::string& filename, int& vertexCount)
{
  std::ifstream file(filename);
  if (!file.is_open()) {
      std::cerr << "Failed to open file: " << filename << std::endl;
      return nullptr;
  }

  std::vector<GLfloat> vertexList;
  std::string line;

  while (std::getline(file, line)) {
      std::istringstream iss(line);
      GLfloat x, y, z;
      
      if (iss >> x >> y >> z) {
          vertexList.push_back(x);
          vertexList.push_back(y);
          vertexList.push_back(z);
      }
  }

  file.close();

  if (vertexList.empty()) 
  {
      std::cerr << "No vertices found in file: " << filename << std::endl;
      return nullptr;
  }
  GLfloat* vertices = new GLfloat[vertexList.size()];
  std::copy(vertexList.begin(), vertexList.end(), vertices);
  vertexCount = static_cast<int>(vertexList.size());
  
  return vertices;
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

    if (wireframeMode)
    {
      glBindVertexArray(VAO);
      glDrawArrays(GL_LINES, 0, vertexCount);
      glBindVertexArray(0);
    }
    else
    {
      glBindVertexArray(VAO);
      glDrawArrays(GL_TRIANGLES, 0, vertexCount);
      glBindVertexArray(0);
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

  void printVertices()
  {
    for(int i = 0 ; i < vertexCount * 3  ; i += 3)
    {
      std::cout << vertices[i] << " " << vertices[i+1] << " " << vertices[i+2] << "\n";
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

GLfloat *createCuboid(float width = 1.0f, float height = 1.0f, float length = 1.0f, float offsetX = 0.0f, float offsetY = 0.0f, float offsetZ = 0.0f, float scaleX = 1.0f, float scaleY = 1.0f, float scaleZ = 1.0f)
{
  GLfloat *vertices = new GLfloat[108];

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

  vertices[18] = 0.0f * scaleX + offsetX;
  vertices[19] = 0.0f * scaleY + offsetY;
  vertices[20] = width * scaleZ + offsetZ;

  vertices[21] = length;
  vertices[22] = height;
  vertices[23] = width;

  vertices[24] = length;
  vertices[25] = 0.0f;
  vertices[26] = width;

  vertices[27] = 0.0f * scaleX + offsetX;
  vertices[28] = 0.0f * scaleY + offsetY;
  vertices[29] = width * scaleZ + offsetZ;

  vertices[30] = 0.0f * scaleX + offsetX;
  vertices[31] = height * scaleY + offsetY;
  vertices[32] = width * scaleZ + offsetZ;

  vertices[33] = length;
  vertices[34] = height;
  vertices[35] = width;

  vertices[36] = 0.0f;
  vertices[37] = 0.0f;
  vertices[38] = 0.0f;

  vertices[39] = 0.0f;
  vertices[40] = height;
  vertices[41] = 0.0f;

  vertices[42] = 0.0f * scaleX + offsetX;
  vertices[43] = height * scaleY + offsetY;
  vertices[44] = width * scaleZ + offsetZ;

  vertices[45] = 0.0f;
  vertices[46] = 0.0f;
  vertices[47] = 0.0f;

  vertices[48] = 0.0f * scaleX + offsetX;
  vertices[49] = height * scaleY + offsetY;
  vertices[50] = width * scaleZ + offsetZ;

  vertices[51] = 0.0f * scaleX + offsetX;
  vertices[52] = 0.0f * scaleY + offsetY;
  vertices[53] = width * scaleZ + offsetZ;

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

  vertices[84] = 0.0f * scaleX + offsetX;
  vertices[85] = 0.0f * scaleY + offsetY;
  vertices[86] = width * scaleZ + offsetZ;

  vertices[87] = length;
  vertices[88] = 0.0f;
  vertices[89] = width;

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

  vertices[105] = 0.0f * scaleX + offsetX;
  vertices[106] = height * scaleY + offsetY;
  vertices[107] = width * scaleZ + offsetZ;

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

GLfloat *createCylinder(float radius = 0.5f, float height = 1.0f, float scale = 1.0, int segments = 36)
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
  Polyhedrons *starDeco;
  Polyhedrons *starDeco_1;
  Polyhedrons *wingLeft;
  Polyhedrons *wingFlapLeft;
  Polyhedrons *wingRight;
  Polyhedrons *wingFlapRight;

  Polyhedrons *cowlBody;
  Polyhedrons *cowlIntake;
  Polyhedrons *cowlOutlet_1;
  Polyhedrons *cowlOutlet_2;
  Polyhedrons *cowlOutlet_3;
  Polyhedrons *cowlOutlet_4;
  Polyhedrons *cowlOutlet_5;

  Polyhedrons *radiatorBody;
  Polyhedrons *radiatorIntake;

  Polyhedrons *tailCone;
  Polyhedrons *noseBody;
  Polyhedrons *noseBody_1;
  Polyhedrons *noseBody_2;
  Polyhedrons *tailBody;

  Polyhedrons *tailWingLeft;
  Polyhedrons *tailWingFlapLeft;
  Polyhedrons *tailWingRight;
  Polyhedrons *tailWingFlapRight;

  Polyhedrons *tailFin;
  Polyhedrons *tailFinFlap;

  Polyhedrons *propellor1;
  Polyhedrons *propellor2;

  Polyhedrons *propellor3;
  Polyhedrons *propellor4;

  Polyhedrons *cockpit;

  Polyhedrons *leftLeg;
  Polyhedrons *rightLeg;
  Polyhedrons *backLeg;

  Polyhedrons *leftWheel;
  Polyhedrons *leftWheelRim;

  Polyhedrons *rightWheel;
  Polyhedrons *rightWheelRim;

  Polyhedrons *backWheel;
  Polyhedrons *backWheelRim;

  Polyhedrons *frontLeftWingDecoration;
  Polyhedrons *frontRightWingDecoration;

  glm::vec3 position = glm::vec3(0.0f);
  glm::vec3 forward = glm::vec3(0.0f, 0.0f, -1.0f);
  glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);

  glm::vec3 bodyOffset = glm::vec3(-0.5f, 0.0f, -1.0f);
  glm::vec3 starDecoOffset = glm::vec3(0.0f , 0.0f , 0.0f);
  glm::vec3 starDeco_1Offset = glm::vec3(0.0f , 0.0f , 0.0f);
  glm::vec3 cockpitOffset = glm::vec3(0.0f, 0.0f, 0.0f);

  glm::vec3 wingLeftOffset = glm::vec3(-2.0f, 0.5f, 0.0f);
  glm::vec3 wingLeftFlapOffset = glm::vec3(2.0f, 0.5f, 0.0f);
  glm::vec3 wingRightOffset = glm::vec3(2.0f, 0.5f, 0.0f);
  glm::vec3 wingRightFlapOffset = glm::vec3(2.0f, 0.5f, 0.0f);

  glm::vec3 tailWingLeftOffset = glm::vec3(-2.0f, 0.5f, 0.0f);
  glm::vec3 tailWingLeftFlapOffset = glm::vec3(2.0f, 0.5f, 0.0f);
  glm::vec3 tailWingRightOffset = glm::vec3(2.0f, 0.5f, 0.0f);
  glm::vec3 tailWingRightFlapOffset = glm::vec3(2.0f, 0.5f, 0.0f);

  glm::vec3 tailFinOffset = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 tailFinFlapOffset = glm::vec3(0.0f, 0.0f, 0.0f);

  glm::vec3 tailConeOffset = glm::vec3(-0.5f, 0.5f, -1.0f);
  glm::vec3 noseBodyOffset = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 tailBodyOffset = glm::vec3(0.0f, 0.0f, 0.0f);

  glm::vec3 propellorOffset1 = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 propellorOffset2 = glm::vec3(0.0f, 0.0f, 0.0f);

  glm::vec3 propellorOffset3 = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 propellorOffset4 = glm::vec3(0.0f, 0.0f, 0.0f);

  glm::vec3 leftLegOffset = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 rightLegOffset = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 backLegOffset = glm::vec3(0.0f, 0.0f, 0.0f);

  float leftWingFlapAngle = 0.0f;
  float rightWingFlapAngle = 0.0f;
  float leftTailWingFlapAngle = 0.0f;
  float rightTailWingFlapAngle = 0.0f;
  float maxFlapAngle = 30.0f;

  float propellorSpeed = 0.0f;
  float propellorAngle = 0.0f;

  float finAngle = 0.0f;
  float maxFinAngle = 30.0f;

  bool wheelsUp = false;
  float legRotation = 0.0f;

  float wheelSpeed = 0.0f;
  float wheelRotation = 0.0f;

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
    body->setTransformation(bodyTransform);

    glm::mat4 cowlBodyTransform = transform;
    cowlBodyTransform = Math::translate(cowlBodyTransform, bodyOffset);
    cowlBodyTransform = Math::rotate(cowlBodyTransform, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    cowlBodyTransform = Math::translate(cowlBodyTransform, glm::vec3(0.0f, -4.0f, -1.25f));
    cowlBody->setTransformation(cowlBodyTransform);

    glm::mat4 cowlIntakeTransform = transform;
    cowlIntakeTransform = Math::translate(cowlIntakeTransform, bodyOffset);
    cowlIntakeTransform = Math::rotate(cowlIntakeTransform, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    cowlIntakeTransform = Math::translate(cowlIntakeTransform, glm::vec3(0.0f, -4.01f, -1.25f));
    cowlIntake->setTransformation(cowlIntakeTransform);

    glm::mat4 radiatorBodyTransform = transform;
    radiatorBodyTransform = Math::translate(radiatorBodyTransform, bodyOffset);
    radiatorBodyTransform = Math::rotate(radiatorBodyTransform, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    radiatorBodyTransform = Math::translate(radiatorBodyTransform, glm::vec3(-0.75f, 0.0f, -2.0f));
    radiatorBodyTransform = Math::rotate(radiatorBodyTransform, glm::radians(-20.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    radiatorBody->setTransformation(radiatorBodyTransform);

    glm::mat4 radiatorIntakeTransform = transform;
    radiatorIntakeTransform = Math::translate(radiatorIntakeTransform, bodyOffset);
    radiatorIntakeTransform = Math::rotate(radiatorIntakeTransform, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    radiatorIntakeTransform = Math::translate(radiatorIntakeTransform, glm::vec3(-0.5f, -0.1f, -1.9f));
    radiatorIntakeTransform = Math::rotate(radiatorIntakeTransform, glm::radians(-20.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    radiatorIntake->setTransformation(radiatorIntakeTransform);


    glm::mat4 starDecoTransform = transform;
    starDecoTransform = Math::translate(starDecoTransform, starDecoOffset);
    starDecoTransform = Math::rotate(starDecoTransform, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    starDeco->setTransformation(starDecoTransform);

    glm::mat4 starDeco_1Transform = transform;
    starDeco_1Transform = Math::translate(starDeco_1Transform, starDeco_1Offset);
    starDeco_1Transform = Math::rotate(starDeco_1Transform, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    starDeco_1->setTransformation(starDeco_1Transform);


    glm::mat4 wingLeftTransform = transform;
    wingLeftTransform = Math::translate(wingLeftTransform, wingLeftOffset);
    wingLeftTransform = Math::rotate(wingLeftTransform, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    wingLeftTransform = Math::rotate(wingLeftTransform, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    wingLeft->setTransformation(wingLeftTransform);

    glm::mat4 wingFlapLeftTransform = transform;
    wingFlapLeftTransform = Math::translate(wingFlapLeftTransform, wingLeftFlapOffset);
    wingFlapLeftTransform = Math::rotate(wingFlapLeftTransform, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    wingFlapLeftTransform = Math::rotate(wingFlapLeftTransform, glm::radians(-10.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    wingFlapLeftTransform = Math::rotate(wingFlapLeftTransform, glm::radians(-leftWingFlapAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    wingFlapLeft->setTransformation(wingFlapLeftTransform);

    glm::mat4 wingLeftDecoTransform = transform;
    wingLeftDecoTransform = Math::translate(wingLeftDecoTransform, wingLeftOffset);
    wingLeftDecoTransform = Math::rotate(wingLeftDecoTransform, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    wingLeftDecoTransform = Math::rotate(wingLeftDecoTransform, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    wingLeftDecoTransform = Math::translate(wingLeftDecoTransform, glm::vec3(4.0f , 0.0f , 11.3f));
    frontLeftWingDecoration->setTransformation(wingLeftDecoTransform);

    glm::mat4 wingRightTransform = transform;
    wingRightTransform = Math::translate(wingRightTransform, wingRightOffset);
    wingRightTransform = Math::rotate(wingRightTransform, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    wingRightTransform = Math::rotate(wingRightTransform, glm::radians(180.0f), glm::vec3(0.0f, .0f, 1.0f));
    wingRight->setTransformation(wingRightTransform);

    glm::mat4 wingFlapRightTransform = transform;
    wingFlapRightTransform = Math::translate(wingFlapRightTransform, wingRightFlapOffset);
    wingFlapRightTransform = Math::rotate(wingFlapRightTransform, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    wingFlapRightTransform = Math::rotate(wingFlapRightTransform, glm::radians(-10.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    wingFlapRightTransform = Math::rotate(wingFlapRightTransform, glm::radians(rightWingFlapAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    wingFlapRight->setTransformation(wingFlapRightTransform);

    glm::mat4 wingRightDecoTransform = transform;
    wingRightDecoTransform = Math::translate(wingRightDecoTransform, wingRightOffset);
    wingRightDecoTransform = Math::rotate(wingRightDecoTransform, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    wingRightDecoTransform = Math::rotate(wingRightDecoTransform, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    wingRightDecoTransform = Math::translate(wingRightDecoTransform, glm::vec3(4.0f , 0.0f , -11.3f));
    frontRightWingDecoration->setTransformation(wingRightDecoTransform);

    glm::mat4 tailConeTransform = transform;
    tailConeTransform = Math::translate(tailConeTransform, tailConeOffset);
    tailConeTransform = Math::rotate(tailConeTransform, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    tailCone->setTransformation(tailConeTransform);

    glm::mat4 tailBodyTransform = transform;
    tailBodyTransform = Math::translate(tailBodyTransform, tailBodyOffset);
    tailBodyTransform = Math::rotate(tailBodyTransform, glm::radians(90.0f), glm::vec3(1.0f, .0f, 0.0f));
    tailBody->setTransformation(tailBodyTransform);

    glm::mat4 tailWingLeftTransform = transform;
    tailWingLeftTransform = Math::translate(tailWingLeftTransform, tailWingLeftOffset);
    tailWingLeftTransform = Math::rotate(tailWingLeftTransform, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    tailWingLeftTransform = Math::rotate(tailWingLeftTransform, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    tailWingLeft->setTransformation(tailWingLeftTransform);

    glm::mat4 tailWingLeftFlapTransform = transform;
    tailWingLeftFlapTransform = Math::translate(tailWingLeftFlapTransform, tailWingLeftFlapOffset);
    tailWingLeftFlapTransform = Math::rotate(tailWingLeftFlapTransform, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    tailWingLeftFlapTransform = Math::rotate(tailWingLeftFlapTransform, glm::radians(-12.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    tailWingLeftFlapTransform = Math::rotate(tailWingLeftFlapTransform, glm::radians(-leftTailWingFlapAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    tailWingFlapLeft->setTransformation(tailWingLeftFlapTransform);

    glm::mat4 tailWingRightTransform = transform;
    tailWingRightTransform = Math::translate(tailWingRightTransform, tailWingRightOffset);
    tailWingRightTransform = Math::rotate(tailWingRightTransform, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    tailWingRightTransform = Math::rotate(tailWingRightTransform, glm::radians(180.0f), glm::vec3(0.0f, .0f, 1.0f));
    tailWingRight->setTransformation(tailWingRightTransform);

    glm::mat4 tailWingRightFlapTransform = transform;
    tailWingRightFlapTransform = Math::translate(tailWingRightFlapTransform, tailWingRightFlapOffset);
    tailWingRightFlapTransform = Math::rotate(tailWingRightFlapTransform, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    tailWingRightFlapTransform = Math::rotate(tailWingRightFlapTransform, glm::radians(-12.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    tailWingRightFlapTransform = Math::rotate(tailWingRightFlapTransform, glm::radians(rightTailWingFlapAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    tailWingFlapRight->setTransformation(tailWingRightFlapTransform);

    glm::mat4 noseBodyTransform = transform;
    noseBodyTransform = Math::translate(noseBodyTransform, noseBodyOffset);
    noseBodyTransform = Math::rotate(noseBodyTransform, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    noseBody->setTransformation(noseBodyTransform);

    glm::mat4 noseBody_1Transform = transform;
    noseBody_1Transform = Math::translate(noseBody_1Transform, noseBodyOffset);
    noseBody_1Transform = Math::rotate(noseBody_1Transform, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    noseBody_1Transform = Math::translate(noseBody_1Transform, glm::vec3(0.0f , 0.0f , 2.0f));
    noseBody_1->setTransformation(noseBody_1Transform);

    glm::mat4 noseBody_2Transform = transform;
    noseBody_2Transform = Math::translate(noseBody_2Transform, noseBodyOffset);
    noseBody_2Transform = Math::rotate(noseBody_2Transform, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    noseBody_2Transform = Math::translate(noseBody_2Transform, glm::vec3(0.0f , 0.0f , 1.15f));
    noseBody_2->setTransformation(noseBody_2Transform);

    glm::mat4 propellorTransform1 = transform;
    propellorTransform1 = Math::translate(propellorTransform1, propellorOffset1);
    propellorTransform1 = Math::rotate(propellorTransform1, glm::radians(-propellorAngle), glm::vec3(0.0f, 0.0f, 1.0f));
    propellor1->setTransformation(propellorTransform1);

    glm::mat4 propellorTransform2 = transform;
    propellorTransform2 = Math::translate(propellorTransform2, propellorOffset2);
    propellorTransform2 = Math::rotate(propellorTransform2, glm::radians(-propellorAngle + 180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    propellor2->setTransformation(propellorTransform2);

    glm::mat4 propellorTransform3 = transform;
    propellorTransform3 = Math::translate(propellorTransform3, propellorOffset3);
    propellorTransform3 = Math::rotate(propellorTransform3, glm::radians(propellorAngle), glm::vec3(0.0f, 0.0f, 1.0f));
    propellor3->setTransformation(propellorTransform3);

    glm::mat4 propellorTransform4 = transform;
    propellorTransform4 = Math::translate(propellorTransform4, propellorOffset4);
    propellorTransform4 = Math::rotate(propellorTransform4, glm::radians(propellorAngle + 180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    propellor4->setTransformation(propellorTransform4);

    glm::mat4 tailFinTransform = transform;
    tailFinTransform = Math::translate(tailFinTransform, tailFinOffset);
    tailFinTransform = Math::rotate(tailFinTransform, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    tailFin->setTransformation(tailFinTransform);

    glm::mat4 tailFinFlapTransform = transform;
    tailFinFlapTransform = Math::translate(tailFinFlapTransform, tailFinFlapOffset);
    tailFinFlapTransform = Math::rotate(tailFinFlapTransform, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    tailFinFlapTransform = Math::rotate(tailFinFlapTransform, glm::radians(finAngle), glm::vec3(0.0, 1.0f, 0.0f));
    tailFinFlap->setTransformation(tailFinFlapTransform);

    glm::mat4 cockpitTransform = transform;
    cockpitTransform = Math::translate(cockpitTransform, cockpitOffset);
    cockpitTransform = Math::rotate(cockpitTransform, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    cockpit->setTransformation(cockpitTransform);

    glm::mat4 leftLegTransform = transform;
    leftLegTransform = Math::translate(leftLegTransform, leftLegOffset);
    leftLegTransform = Math::rotate(leftLegTransform, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    leftLegTransform = Math::rotate(leftLegTransform, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    leftLegTransform = Math::rotate(leftLegTransform, glm::radians(legRotation), glm::vec3(1.0f, 0.0f, 0.0f));
    leftLeg->setTransformation(leftLegTransform);

    glm::mat4 leftWheelTransform = transform;
    leftWheelTransform = Math::translate(leftWheelTransform, leftLegOffset);
    leftWheelTransform = Math::rotate(leftWheelTransform, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    leftWheelTransform = Math::rotate(leftWheelTransform, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    leftWheelTransform = Math::rotate(leftWheelTransform, glm::radians(legRotation), glm::vec3(1.0f, 0.0f, 0.0f));
    leftWheelTransform = Math::translate(leftWheelTransform, glm::vec3(0.0f, 0.0f, -3.0f));
    leftWheel->setTransformation(leftWheelTransform);

    glm::mat4 leftWheelRimTransform = transform;
    leftWheelRimTransform = Math::translate(leftWheelRimTransform, leftLegOffset);
    leftWheelRimTransform = Math::rotate(leftWheelRimTransform, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    leftWheelRimTransform = Math::rotate(leftWheelRimTransform, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    leftWheelRimTransform = Math::rotate(leftWheelRimTransform, glm::radians(legRotation), glm::vec3(1.0f, 0.0f, 0.0f));
    leftWheelRimTransform = Math::translate(leftWheelRimTransform, glm::vec3(0.0f, -0.1f, -3.0f));
    leftWheelRimTransform = Math::rotate(leftWheelRimTransform, glm::radians(-wheelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
    leftWheelRim->setTransformation(leftWheelRimTransform);

    glm::mat4 rightLegTransform = transform;
    rightLegTransform = Math::translate(rightLegTransform, rightLegOffset);
    rightLegTransform = Math::rotate(rightLegTransform, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rightLegTransform = Math::rotate(rightLegTransform, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    rightLegTransform = Math::rotate(rightLegTransform, glm::radians(-legRotation), glm::vec3(1.0f, 0.0f, 0.0f));
    rightLeg->setTransformation(rightLegTransform);

    glm::mat4 rightWheelTransform = transform;
    rightWheelTransform = Math::translate(rightWheelTransform, rightLegOffset);
    rightWheelTransform = Math::rotate(rightWheelTransform, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rightWheelTransform = Math::rotate(rightWheelTransform, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    rightWheelTransform = Math::rotate(rightWheelTransform, glm::radians(-legRotation), glm::vec3(1.0f, 0.0f, 0.0f));
    rightWheelTransform = Math::translate(rightWheelTransform, glm::vec3(0.0f, 0.3f, -3.0f));
    rightWheel->setTransformation(rightWheelTransform);

    glm::mat4 rightWheelRimTransform = transform;
    rightWheelRimTransform = Math::translate(rightWheelRimTransform, rightLegOffset);
    rightWheelRimTransform = Math::rotate(rightWheelRimTransform, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rightWheelRimTransform = Math::rotate(rightWheelRimTransform, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    rightWheelRimTransform = Math::rotate(rightWheelRimTransform, glm::radians(-legRotation), glm::vec3(1.0f, 0.0f, 0.0f));
    rightWheelRimTransform = Math::translate(rightWheelRimTransform, glm::vec3(0.0f, 0.3f, -3.0f));
    rightWheelRimTransform = Math::rotate(rightWheelRimTransform, glm::radians(-wheelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
    rightWheelRim->setTransformation(rightWheelRimTransform);

    glm::mat4 backLegTransform = transform;
    backLegTransform = Math::translate(backLegTransform, backLegOffset);
    backLegTransform = Math::rotate(backLegTransform, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    backLegTransform = Math::rotate(backLegTransform, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    backLegTransform = Math::rotate(backLegTransform, glm::radians(legRotation), glm::vec3(0.0f, 1.0f, 0.0f));
    backLeg->setTransformation(backLegTransform);

    glm::mat4 backWheelTransform = transform;
    backWheelTransform = Math::translate(backWheelTransform, backLegOffset);
    backWheelTransform = Math::rotate(backWheelTransform, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    backWheelTransform = Math::rotate(backWheelTransform, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    backWheelTransform = Math::rotate(backWheelTransform, glm::radians(legRotation), glm::vec3(0.0f, 1.0f, 0.0f));
    backWheelTransform = Math::translate(backWheelTransform, glm::vec3(0.0, 0.125f, -1.0f));
    backWheel->setTransformation(backWheelTransform);

    glm::mat4 backWheelRimTransform = transform;
    backWheelRimTransform = Math::translate(backWheelRimTransform, backLegOffset);
    backWheelRimTransform = Math::rotate(backWheelRimTransform, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    backWheelRimTransform = Math::rotate(backWheelRimTransform, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    backWheelRimTransform = Math::rotate(backWheelRimTransform, glm::radians(legRotation), glm::vec3(.0f, 1.0f, 0.0f));
    backWheelRimTransform = Math::translate(backWheelRimTransform, glm::vec3(0.0, 0.125f, -1.0f));
    backWheelRimTransform = Math::rotate(backWheelRimTransform, glm::radians(-wheelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
    backWheelRim->setTransformation(backWheelRimTransform);

    glm::mat4 cowlOutlet_1Transform = transform;
    cowlOutlet_1Transform = Math::translate(cowlOutlet_1Transform, bodyOffset);
    cowlOutlet_1Transform = Math::rotate(cowlOutlet_1Transform, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    cowlOutlet_1Transform = Math::rotate(cowlOutlet_1Transform, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    cowlOutlet_1Transform = Math::translate(cowlOutlet_1Transform, glm::vec3(2.0f, -0.5f, 1.0f));
    cowlOutlet_1->setTransformation(cowlOutlet_1Transform);
  }

public:
  Airplane(float bodyWidth, float bodyLength, GLfloat *bodyColor,
           float wingSpan, float wingWidth, float wingHeight, GLfloat *wingColor)
  {
    // P-51D Mustang specifications

    body = new Polyhedrons(createCylinder(bodyWidth, bodyLength), bodyColor, 36 * 12);
    bodyOffset = glm::vec3(0, 0, 0);

    cowlBody = new Polyhedrons(createCylinder(0.5f, 1.0f), bodyColor, 36 * 12);
    cowlIntake = new Polyhedrons(createCylinder(0.35f, 1.0f), getColor(0.0f , 0.0f , 0.0f , 36*12), 36 * 12);

    radiatorBody = new Polyhedrons(createCuboid(2.0f, 3.0f, 1.5f) , bodyColor, 36);
    radiatorIntake = new Polyhedrons(createCuboid(1.5f, 2.0f, 1.0f) , getColor(0.0f , 0.0f , 0.0f , 36*12), 36);

    int vertexCount = 0;
    GLfloat* verts = vertices("vertex_output.txt", vertexCount);
    starDeco = new Polyhedrons(verts, getColor(0.8f , 0.1f , 0.1f, vertexCount/3) , vertexCount/3);

    starDecoOffset.x = 5.0f;
    starDecoOffset.y = -bodyWidth-0.2f;
    starDecoOffset.z = 2.0f;

    starDeco_1 = new Polyhedrons(verts, getColor(0.8f , 0.1f , 0.1f, vertexCount/3) , vertexCount/3);

    starDeco_1Offset.x = -5.0f;
    starDeco_1Offset.y = -bodyWidth-0.2f;
    starDeco_1Offset.z = 2.0f;

    cockpit = new Polyhedrons(createCuboid(2.0f, 2.0f, 2.0f, 0.0f, 0.0f, 2.0f), getColor(0.0f, 0.0f, 1.0f, 1000), 36);
    cockpitOffset = glm::vec3(1.0f, -0.5f, -1.0f);

    wingLeft = new Polyhedrons(createCuboid(wingSpan, wingHeight, wingWidth, 2.0f, 0.0f, 0.0f), wingColor, 36);
    wingLeftOffset = glm::vec3(0, -bodyWidth, -bodyLength * 0.1);

    frontLeftWingDecoration =  new Polyhedrons(createCuboid(-wingSpan, wingHeight, -wingWidth*0.1, 1.0f, 0.0f, 0.0f), wingColor, 36);
    frontRightWingDecoration =  new Polyhedrons(createCuboid(wingSpan, wingHeight, -wingWidth*0.1, 1.0f, 0.0f, 0.0f), wingColor, 36);

    wingFlapLeft = new Polyhedrons(createTriangularPrism(wingSpan, -(wingWidth * 0.2), wingHeight), getColor(1.0, 0.9, 0.9, 24), 24);
    wingLeftFlapOffset = glm::vec3(0, (-bodyWidth), -(wingWidth * 0.2));

    wingRight = new Polyhedrons(createCuboid(-wingSpan, wingHeight, wingWidth, 2.0f, 0.0f, 0.0f), wingColor, 36);
    wingRightOffset = glm::vec3(0, -bodyWidth, -bodyLength * 0.1);

    wingFlapRight = new Polyhedrons(createTriangularPrism(wingSpan, -(wingWidth * 0.2), wingHeight), getColor(1.0, 0.9, 0.9, 24), 24);
    wingRightFlapOffset = glm::vec3(0, (-bodyWidth - wingHeight), -(wingWidth * 0.2));

    noseBody = new Polyhedrons(createCone(bodyWidth, bodyLength * 0.3), bodyColor, 36 * 6);
    noseBodyOffset = glm::vec3(0, 0, bodyLength / 2);

    noseBody_1 = new Polyhedrons(createCone(bodyWidth*0.4, bodyLength * 0.1), getColor(1.0, 0.0, 0.0, 36 * 6), 36 * 6);
    noseBody_2 = new Polyhedrons(createCone(bodyWidth*0.7, bodyLength * 0.17), wingColor, 36 * 6);

    propellor1 = new Polyhedrons(createCuboid(0.5f, 2.0f, 0.1f), getColor(0.0, 0.0, 0.0, 36), 36);
    propellor2 = new Polyhedrons(createCuboid(0.5f, 2.0f, 0.1f), getColor(0.0, 0.0, 0.0, 36), 36);

    propellor3 = new Polyhedrons(createCuboid(0.5f, 2.0f, 0.1f), getColor(0.0, 0.0, 0.0, 36), 36);
    propellor4 = new Polyhedrons(createCuboid(0.5f, 2.0f, 0.1f), getColor(0.0, 0.0, 0.0, 36), 36);

    propellorOffset1 = glm::vec3(0, 0, (bodyLength / 2 + bodyLength * 0.2));
    propellorOffset2 = glm::vec3(0, 0, (bodyLength / 2 + bodyLength * 0.2));

    propellorOffset3 = glm::vec3(0, 0, (bodyLength / 2 + bodyLength * 0.12));
    propellorOffset4 = glm::vec3(0, 0, (bodyLength / 2 + bodyLength * 0.12));


    cowlOutlet_1 = new Polyhedrons(createCylinder(0.1f , 1.0f), getColor(0.0f , 0.0f , 0.0f , 36*12) , 36*12);


    tailCone = new Polyhedrons(createCone(bodyWidth * 0.6, bodyLength * 0.1), bodyColor, 36 * 6);
    tailConeOffset = glm::vec3(0, 0, -bodyLength * 0.95);

    tailBody = new Polyhedrons(createCylinder(bodyWidth, bodyLength * 0.5, 0.6), bodyColor, 36 * 12);
    tailBodyOffset = glm::vec3(0, 0, -bodyLength * 0.7);

    tailWingLeft = new Polyhedrons(createCuboid(wingSpan * 0.4, wingHeight, wingWidth * 0.4, 1.0f, 0.0f, 0.0f), wingColor, 36);
    tailWingLeftOffset = glm::vec3(0, bodyWidth * 0.725, -bodyLength * 0.9);

    tailWingFlapLeft = new Polyhedrons(createTriangularPrism(wingSpan * 0.4, -(wingWidth * 0.1), wingHeight), getColor(1.0, 0.9, 0.9, 24), 24);
    tailWingLeftFlapOffset = glm::vec3(0.0f, bodyWidth * 0.725, -bodyLength * 0.9);

    tailWingRight = new Polyhedrons(createCuboid(-wingSpan * 0.4, wingHeight, wingWidth * 0.4, 1.0f, 0.0f, 0.0f), wingColor, 36);
    tailWingRightOffset = glm::vec3(0, bodyWidth * 0.725, -bodyLength * 0.9);

    tailWingFlapRight = new Polyhedrons(createTriangularPrism(wingSpan * 0.4, -(wingWidth * 0.1), -wingHeight), getColor(1.0, 0.9, 0.9, 24), 24);
    tailWingRightFlapOffset = glm::vec3(0.0f, bodyWidth * 0.725, -bodyLength * 0.9);

    tailFin = new Polyhedrons(createTriangularPrism(4.0f, wingWidth * 0.7, 0.5f), wingColor, 24);
    tailFinOffset = glm::vec3(-0.5f / 2, -bodyWidth / 2, -bodyLength * 1.05);

    tailFinFlap = new Polyhedrons(createTriangularPrism(4.0f, -wingWidth * 0.3, 0.5f), getColor(1.0f, 1.0f, 1.0f, 1000), 24);
    tailFinFlapOffset = glm::vec3(-0.5f / 2, -bodyWidth / 2, -bodyLength * 1.04);

    leftLeg = new Polyhedrons(createCuboid(-3.0f, 0.25f, 1.0f, -1.0f), getColor(0.6, 0.6, 0.6, 36), 36);
    leftLegOffset = glm::vec3(wingSpan * 0.4, -bodyWidth - 0.15f, 1.0f);

    leftWheel = new Polyhedrons(createCylinder(0.75f, 0.1), getColor(0.0f, 0.0f, 0.0f, 36 * 12), 36 * 12);
    leftWheelRim = new Polyhedrons(createCuboid(0.75f, 0.1f, 0.1f), getColor(1.0f, 1.0f, 1.0f, 36), 36);

    rightLeg = new Polyhedrons(createCuboid(-3.0f, 0.25f, 1.0f, -1.0f), getColor(0.6, 0.6, 0.6, 36), 36);
    rightLegOffset = glm::vec3(-wingSpan * 0.4, -bodyWidth - 0.45f, 1.0f);

    rightWheel = new Polyhedrons(createCylinder(0.75f, 0.1), getColor(0.0f, 0.0f, 0.0f, 36 * 12), 36 * 12);
    rightWheelRim = new Polyhedrons(createCuboid(0.75f, 0.1f, 0.1f), getColor(1.0f, 1.0f, 1.0f, 36), 36);

    backLeg = new Polyhedrons(createCuboid(-1.0f, 0.25f, 0.5f, -0.5f), getColor(0.6, 0.6, 0.6, 36), 36);
    backLegOffset = glm::vec3(-0.125f, -0.5f, -bodyLength * 0.9);

    backWheel = new Polyhedrons(createCylinder(0.3f, 0.1), getColor(0.0f, 0.0f, 0.0f, 36 * 12), 36 * 12);
    backWheelRim = new Polyhedrons(createCuboid(0.3f, 0.1f, 0.1f), getColor(1.0f, 1.0f, 1.0f, 36), 36);

    updateTransforms();
  }

  void upload()
  {
    body->upload();
    cowlBody->upload();
    cowlIntake->upload();
    radiatorBody->upload();
    radiatorIntake->upload();
    wingLeft->upload();
    wingFlapLeft->upload();
    frontLeftWingDecoration->upload();

    wingRight->upload();
    wingFlapRight->upload();
    frontRightWingDecoration->upload();

    tailCone->upload();
    noseBody->upload();
    noseBody_1->upload();
    noseBody_2->upload();
    propellor1->upload();
    propellor2->upload();
    propellor3->upload();
    propellor4->upload();

    cowlOutlet_1->upload();

    tailBody->upload();

    tailWingLeft->upload();
    tailWingFlapLeft->upload();

    tailWingRight->upload();
    tailWingFlapRight->upload();

    tailFin->upload();
    tailFinFlap->upload();
    cockpit->upload();

    leftLeg->upload();
    leftWheel->upload();
    leftWheelRim->upload();

    rightLeg->upload();
    rightWheel->upload();
    rightWheelRim->upload();

    backLeg->upload();
    backWheel->upload();
    backWheelRim->upload();

    starDeco->upload();
    starDeco_1->upload();
  }

  void draw()
  {
    body->draw();
    cowlBody->draw();
    cowlIntake->draw();
    radiatorBody->draw();
    radiatorIntake->draw();
    starDeco->draw();
    starDeco_1->draw();
    wingLeft->draw();
    wingFlapLeft->draw();
    frontLeftWingDecoration->draw();
    frontRightWingDecoration->draw();
    tailCone->draw();
    noseBody->draw();
    noseBody_1->draw();
    noseBody_2->draw();

    cowlOutlet_1->draw();

    wingRight->draw();
    wingFlapRight->draw();
    propellor1->draw();
    propellor2->draw();
    propellor3->draw();
    propellor4->draw();
    tailBody->draw();
    tailWingLeft->draw();
    tailWingFlapLeft->draw();
    tailWingRight->draw();
    tailWingFlapRight->draw();

    tailFin->draw();
    tailFinFlap->draw();

    cockpit->draw();
    leftLeg->draw();
    leftWheel->draw();
    leftWheelRim->draw();

    rightLeg->draw();
    rightWheel->draw();
    rightWheelRim->draw();

    backLeg->draw();
    backWheel->draw();
    backWheelRim->draw();
  }

  void addSpeed(float delta)
  {
    propellorSpeed += delta;
    if (propellorSpeed < 0)
    {
      propellorSpeed = 0;
    }
    if (!wheelsUp)
    {
      wheelSpeed += delta;
      if (wheelSpeed < 0)
      {
        wheelSpeed = 0;
      }
    }
  }

  void setWheels()
  {
    wheelsUp = !wheelsUp;
  }

  void run(float speed = 1.0f)
  {
    if (wheelsUp)
    {
      legRotation += speed;
      if (legRotation >= 90.0f)
      {
        legRotation = 90.0f;
      }
    }
    else
    {
      legRotation -= speed;
      if (legRotation <= 0.0f)
      {
        legRotation = 0.0f;
      }
    }
    propellorAngle += propellorSpeed;
    wheelRotation += wheelSpeed;

    if (legRotation != 0.0f)
    {
      wheelSpeed -= 0.5f;
      if (wheelSpeed <= 0.0f)
      {
        wheelSpeed = 0.0f;
      }
    }

    speed *= 3;
    if (leftWingFlapAngle < 0)
    {
      leftWingFlapAngle += speed;
      if (leftWingFlapAngle >= 0)
      {
        leftWingFlapAngle = 0;
      }
    }
    else
    {
      leftWingFlapAngle -= speed;
      if (leftWingFlapAngle <= 0)
      {
        leftWingFlapAngle = 0;
      }
    }

    if (rightWingFlapAngle < 0)
    {
      rightWingFlapAngle += speed;
      if (rightWingFlapAngle >= 0)
      {
        rightWingFlapAngle = 0;
      }
    }
    else
    {
      rightWingFlapAngle -= speed;
      if (rightWingFlapAngle <= 0)
      {
        rightWingFlapAngle = 0;
      }
    }

    if (leftTailWingFlapAngle < 0)
    {
      leftTailWingFlapAngle += speed;
      if (leftTailWingFlapAngle >= 0)
      {
        leftTailWingFlapAngle = 0;
      }
    }
    else
    {
      leftTailWingFlapAngle -= speed;
      if (leftTailWingFlapAngle <= 0)
      {
        leftTailWingFlapAngle = 0;
      }
    }

    if (rightTailWingFlapAngle < 0)
    {
      rightTailWingFlapAngle += speed;
      if (rightTailWingFlapAngle >= 0)
      {
        rightTailWingFlapAngle = 0;
      }
    }
    else
    {
      rightTailWingFlapAngle -= speed;
      if (rightTailWingFlapAngle <= 0)
      {
        rightTailWingFlapAngle = 0;
      }
    }

    if (finAngle < 0)
    {
      finAngle += speed;
      if (finAngle >= 0)
      {
        finAngle = 0;
      }
    }
    else
    {
      finAngle -= speed;
      if (finAngle <= 0)
      {
        finAngle = 0;
      }
    }
    updateTransforms();
  }

  void setFlapAngle(float angle)
  {
    leftWingFlapAngle = glm::clamp(angle, -maxFlapAngle, maxFlapAngle);
    rightWingFlapAngle = glm::clamp(angle, -maxFlapAngle, maxFlapAngle);

    leftTailWingFlapAngle = glm::clamp(angle, -maxFlapAngle, maxFlapAngle);
    rightTailWingFlapAngle = glm::clamp(angle, -maxFlapAngle, maxFlapAngle);
    updateTransforms();
  }

  void adjustFlaps(float delta)
  {
    leftWingFlapAngle = glm::clamp(leftWingFlapAngle + delta * 5, -maxFlapAngle, maxFlapAngle);
    rightWingFlapAngle = glm::clamp(rightWingFlapAngle + delta * 5, -maxFlapAngle, maxFlapAngle);

    leftTailWingFlapAngle = glm::clamp(leftTailWingFlapAngle + delta * 5, -maxFlapAngle, maxFlapAngle);
    rightTailWingFlapAngle = glm::clamp(rightTailWingFlapAngle + delta * 5, -maxFlapAngle, maxFlapAngle);
    updateTransforms();
  }

  void pitch(float degrees)
  {
    float radians = glm::radians(degrees);
    glm::mat4 rotation = Math::rotate(glm::mat4(1.0f), radians, right);

    forward = glm::vec3(rotation * glm::vec4(forward, 0.0f));
    up = glm::vec3(rotation * glm::vec4(up, 0.0f));

    adjustFlaps(degrees);

    updateTransforms();
  }

  void yaw(float degrees)
  {
    rotateLocal(forward, degrees);
    finAngle = glm::clamp(finAngle + degrees * 5, -maxFlapAngle, maxFlapAngle);
    updateTransforms();
  }

  void roll(float degrees)
  {
    rotateLocal(up, degrees);
    leftTailWingFlapAngle = glm::clamp(leftTailWingFlapAngle - degrees * 5, -maxFlapAngle, maxFlapAngle);
    rightTailWingFlapAngle = glm::clamp(rightTailWingFlapAngle + degrees * 5, -maxFlapAngle, maxFlapAngle);
    updateTransforms();
  }

  void resetOrientation()
  {
    forward = glm::vec3(0.0f, 0.0f, -1.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);
    right = glm::vec3(1.0f, 0.0f, 0.0f);
    finAngle = 0.0f;
    propellorSpeed = 0.0f;
    propellorAngle = 0.0f;
    position.x = 0.0f;
    position.y = 0.0f;
    position.z = 0.0f;
    wheelSpeed = 0.0f;
    wheelRotation = 0.0f;
    wheelsUp = false;
    legRotation = 0.0f;
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
  static auto lastSpacePressTime = std::chrono::steady_clock::now();

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
    plane.pitch(-rotationAmount);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
  {
    plane.pitch(rotationAmount);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
  {
    plane.yaw(rotationAmount);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
  {
    plane.yaw(-rotationAmount);
  }
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
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
  {
    auto now = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastSpacePressTime);

    if (duration.count() < 100)
      return;

    plane.setWheels();
    lastSpacePressTime = now;
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

  Airplane plane(1.319784f, 9.83f, getColor(0.5f, 0.5f, 0.5f, 1000), 11.28f, 4.0f, 0.3f, getColor(0.7f, 0.7f, 0.7, 1000));
  plane.upload();

  double lastTime;
  lastTime = glfwGetTime();
  do
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(programID);
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

    plane.draw();
    plane.run();

    glfwSwapBuffers(window);
    glfwPollEvents();
    handleKeyPresses(window, plane);

    float currentTime = glfwGetTime();
    float deltaTime = currentTime - lastTime;
    lastTime = currentTime;
    cout << "FPS: " << 1 / deltaTime << endl;
  } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && !glfwWindowShouldClose(window));

  glDeleteProgram(programID);
  glfwTerminate();
  return 0;
}