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

class Polyhedrons
{
private:
  GLfloat *vertices;
  GLfloat *colors;
  GLfloat *normals;
  GLuint VAO = 0, VBO = 0, CBO = 0 , NBO = 0;
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

    normals = new GLfloat[vertexCount * 3];
    calculateNormals();
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

  void calculateNormals() 
  {
    for (int i = 0; i < vertexCount * 3; i++) {
        normals[i] = 0.0f;
    }
    
    for (int i = 0; i < vertexCount; i += 3) {
        glm::vec3 v0(vertices[i*3], vertices[i*3+1], vertices[i*3+2]);
        glm::vec3 v1(vertices[(i+1)*3], vertices[(i+1)*3+1], vertices[(i+1)*3+2]);
        glm::vec3 v2(vertices[(i+2)*3], vertices[(i+2)*3+1], vertices[(i+2)*3+2]);
        
        glm::vec3 edge1 = v1 - v0;
        glm::vec3 edge2 = v2 - v0;
        
        glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));
        
        for (int j = 0; j < 3; j++) {
            normals[(i+j)*3] += normal.x;
            normals[(i+j)*3+1] += normal.y;
            normals[(i+j)*3+2] += normal.z;
        }
    }
    
    for (int i = 0; i < vertexCount * 3; i += 3) {
        glm::vec3 normal(normals[i], normals[i+1], normals[i+2]);
        normal = glm::normalize(normal);
        normals[i] = normal.x;
        normals[i+1] = normal.y;
        normals[i+2] = normal.z;
    }
}

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

    glBindBuffer(GL_ARRAY_BUFFER, NBO);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(GLfloat), normals, GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

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

class Scene
{
private:
  Polyhedrons *bottomSheet;
  std::vector<Polyhedrons*> backSheets;
  Polyhedrons *glassSheet;
  Polyhedrons *light;
  

