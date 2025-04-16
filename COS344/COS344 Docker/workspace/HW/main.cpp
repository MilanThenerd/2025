#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <sstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "shader.hpp"

#include "Math.h"

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

std::vector<GLfloat> getColor(float red, float green, float blue, int vertices)
{
  std::vector<GLfloat> result;
  for (int i = 0; i < vertices * 3; i += 3)
  {
    result[i] = red;
    result[i + 1] = green;
    result[i + 2] = blue;
  }
  return result;
}

std::vector<GLfloat> vertices(const std::string &filename)
{
  std::vector<GLfloat> vertexList;
  std::ifstream file(filename);
  if (!file.is_open())
  {
    std::cerr << "Failed to open file: " << filename << std::endl;
    return vertexList;
  }

  std::string line;

  while (std::getline(file, line))
  {
    std::istringstream iss(line);
    GLfloat x, y, z;

    if (iss >> x >> y >> z)
    {
      vertexList.push_back(x);
      vertexList.push_back(y);
      vertexList.push_back(z);
    }
  }

  file.close();
  return vertexList;
}

bool wireframeMode = false;

class Polyhedrons
{
private:
  std::vector<GLfloat> vertices;
  std::vector<GLfloat> colors;
  std::vector<GLfloat> transformedVertices;
  GLuint VAO = 0, VBO = 0, CBO = 0;
  glm::vec3 position = {0.0f, 0.0f, 0.0f};
  float scale[3] = {1.0f, 1.0f, 1.0f};
  glm::vec3 rotation = {0.0f, 0.0f, 0.0f};
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
    const float degToRad = static_cast<float>(M_PI / 180.0);
    glm::mat4 transform = getTransformationMatrix();
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
      glBufferSubData(GL_ARRAY_BUFFER, 0, vertexCount * 3 * sizeof(GLfloat), transformedVertices.data());
    }
  }

public:
  Polyhedrons(std::vector<GLfloat> v, std::vector<GLfloat> c)
      : vertices(v), colors(c)
  {
    vertexCount = v.size() / 3;

    if (vertexCount <= 0)
    {
      throw std::runtime_error("Vertex count must be positive");
    }
    for (int i = 0; i < vertexCount * 3; i++)
    {
      transformedVertices.push_back(vertices[i]);
    }
  }

  ~Polyhedrons()
  {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &CBO);
    transformedVertices.clear();
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
    glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(GLfloat), transformedVertices.data(), GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, CBO);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(GLfloat), colors.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindVertexArray(0);
  }

  glm::mat4 getTransformationMatrix() const
  {
    glm::mat4 model = glm::mat4(1.0f);
    model = Math::translate(model, glm::vec3(position.x, position.y, position.z));

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
      glBufferSubData(GL_ARRAY_BUFFER, 0, vertexCount * 3 * sizeof(GLfloat), transformedVertices.data());
    }

    applyTransform();
  }

  void setPosition(float x, float y, float z)
  {
    position.x = x;
    position.y = y;
    position.z = z;
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
    position.x += x;
    position.y += y;
    position.z += z;
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

  glm::vec3 getPosition() const
  {
    return position;
  }

  void getScale(float &x, float &y, float &z) const
  {
    x = scale[0];
    y = scale[1];
    z = scale[2];
  }

  glm::vec3 getRotation() const
  {
    return rotation;
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
      glBufferSubData(GL_ARRAY_BUFFER, 0, vertexCount * 3 * sizeof(GLfloat), transformedVertices.data());
    }
  }

  void printVertices()
  {
    for (int i = 0; i < vertexCount * 3; i += 3)
    {
      std::cout << vertices[i] << " " << vertices[i + 1] << " " << vertices[i + 2] << "\n";
    }
  }
};

class Drone
{
private:
  Polyhedrons *body;