  glm::vec3 position = glm::vec3(0.0f);
  glm::vec3 forward = glm::vec3(0.0f, 0.0f, -1.0f);
  glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);

  glm::vec3 bottomSheetOffset = glm::vec3(100.0f, 0.5f, -2.5f);
  glm::vec3 lightOffset = glm::vec3(0.0f , 0.0f , 0.0f);

  std::vector<glm::vec4> Colors = {
    glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), // Black
    glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), // Red
    glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), // Green
    glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), // Blue
    glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), // White
    glm::vec4(1.0f, 0.5f, 0.0f, 1.0f), // Orange
    glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), // Purple
    glm::vec4(0.0f, 1.0f, 1.0f, 1.0f), // Cyan
    glm::vec4(0.5f, 0.5f, 0.0f, 1.0f), // Olive
    glm::vec4(0.5f, 0.0f, 0.5f, 1.0f)  // Dark purple
};

  float glassAlpha = 0.5f;

  int lightColor = 1;
  int glassColor = 2;
  int backSheetColor = 3;


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

    glm::mat4 bottomSheetTransform = transform;
    bottomSheetTransform = Math::translate(bottomSheetTransform, bottomSheetOffset);
    bottomSheet->setTransformation(bottomSheetTransform);

    float spacing = 0.1f;
    for (size_t i = 0; i < backSheets.size(); i++)
    {
        glm::mat4 sheetTransform = transform;
        sheetTransform = Math::translate(sheetTransform, bottomSheetOffset);
        sheetTransform = Math::translate(sheetTransform, glm::vec3(0.0f, 0.0f, 10.0f -( i * spacing)));
        sheetTransform = Math::rotate(sheetTransform, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        backSheets[i]->setTransformation(sheetTransform);
    }

    glm::mat4 glassSheetTransform = transform;
    glassSheetTransform = Math::translate(glassSheetTransform, bottomSheetOffset);
    glassSheetTransform = Math::translate(glassSheetTransform, glm::vec3(0.0f , 0.0f , 5.0f));
    glassSheetTransform = Math::rotate(glassSheetTransform, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    glassSheet->setTransformation(glassSheetTransform);

    glm::mat4 lightTransform = transform;
    lightTransform = Math::translate(lightTransform, bottomSheetOffset);
    lightTransform = Math::translate(lightTransform, glm::vec3(2.25f , 2.5f , 1.0f));
    lightTransform = Math::rotate(lightTransform, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    lightTransform = Math::translate(lightTransform, lightOffset);
    light->setTransformation(lightTransform);
  }

public:
  Scene(int numRects = 10)
  {
    bottomSheet = new Polyhedrons(createCuboid(10.0f , 0.1f , 5.0f), getColor(0.1f , 0.1f , 0.1f , 36), 36);
    bottomSheetOffset = glm::vec3(-2.5f , 0.05f , -5.0f);

    for (int i = 0; i < numRects; i++)
    {
        Polyhedrons* sheet = new Polyhedrons(createCuboid(5.0f, 0.1f, 5.0f), 
                    getColor(0.2f, 0.2f, 0.2f, 36), 36);
        backSheets.push_back(sheet);
    }
    
    glassSheet = new Polyhedrons(createCuboid(5.0f , 0.2f , 5.0f), getColor(0.0f , 0.1f , 0.5f , 36), 36);

    light = new Polyhedrons(createCuboid(0.5f  , 0.5f , 0.5f), getColor(1.0f , 1.0f , 1.0f , 36), 36);
    updateTransforms();
  }

  void cycleGlassColor(int amount)
  {
    glassColor += amount;
    glassColor = glassColor % Colors.size();
  }

  void cycleGlassAlpha(float amount)
  {
    glassAlpha += amount;
    if(glassAlpha <= 0.0f)
    {
      glassAlpha = 0.0f;
    }
    if(glassAlpha >= 1.0f)
    {
      glassAlpha = 1.0f;
    }
  }

  void cycleLightColor(int amount)
  {
    lightColor += amount;
    lightColor = lightColor % Colors.size();
    if(lightColor == 0)
    {
      lightColor ++;
    }
  }

  void cycleBackSheetColor(int amount)
  {
    backSheetColor += amount % Colors.size();
  }

  void moveLightX(float amount)
  {
    lightOffset.x += amount/10;
    
    updateTransforms();
  }

  void moveLightZ(float amount)
  {
    lightOffset.y += amount/10;
    
    updateTransforms();
  }

  void moveLightY(float amount)
  {
    lightOffset.z += amount/10;
    
    updateTransforms();
  }

  void upload()
  {
    bottomSheet->upload();
    for (auto sheet : backSheets) sheet->upload();
    glassSheet->upload();
    light->upload();
  }

  void draw(GLuint programID)
  {
    GLuint modelLoc = glGetUniformLocation(programID, "Model");
    glUniform3f(glGetUniformLocation(programID, "glassColor"), Colors[glassColor].r , Colors[glassColor].g , Colors[glassColor].b);
    glUniform1f(glGetUniformLocation(programID, "glassAlpha"), glassAlpha);
    glUniform3f(glGetUniformLocation(programID, "lightColor"), Colors[lightColor].r , Colors[lightColor].g , Colors[lightColor].b);

    glm::mat4 lightTransform = light->getTransformationMatrix();
    glm::vec3 lightWorldPos = glm::vec3(lightTransform[3][0] - lightOffset.x , lightTransform[3][1] + lightOffset.z , lightTransform[3][2] + lightOffset.y);


    glUniform3f(glGetUniformLocation(programID, "lightPos"), 
                lightWorldPos.x, lightWorldPos.y, lightWorldPos.z);

    glm::mat4 rotationMatrix = glm::mat4(
      glm::vec4(right, 0.0f),
      glm::vec4(up, 0.0f),
      glm::vec4(forward, 0.0f),
      glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
  
    glm::vec3 worldNormal = glm::vec3(rotationMatrix * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
    glUniform3f(glGetUniformLocation(programID, "Normal"), worldNormal.x, worldNormal.y, worldNormal.z);
    glUniform3f(glGetUniformLocation(programID, "forward"), forward.x, forward.y, forward.z);

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &bottomSheet->getTransformationMatrix()[0][0]);
    
    glUniform1i(glGetUniformLocation(programID, "isBottom"), GL_TRUE);
    bottomSheet->draw();   
    glUniform1i(glGetUniformLocation(programID, "isBottom"), GL_FALSE);

    glUniform1i(glGetUniformLocation(programID, "isLight"), GL_TRUE);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &light->getTransformationMatrix()[0][0]);
    light->draw();
    glUniform1i(glGetUniformLocation(programID, "isLight"), GL_FALSE);

    for (auto sheet : backSheets)
    {
      glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &sheet->getTransformationMatrix()[0][0]);
      sheet->draw();
    }


    glUniform1i(glGetUniformLocation(programID, "isGlass"), GL_TRUE);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &glassSheet->getTransformationMatrix()[0][0]);
    glassSheet->draw();
    glUniform1i(glGetUniformLocation(programID, "isGlass"), GL_FALSE);


  }

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
    lightOffset = glm::vec3(0.0f , 0.0f , 0.0f);
    position.x = 0.0f;
    position.y = 0.0f;
    position.z = 0.0f;
    updateTransforms();
  }
};

void handleKeyPresses(GLFWwindow *window, Scene &scene)
{
  
  const float rotationAmount = 2.0f;
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
  {
    scene.pitch(-rotationAmount);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
  {
    scene.pitch(rotationAmount);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
  {
    scene.yaw(rotationAmount);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
  {
    scene.yaw(-rotationAmount);
  }
  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) 
  {
    scene.roll(-rotationAmount);
  }
  if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
  {
    scene.roll(rotationAmount);
  }
  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
  {
    scene.moveLightY(rotationAmount);
  }
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
  {
    scene.moveLightY(-rotationAmount);
  }
  if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
  {
    scene.moveLightX(rotationAmount);
  }
  if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
  {
    scene.moveLightX(-rotationAmount);
  }
  if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
  {
    scene.moveLightZ(-rotationAmount);
  }
  if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
  {
    scene.moveLightZ(rotationAmount);
  }
  if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
  {
    scene.resetOrientation();
  }

  static auto lastSpacePressTime = std::chrono::steady_clock::now();

  auto now = std::chrono::steady_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastSpacePressTime);

  if (duration.count() < 100)
    return;

  lastSpacePressTime = now;
  if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
  {
    wireframeMode = !wireframeMode;
  }
  if(glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
  {
    scene.cycleLightColor(1);
  }
  if(glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
  {
    scene.cycleLightColor(-1);
  }
  if(glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
  {
    scene.cycleGlassColor(1);
  }
  if(glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
  {
    scene.cycleGlassColor(-1);
  }
  if(glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
  {
    scene.cycleGlassAlpha(0.1f);
  }
  if(glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
  {
    scene.cycleGlassAlpha(-0.1f);
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

  glClearColor(0.2f , 0.2f , 0.2f , 1.0f);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


  GLuint programID = LoadShaders("vertex.glsl", "fragment.glsl");

  GLuint MatrixID = glGetUniformLocation(programID, "MVP");

  glm::mat4 Projection = Math::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);

  glm::mat4 View = Math::lookAt(
      glm::vec3(0, 15, 15),
      glm::vec3(0, 0, 0),
      glm::vec3(0, 1, 0));

  glm::mat4 MVP = Projection * View;

  Scene scene(1);
  scene.upload();

  double lastTime;
  lastTime = glfwGetTime();
  do
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(programID);
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

    scene.draw(programID);

    glfwSwapBuffers(window);
    glfwPollEvents();
    handleKeyPresses(window, scene);

  } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && !glfwWindowShouldClose(window));

  glDeleteProgram(programID);
  glfwTerminate();
  return 0;
}