  glm::vec3 position = glm::vec3(0.0f);
  glm::vec3 rotation = glm::vec3(0.0f);
  glm::vec3 forward = glm::vec3(0.0f, 0.0f, -1.0f);
  glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);

  glm::vec3 bodyOffset = glm::vec3(0.0f, 0.0f, 0.0f);

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
    static glm::vec3 lastForward = forward;
    static glm::vec3 lastUp = up;
    
    if (glm::length(forward - lastForward) > 0.001f || 
        glm::length(up - lastUp) > 0.001f) {
        right = glm::normalize(glm::cross(up, forward));
        up = glm::normalize(glm::cross(forward, right));
        forward = glm::normalize(forward);
        
        lastForward = forward;
        lastUp = up;
    }
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

    glm::mat4 transform = glm::mat4(1.0f);

    transform = Math::translate(transform, position);

    glm::mat4 orientation(
        glm::vec4(right, 0.0f),
        glm::vec4(up, 0.0f),
        glm::vec4(forward, 0.0f),
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

    transform = transform * orientation;

    return transform;
  }

  void updateTransforms() 
  {
    static glm::mat4 lastTransform;
    glm::mat4 transform = buildTransformMatrix();
    
    if (transform != lastTransform) {
        glm::mat4 bodyTransform = transform;
        bodyTransform = Math::translate(bodyTransform, bodyOffset);
        bodyTransform = Math::rotate(bodyTransform, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        bodyTransform = Math::rotate(bodyTransform, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        body->setTransformation(bodyTransform);
        lastTransform = transform;
    }
}

public:
  Drone(std::vector<GLfloat> bodyVertices, std::vector<GLfloat> bodyColor)
  {
    body = new Polyhedrons(bodyVertices, bodyColor);
    bodyOffset = glm::vec3(-4.0f, 0, 0);
    updateTransforms();
  }

  void upload()
  {
    body->upload();
  }

  void draw()
  {
    body->draw();
  }

  void faceMovementDirection(const glm::vec3 &moveDirection, float deltaTime, float rotationSpeed = 5.0f)
  {
    if (glm::length(moveDirection) > 0.01f)
    {
      glm::vec3 newForward = glm::normalize(moveDirection);

      glm::vec3 horizontalForward = glm::normalize(glm::vec3(newForward.x, 0.0f, newForward.z));
      if (glm::length(horizontalForward) > 0.01f)
      {
        forward = glm::normalize(glm::mix(forward, horizontalForward,
                                          glm::clamp(rotationSpeed * deltaTime, 0.0f, 1.0f)));
        rebuildOrientation();
      }
    }
    updateTransforms();
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
    position = glm::vec3(0.0f);
    updateTransforms();
  }

  void move(glm::vec3 movement)
  {
    this->position += movement;
    updateTransforms();
  }

  glm::vec3 getPosition() const
  {
    return position;
  }

  glm::vec3 getForward() const
  {
    return forward;
  }

  glm::vec3 getUp() const
  {
    return up;
  }

  glm::vec3 getRight() const
  {
    return right;
  }
};

Drone *drone;

float cameraDistance = 10.0f;
float cameraYaw = -90.0f;
float cameraPitch = -30.0f;

float yaw = -90.0f;
float pitch = 0.0f;
float lastX = 1000, lastY = 1000;
bool firstMouse = true;
float sensitivity = 1.0f;

void mouseCallBack(GLFWwindow *window, double xpos, double ypos)
{
  if (firstMouse)
  {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos;

  lastX = xpos;
  lastY = ypos;

  xoffset *= sensitivity;
  yoffset *= sensitivity;

  cameraYaw += xoffset;
  cameraPitch += yoffset;

  if (cameraPitch > 89.0f)
    cameraPitch = 89.0f;
  if (cameraPitch < -89.0f)
    cameraPitch = -89.0f;
}

void scrollCallBack(GLFWwindow *window, double xoffset, double yoffset)
{
  cameraDistance -= yoffset;
  if (cameraDistance < 2.0f)
    cameraDistance = 2.0f;
  if (cameraDistance > 50.0f)
    cameraDistance = 50.0f;
}

double lastInputTime = 0.0;
const double inputInterval = 1.0/30.0;
void handleInput(GLFWwindow *window, float deltaTime)
{
  if (!drone)
  {
    return;
  }
  double currentTime = glfwGetTime();
  if (currentTime - lastInputTime < inputInterval)
  {
      return;
  }
  lastInputTime = currentTime;

  float speed = 2.0f * deltaTime;
  float rotationSpeed = 2.0f * deltaTime;

  glm::vec3 cameraForward(
      cos(glm::radians(cameraPitch)) * cos(glm::radians(cameraYaw)),
      sin(glm::radians(cameraPitch)),
      cos(glm::radians(cameraPitch)) * sin(glm::radians(cameraYaw)));
  cameraForward = glm::normalize(cameraForward);

  glm::vec3 moveDirection(0.0f);

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
  {
    moveDirection += cameraForward * speed;
  }
  else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
  {
    moveDirection -= cameraForward * speed;
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
  {
    moveDirection -= glm::normalize(glm::cross(cameraForward, glm::vec3(0.0f, 1.0f, 0.0f))) * speed;
  }
  else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
  {
    moveDirection += glm::normalize(glm::cross(cameraForward, glm::vec3(0.0f, 1.0f, 0.0f))) * speed;
  }
  else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
  {
    drone->yaw(speed * 30);
  }
  else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
  {
    drone->yaw(-speed * 30);
  }
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
  {
    moveDirection.y += speed;
  }
  else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
  {
    moveDirection.y -= speed;
  }

  if (glm::length(moveDirection) > 0.0f)
  {
    moveDirection = glm::normalize(moveDirection) * speed;
    drone->move(moveDirection);
    drone->faceMovementDirection(-moveDirection, deltaTime);
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

  glfwSetCursorPosCallback(window, mouseCallBack);
  glfwSetScrollCallback(window, scrollCallBack);

  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glEnable(GL_DEPTH_TEST);

  GLuint programID = LoadShaders("vertex.glsl", "fragment.glsl");

  GLuint MatrixID = glGetUniformLocation(programID, "MVP");

  std::vector<GLfloat> floorVerts = vertices("./assets/floorplan.txt");
  std::vector<GLfloat> floorColor = vertices("./assets/floorplan_color.txt");


  Polyhedrons floorplan(floorVerts, floorColor);

  floorplan.upload();

  floorplan.rotate(90.0f, 0.0f, 0.0f);

  std::vector<GLfloat> droneVerts = vertices("./assets/drone_4.txt");
  std::vector<GLfloat> droneColor = vertices("./assets/drone_4_color.txt");
  drone = new Drone(droneVerts, droneColor);
  drone->move(glm::vec3(0.0f , 5.0f , 0.0f));
  drone->upload();

  double lastTime = glfwGetTime();
  int nbFrames = 0;

  glm::mat4 Projection = Math::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);

  do
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(programID);

    float currentTime = glfwGetTime();
    float deltaTime = currentTime - lastTime;

    nbFrames++;
    if (currentTime - lastTime >= 1.0)
    {
      std::cout << "FPS: " << nbFrames << "\n";
      nbFrames = 0;
      lastTime += 1.0;
    }

    handleInput(window, deltaTime);

    glm::mat4 View = Math::lookAt(
        drone->getPosition() - glm::vec3(
                                   cameraDistance * cos(glm::radians(cameraPitch)) * cos(glm::radians(cameraYaw)),
                                   cameraDistance * sin(glm::radians(cameraPitch)),
                                   cameraDistance * cos(glm::radians(cameraPitch)) * sin(glm::radians(cameraYaw))),
        drone->getPosition(),
        glm::vec3(0, 1, 0));
    glm::mat4 MVP = Projection * View;
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

    drone->draw();
    floorplan.draw();

    glfwSwapBuffers(window);
    glfwPollEvents();

  } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && !glfwWindowShouldClose(window));
  glDeleteProgram(programID);
  glfwTerminate();
  return 0;
}