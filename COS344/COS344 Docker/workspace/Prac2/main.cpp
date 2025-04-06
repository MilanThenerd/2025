#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
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

inline GLFWwindow *setUp(int width = 1000, int height = 1000, float red = 0.0f, float green = 0.0f, float blue = 0.0f, float alpha = 1.0f)
{
  startUpGLFW();
  glfwWindowHint(GLFW_SAMPLES, 4);               // 4x antialiasing
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // To make MacOS happy; should not be needed
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL
  GLFWwindow *window;                                            // (In the accompanying source code, this variable is global for simplicity)
  window = glfwCreateWindow(width, height, "IT Kiosk Floor Plan", NULL, NULL);
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

void getColor(float red, float green, float blue, int vertices, GLfloat *result)
{
  for (int i = 0; i < vertices; i++)
  {
    result[3 * i] = red;
    result[3 * i + 1] = green;
    result[3 * i + 2] = blue;
  }
}

void createCircle(int segments = 30, float radius = 0.1f, GLfloat *vertices = nullptr)
{
  int vertexCount = segments + 1;
  if (vertices == nullptr)
  {
    vertices = new GLfloat[3 * vertexCount];
  }
  vertices[0] = 0.0f;
  vertices[1] = 0.0f;
  vertices[2] = 0.0f;

  for (int i = 0; i <= segments; ++i)
  {
    float angle = 2.0f * M_PI * i / segments;
    float x = radius * cosf(angle);
    float y = radius * sinf(angle);

    vertices[3 * (i + 1)] = x;
    vertices[3 * (i + 1) + 1] = y;
    vertices[3 * (i + 1) + 2] = 0.0f;
  }
}

bool wireframeMode = false;

class Shape
{
public:
  GLfloat vertices[1000] = {0};
  GLfloat transformedVertices[1000] = {0};
  GLfloat colors[1000] = {0};
  GLfloat x = 0.0f, y = 0.0f;
  float rotationAngle = 0.0f;
  float scaleX = 1.0f, scaleY = 1.0f;
  bool isSelected = false;
  GLuint VAO = 0, VBO = 0, CBO = 0;
  int vertexCount = 0;

  Shape() {}

  Shape(GLfloat *v, GLfloat *c, int vCount) : x(0.0f), y(0.0f), vertexCount(vCount)
  {
    for (int i = 0; i < vCount * 3; i++)
    {
      vertices[i] = v[i];
      transformedVertices[i] = v[i];
      colors[i] = c[i];
    }
  }

  virtual void draw() = 0;

  void move(GLfloat dx, GLfloat dy)
  {
    x += dx;
    y += dy;
    applyTransformations();
  }

  void rotate(float angle)
  {
    rotationAngle += angle;
    applyTransformations();
  }

  void scale(float sX, float sY)
  {
    scaleX *= sX;
    scaleY *= sY;
    applyTransformations();
  }

  void setColor(GLfloat *color)
  {
    for (int i = 0; i < vertexCount * 3; i++)
    {
      colors[i] = color[i];
    }
  }

  void applyTransformations()
  {
    double radians = rotationAngle * (M_PI / 180.0);
    double cosTheta = cos(radians);
    double sinTheta = sin(radians);

    GLfloat centerX = 0.0f, centerY = 0.0f;

    for (int i = 0; i < vertexCount * 3; i += 3)
    {
      centerX += vertices[i];
      centerY += vertices[i + 1];
    }
    centerX /= vertexCount;
    centerY /= vertexCount;

    for (int i = 0; i < vertexCount * 3; i += 3)
    {
      GLfloat localX = vertices[i] - centerX;
      GLfloat localY = vertices[i + 1] - centerY;

      localX *= scaleX;
      localY *= scaleY;

      GLfloat rotatedX = localX * cosTheta - localY * sinTheta;
      GLfloat rotatedY = localX * sinTheta + localY * cosTheta;

      transformedVertices[i] = rotatedX + centerX + x;
      transformedVertices[i + 1] = rotatedY + centerY + y;
      transformedVertices[i + 2] = vertices[i + 2];
    }
    upload();
  }

  void upload()
  {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &CBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(GLfloat), transformedVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

    glBindBuffer(GL_ARRAY_BUFFER, CBO);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

    glBindVertexArray(0);
  }
};

class Triangle : public Shape
{
public:
  Triangle() {}
  Triangle(GLfloat *v, GLfloat *c, int vCount) : Shape(v, c, vCount) {}
  void draw() override
  {
    glBindVertexArray(VAO);

    if (isSelected)
    {
      GLfloat pastelColor[1000];
      getColor(1.0f, 0.9f, 0.7f, vertexCount, pastelColor);
      glBindBuffer(GL_ARRAY_BUFFER, CBO);
      glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(GLfloat), pastelColor, GL_STATIC_DRAW);
    }
    else
    {
      glBindBuffer(GL_ARRAY_BUFFER, CBO);
      glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
    }
    if (wireframeMode)
    {
      glDrawArrays(GL_LINE_LOOP, 0, vertexCount);
    }
    else
    {
      glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    }
  }
};

class Square : public Shape
{
public:
  Square() {}
  Square(GLfloat *v, GLfloat *c, int vCount) : Shape(v, c, vCount) {}
  void draw() override
  {
    glBindVertexArray(VAO);
    if (isSelected)
    {
      GLfloat pastelColor[1000];
      getColor(1.0f, 0.9f, 0.7f, vertexCount, pastelColor);
      glBindBuffer(GL_ARRAY_BUFFER, CBO);
      glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(GLfloat), pastelColor, GL_STATIC_DRAW);
    }
    else
    {
      glBindBuffer(GL_ARRAY_BUFFER, CBO);
      glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
    }
    if (wireframeMode)
    {
      glDrawArrays(GL_LINE_LOOP, 0, 3);
      glDrawArrays(GL_LINE_LOOP, 1, 3);
    }
    else
    {
      glDrawArrays(GL_TRIANGLE_STRIP, 0, vertexCount);
    }
  }
};

class Circle : public Shape
{
public:
  Circle() {}
  Circle(GLfloat *v, GLfloat *c, int vCount) : Shape(v, c, vCount) {}
  void draw() override
  {
    glBindVertexArray(VAO);
    if (isSelected)
    {
      GLfloat pastelColor[1000];
      getColor(1.0f, 0.9f, 0.7f, vertexCount, pastelColor);
      glBindBuffer(GL_ARRAY_BUFFER, CBO);
      glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(GLfloat), pastelColor, GL_STATIC_DRAW);
    }
    else
    {
      glBindBuffer(GL_ARRAY_BUFFER, CBO);
      glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
    }
    if (wireframeMode)
    {
      for (int i = 0; i <= vertexCount; i++)
      {
        glDrawArrays(GL_LINE_LOOP, 0, i);
      }
    }
    else
    {
      glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);
    }
  }
};

class EntryDoor : public Square
{
public:
  EntryDoor() {}
  EntryDoor(GLfloat *v, GLfloat *c, int vCount) : Square(v, c, vCount) {}
};

class Counter : public Square
{
public:
  Counter() {}
  Counter(GLfloat *v, GLfloat *c, int vCount) : Square(v, c, vCount) {}
};

class Bench : public Square
{
public:
  Bench() {}
  Bench(GLfloat *v, GLfloat *c, int vCount) : Square(v, c, vCount) {}
};

class Dustbin : public Triangle
{
public:
  Dustbin() {}
  Dustbin(GLfloat *v, GLfloat *c, int vCount) : Triangle(v, c, vCount) {}
};

const int maxDoors = 10;
const int maxPlants = 10;
const int maxPosts = 10;
const int maxTables = 10;
const int maxSeating = 10;
const int maxEntryDoors = 2;
const int maxCounters = 1;
const int maxBenches = 3;
const int maxDustbins = 2;

Square doors[maxDoors];
Triangle plants[maxPlants];
Square posts[maxPosts];
Square tables[maxTables];
Circle seating[maxSeating];
EntryDoor entryDoors[maxEntryDoors];
Counter counters[maxCounters];
Bench benches[maxBenches];
Dustbin dustbins[maxDustbins];

int doorCount = 0, plantCount = 0, postCount = 0, tableCount = 0, seatingCount = 0;
int entryDoorCount = 0, counterCount = 0, benchCount = 0, dustbinCount = 0;

bool isDragging = false;
double lastX, lastY;

void handleMovement(Shape *shape, GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
  {
    shape->move(0.02f, 0.0f);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
  {
    shape->move(-0.02f, 0.0f);
  }
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
  {
    shape->move(0.0f, 0.02f);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
  {
    shape->move(0.0f, -0.02f);
  }
  if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
  {
    shape->rotate(-3);
  }
  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
  {
    shape->rotate(3);
  }
  if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS)
  {
    shape->scale(1.1f, 1.1f);
  }
  if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS)
  {
    shape->scale(0.9f, 0.9f);
  }
}

void saveLayout(const std::string &filename)
{
  ofstream file(filename);
  if (!file)
  {
    cerr << "Error: Could not open file for saving layout.\n";
    return;
  }

  for (int i = 0; i < doorCount; i++)
  {
    file << "Doors " << doors[i].x << " " << doors[i].y << " "
         << doors[i].rotationAngle << " " << doors[i].scaleX << " " << doors[i].scaleY << " " << doors[i].vertexCount <<  "\n";
  }
  for (int i = 0; i < plantCount; i++)
  {
    file << "Plants " << plants[i].x << " " << plants[i].y << " "
         << plants[i].rotationAngle << " " << plants[i].scaleX << " " << plants[i].scaleY << " " << plants[i].vertexCount << "\n";
  }
  for (int i = 0; i < tableCount; i++)
  {
    file << "Tables " << tables[i].x << " " << tables[i].y << " "
         << tables[i].rotationAngle << " " << tables[i].scaleX << " " << tables[i].scaleY << " " << tables[i].vertexCount <<"\n";
  }
  for (int i = 0; i < seatingCount; i++)
  {
    file << "Seating " << seating[i].x << " " << seating[i].y << " "
         << seating[i].rotationAngle << " " << seating[i].scaleX << " " << seating[i].scaleY << " " << seating[i].vertexCount << "\n";
  }
  for (int i = 0; i < postCount; i++)
  {
    file << "Posts " << posts[i].x << " " << posts[i].y << " "
         << posts[i].rotationAngle << " " << posts[i].scaleX << " " << posts[i].scaleY << " " << posts[i].vertexCount << "\n";
  }
  for (int i = 0; i < entryDoorCount; i++)
  {
    file << "EntryDoors " << entryDoors[i].x << " " << entryDoors[i].y << " "
         << entryDoors[i].rotationAngle << " " << entryDoors[i].scaleX << " " << entryDoors[i].scaleY << " " << entryDoors[i].vertexCount << "\n";
  }
  for (int i = 0; i < counterCount; i++)
  {
    file << "Counters " << counters[i].x << " " << counters[i].y << " "
         << counters[i].rotationAngle << " " << counters[i].scaleX << " " << counters[i].scaleY << " " << counters[i].vertexCount << "\n";
  }
  for (int i = 0; i < benchCount; i++)
  {
    file << "Benches " << benches[i].x << " " << benches[i].y << " "
         << benches[i].rotationAngle << " " << benches[i].scaleX << " " << benches[i].scaleY << " " << benches[i].vertexCount << "\n";
  }
  for (int i = 0; i < dustbinCount; i++)
  {
    file << "Dustbins " << dustbins[i].x << " " << dustbins[i].y << " "
         << dustbins[i].rotationAngle << " " << dustbins[i].scaleX << " " << dustbins[i].scaleY << " " << dustbins[i].vertexCount << "\n";
  }

  file.close();
}

void loadLayout(const std::string &filename)
{
  ifstream file(filename);
  if (!file)
  {
    cerr << "Error: Could not open file for loading layout.\n";
    return;
  }

  string shapeType;
  float x, y, rotation, scaleX, scaleY ,vertexCount;

  doorCount = 0;
  plantCount = 0;
  tableCount = 0;
  seatingCount = 0;
  postCount = 0;
  entryDoorCount = 0;
  counterCount = 0;
  benchCount = 0;
  dustbinCount = 0;

  while (file >> shapeType >> x >> y >> rotation >> scaleX >> scaleY >> vertexCount)
  {
    if (shapeType == "Doors" && doorCount < maxDoors)
    {
      GLfloat doorVertices[] = {-0.1f, -0.8f, 0.0f, 0.1f, -0.8f, 0.0f, -0.1f, -0.75f, 0.0f, 0.1f, -0.75f, 0.0f};
      GLfloat doorColors[12];
      getColor(0.5f, 0.5f, 0.5f, 4, doorColors);
      doors[doorCount] = Square(doorVertices, doorColors, 4);
      doors[doorCount].move(x, y);
      doors[doorCount].rotate(rotation);
      doors[doorCount].scale(scaleX, scaleY);
      doors[doorCount].upload();
      doorCount++;
    }
    else if (shapeType == "Posts" && postCount < maxPosts)
    {
      GLfloat postVertices[] = {-0.05f, -0.05f, 0.0f, 0.05f, -0.05f, 0.0f, -0.05f, 0.05f, 0.0f, 0.05f, 0.05f, 0.0f};
      GLfloat postColors[12];
      getColor(0.5f, 0.5f, 0.5f, 4, postColors);
      posts[postCount] = Square(postVertices, postColors, 4);
      posts[postCount].move(x, y);
      posts[postCount].rotate(rotation);
      posts[postCount].scale(scaleX, scaleY);
      posts[postCount].upload();
      postCount++;
    }
    else if (shapeType == "Plants" && plantCount < maxPlants)
    {
      GLfloat plantVertices[] = {-0.05f, -0.05f, 0.0f, 0.05f, -0.05f, 0.0f, 0.0f, 0.05f, 0.0f};
      GLfloat plantColors[9];
      getColor(0.0f, 1.0f, 0.0f, 3, plantColors);
      plants[plantCount] = Triangle(plantVertices, plantColors, 3);
      plants[plantCount].move(x, y);
      plants[plantCount].rotate(rotation);
      plants[plantCount].scale(scaleX, scaleY);
      plants[plantCount].upload();
      plantCount++;
    }
    else if (shapeType == "Seating" && seatingCount < maxSeating)
    {
      int count = 3 * vertexCount;
      GLfloat seatingVertices[count];
      createCircle(vertexCount - 2, 0.1f, seatingVertices);
      GLfloat seatingColors[count];
      getColor(0.5f, 0.0f, 0.5f, vertexCount, seatingColors);
      seating[seatingCount] = Circle(seatingVertices, seatingColors, vertexCount);
      seating[seatingCount].move(x, y);
      seating[seatingCount].rotate(rotation);
      seating[seatingCount].scale(scaleX, scaleY);
      seating[seatingCount].upload();
      seatingCount++;
    }
    else if (shapeType == "Tables" && tableCount < maxTables)
    {
      GLfloat tableVertices[] = {-0.2f, 0.2f, 0.0f, 0.2f, 0.2f, 0.0f, -0.2f, 0.3f, 0.0f, 0.2f, 0.3f, 0.0f};
      GLfloat tableColors[12];
      getColor(0.0f, 0.0f, 1.0f, 4, tableColors);
      tables[tableCount] = Square(tableVertices, tableColors, 4);
      tables[tableCount].move(x, y);
      tables[tableCount].rotate(rotation);
      tables[tableCount].scale(scaleX, scaleY);
      tables[tableCount].upload();
      tableCount++;
    }
    else if (shapeType == "EntryDoors" && entryDoorCount < maxEntryDoors)
    {
      GLfloat entryDoorVertices[] = {-0.2f, -0.8f, 0.0f, 0.2f, -0.8f, 0.0f, -0.2f, -0.7f, 0.0f, 0.2f, -0.7f, 0.0f};
      GLfloat entryDoorColors[12];
      getColor(0.5f, 0.5f, 0.5f, 4, entryDoorColors);
      entryDoors[entryDoorCount] = EntryDoor(entryDoorVertices, entryDoorColors, 4);
      entryDoors[entryDoorCount].move(x, y);
      entryDoors[entryDoorCount].rotate(rotation);
      entryDoors[entryDoorCount].scale(scaleX, scaleY);
      entryDoors[entryDoorCount].upload();
      entryDoorCount++;
    }
    else if (shapeType == "Counters" && counterCount < maxCounters)
    {
      GLfloat counterVertices[] = {-0.3f, 0.6f, 0.0f, 0.3f, 0.6f, 0.0f, -0.3f, 0.7f, 0.0f, 0.3f, 0.7f, 0.0f};
      GLfloat counterColors[12];
      getColor(0.0f, 0.0f, 1.0f, 4, counterColors);
      counters[counterCount] = Counter(counterVertices, counterColors, 4);
      counters[counterCount].move(x, y);
      counters[counterCount].rotate(rotation);
      counters[counterCount].scale(scaleX, scaleY);
      counters[counterCount].upload();
      counterCount++;
    }
    else if (shapeType == "Benches" && benchCount < maxBenches)
    {
      GLfloat benchVertices[] = {-0.2f, 0.4f, 0.0f, 0.2f, 0.4f, 0.0f, -0.2f, 0.5f, 0.0f, 0.2f, 0.5f, 0.0f};
      GLfloat benchColors[12];
      getColor(0.5f, 0.3f, 0.1f, 4, benchColors);
      benches[benchCount] = Bench(benchVertices, benchColors, 4);
      benches[benchCount].move(x, y);
      benches[benchCount].rotate(rotation);
      benches[benchCount].scale(scaleX, scaleY);
      benches[benchCount].upload();
      benchCount++;
    }
    else if (shapeType == "Dustbins" && dustbinCount < maxDustbins)
    {
      GLfloat dustbinVertices[] = {-0.05f, -0.05f, 0.0f, 0.05f, -0.05f, 0.0f, -0.05f, 0.05f, 0.0f, 0.05f, 0.05f, 0.0f};
      GLfloat dustbinColors[12];
      getColor(0.3f, 0.3f, 0.3f, 4, dustbinColors);
      dustbins[dustbinCount] = Dustbin(dustbinVertices, dustbinColors, 4);
      dustbins[dustbinCount].move(x, y);
      dustbins[dustbinCount].rotate(rotation);
      dustbins[dustbinCount].scale(scaleX, scaleY);
      dustbins[dustbinCount].upload();
      dustbinCount++;
    }
  }

  file.close();
}

void handleKeyPresses(GLFWwindow *window, int &type, int &index,
                      Square doors[], int doorCount, Triangle plants[], int plantCount,
                      Square posts[], int postCount, Square tables[], int tableCount,
                      Circle seating[], int seatingCount, EntryDoor entryDoors[], int entryDoorCount,
                      Counter counters[], int counterCount, Bench benches[], int benchCount,
                      Dustbin dustbins[], int dustbinCount, double &lastKeyPressTime, const double keyPressCooldown)
{
  double currentTime = glfwGetTime();
  if (currentTime - lastKeyPressTime >= keyPressCooldown)
  {
    for (int i = 0; i < doorCount; i++)
      doors[i].isSelected = false;
    for (int i = 0; i < plantCount; i++)
      plants[i].isSelected = false;
    for (int i = 0; i < postCount; i++)
      posts[i].isSelected = false;
    for (int i = 0; i < tableCount; i++)
      tables[i].isSelected = false;
    for (int i = 0; i < seatingCount; i++)
      seating[i].isSelected = false;
    for (int i = 0; i < entryDoorCount; i++)
      entryDoors[i].isSelected = false;
    for (int i = 0; i < counterCount; i++)
      counters[i].isSelected = false;
    for (int i = 0; i < benchCount; i++)
      benches[i].isSelected = false;
    for (int i = 0; i < dustbinCount; i++)
      dustbins[i].isSelected = false;

    if (type == 1 && index < doorCount)
      doors[index].isSelected = true;
    else if (type == 2 && index < plantCount)
      plants[index].isSelected = true;
    else if (type == 3 && index < postCount)
      posts[index].isSelected = true;
    else if (type == 4 && index < tableCount)
      tables[index].isSelected = true;
    else if (type == 5 && index < seatingCount)
      seating[index].isSelected = true;
    else if (type == 6 && index < entryDoorCount)
      entryDoors[index].isSelected = true;
    else if (type == 7 && index < counterCount)
      counters[index].isSelected = true;
    else if (type == 8 && index < benchCount)
      benches[index].isSelected = true;
    else if (type == 9 && index < dustbinCount)
      dustbins[index].isSelected = true;
    if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
    {
      type = 0;
      index = 0;
      lastKeyPressTime = currentTime;
    }
    else if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
      type = 8;
      index = (index + 1) % benchCount;
      benches[index].isSelected = true;
      lastKeyPressTime = currentTime;
    }
    else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
      type = 5;
      index = (index + 1) % seatingCount;
      seating[index].isSelected = true;
      lastKeyPressTime = currentTime;
    }
    else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {
      type = 2;
      index = (index + 1) % plantCount;
      plants[index].isSelected = true;
      lastKeyPressTime = currentTime;
    }
    else if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
    {
      type = 4;
      index = (index + 1) % tableCount;
      tables[index].isSelected = true;
      lastKeyPressTime = currentTime;
    }
    else if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
    {
      type = 1;
      index = (index + 1) % doorCount;
      doors[index].isSelected = true;
      lastKeyPressTime = currentTime;
    }
    else if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
    {
      type = 3;
      index = (index + 1) % postCount;
      posts[index].isSelected = true;
      lastKeyPressTime = currentTime;
    }
    else if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
    {
      type = 6;
      index = (index + 1) % entryDoorCount;
      entryDoors[index].isSelected = true;
      lastKeyPressTime = currentTime;
    }
    else if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
    {
      type = 7;
      index = (index + 1) % counterCount;
      counters[index].isSelected = true;
      lastKeyPressTime = currentTime;
    }
    else if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
    {
      type = 9;
      index = (index + 1) % dustbinCount;
      dustbins[index].isSelected = true;
      lastKeyPressTime = currentTime;
    }
    else if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
    {
      wireframeMode = !wireframeMode;
      lastKeyPressTime = currentTime;
    }
    else if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    {
      saveLayout("layout.txt");
      lastKeyPressTime = currentTime;
    }
    else if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    {
      loadLayout("layout.txt");
      lastKeyPressTime = currentTime;
    }
  }
}
int main()
{
  GLFWwindow *window = setUp(1000, 1000, 0.3f, 0.3f, 0.3f, 1.0f);
  GLuint programID = LoadShaders("vertex.glsl", "fragment.glsl");
  GLuint VertexArrayID;
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);

  GLfloat floorVertices[] = {-0.8f, -0.8f, 0.0f, 0.8f, -0.8f, 0.0f, -0.8f, 0.8f, 0.0f, 0.8f, 0.8f, 0.0f};
  GLfloat floorColors[12];
  getColor(1.0f, 0.5f, 0.0f, 4, floorColors);
  Square floor(floorVertices, floorColors, 4);
  floor.upload();

  GLfloat doorVertices[] = {-0.1f, -0.8f, 0.0f, 0.1f, -0.8f, 0.0f, -0.1f, -0.75f, 0.0f, 0.1f, -0.75f, 0.0f};
  GLfloat doorColors[12];
  getColor(0.5f, 0.5f, 0.5f, 4, doorColors);
  doors[doorCount++] = Square(doorVertices, doorColors, 4);
  doors[doorCount - 1].move(-0.3f, 0.0f);

  GLfloat plantVertices[] = {-0.05f, -0.05f, 0.0f, 0.05f, -0.05f, 0.0f, 0.0f, 0.05f, 0.0f};
  GLfloat plantColors[9];
  getColor(0.0f, 1.0f, 0.0f, 3, plantColors);
  for (int i = 0; i < 6; i++)
  {
    plants[plantCount++] = Triangle(plantVertices, plantColors, 3);
  }

  GLfloat postVertices[] = {-0.05f, -0.05f, 0.0f, 0.05f, -0.05f, 0.0f, -0.05f, 0.05f, 0.0f, 0.05f, 0.05f, 0.0f};
  GLfloat postColors[12];
  getColor(0.5f, 0.5f, 0.5f, 4, postColors);
  posts[postCount++] = Square(postVertices, postColors, 4);
  posts[postCount - 1].move(0.7f, -0.7f);

  GLfloat tableVertices[] = {-0.2f, 0.2f, 0.0f, 0.2f, 0.2f, 0.0f, -0.2f, 0.3f, 0.0f, 0.2f, 0.3f, 0.0f};
  GLfloat tableColors[12];
  getColor(0.0f, 0.0f, 1.0f, 4, tableColors);
  tables[tableCount++] = Square(tableVertices, tableColors, 4);

  GLfloat seatingVertices[3 * (52 + 1)];
  createCircle(50, 0.1f, seatingVertices);
  GLfloat seatingColors[3 * 52];
  getColor(0.5f, 0.0f, 0.5f, 52, seatingColors);
  seating[seatingCount++] = Circle(seatingVertices, seatingColors, 52);

  GLfloat seatingVerticesLow[3 * (12)];
  createCircle(10, 0.1f, seatingVerticesLow);
  GLfloat seatingColorsLow[3 * 12];
  getColor(0.5f, 0.0f, 0.5f, 12, seatingColorsLow);
  seating[seatingCount++] = Circle(seatingVerticesLow, seatingColorsLow, 12);

  GLfloat entryDoorVertices[] = {-0.2f, -0.8f, 0.0f, 0.2f, -0.8f, 0.0f, -0.2f, -0.7f, 0.0f, 0.2f, -0.7f, 0.0f};
  GLfloat entryDoorColors[12];
  getColor(0.5f, 0.5f, 0.5f, 4, entryDoorColors);
  entryDoors[entryDoorCount++] = EntryDoor(entryDoorVertices, entryDoorColors, 4);
  entryDoors[entryDoorCount - 1].move(-0.5f, 0.0f);

  GLfloat counterVertices[] = {-0.3f, 0.6f, 0.0f, 0.3f, 0.6f, 0.0f, -0.3f, 0.7f, 0.0f, 0.3f, 0.7f, 0.0f};
  GLfloat counterColors[12];
  getColor(0.0f, 0.0f, 1.0f, 4, counterColors);
  counters[counterCount++] = Counter(counterVertices, counterColors, 4);

  GLfloat benchVertices[] = {-0.2f, 0.4f, 0.0f, 0.2f, 0.4f, 0.0f, -0.2f, 0.5f, 0.0f, 0.2f, 0.5f, 0.0f};
  GLfloat benchColors[12];
  getColor(0.5f, 0.3f, 0.1f, 4, benchColors);
  for (int i = 0; i < maxBenches; i++)
  {
    benches[benchCount++] = Bench(benchVertices, benchColors, 4);
    benches[benchCount - 1].move(0.0f, -0.2f * i);
  }

  GLfloat dustbinVertices[] = {-0.05f, -0.05f, 0.0f, 0.05f, -0.05f, 0.0f, -0.05f, 0.05f, 0.0f, 0.05f, 0.05f, 0.0f};
  GLfloat dustbinColors[12];
  getColor(0.3f, 0.3f, 0.3f, 4, dustbinColors);
  for (int i = 0; i < maxDustbins; i++)
  {
    dustbins[dustbinCount++] = Dustbin(dustbinVertices, dustbinColors, 4);
    dustbins[dustbinCount - 1].move(0.6f, -0.6f + 0.2f * i);
  }

  int type = 0;
  int index = 0;

  double lastKeyPressTime = 0.0;
  const double keyPressCooldown = 0.5;
  loadLayout("layout.txt");
  do
  {
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(programID);
    floor.draw();

    for (auto &door : doors)
    {
      door.upload();
      door.draw();
    }
    for (auto &plant : plants)
    {
      plant.upload();
      plant.draw();
    }
    for (auto &post : posts)
    {
      post.upload();
      post.draw();
    }
    for (auto &table : tables)
    {
      table.upload();
      table.draw();
    }
    for (auto &seat : seating)
    {
      seat.upload();
      seat.draw();
    }
    for (auto &entryDoor : entryDoors)
    {
      entryDoor.upload();
      entryDoor.draw();
    }
    for (auto &counter : counters)
    {
      counter.upload();
      counter.draw();
    }
    for (auto &bench : benches)
    {
      bench.upload();
      bench.draw();
    }
    for (auto &dustbin : dustbins)
    {
      dustbin.upload();
      dustbin.draw();
    }

    if (type == 1 && index < doorCount)
    {
      handleMovement(&doors[index], window);
    }
    else if (type == 2 && index < plantCount)
    {
      handleMovement(&plants[index], window);
    }
    else if (type == 3 && index < postCount)
    {
      handleMovement(&posts[index], window);
    }
    else if (type == 4 && index < tableCount)
    {
      handleMovement(&tables[index], window);
    }
    else if (type == 5 && index < seatingCount)
    {
      handleMovement(&seating[index], window);
    }
    else if (type == 6 && index < entryDoorCount)
    {
      handleMovement(&entryDoors[index], window);
    }
    else if (type == 7 && index < counterCount)
    {
      handleMovement(&counters[index], window);
    }
    else if (type == 8 && index < benchCount)
    {
      handleMovement(&benches[index], window);
    }
    else if (type == 9 && index < dustbinCount)
    {
      handleMovement(&dustbins[index], window);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
    handleKeyPresses(window, type, index, doors, doorCount, plants, plantCount, posts, postCount, tables, tableCount, seating, seatingCount, entryDoors, entryDoorCount, counters, counterCount, benches, benchCount, dustbins, dustbinCount, lastKeyPressTime, keyPressCooldown);
  } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && !glfwWindowShouldClose(window));
  saveLayout("layout.txt");
  glfwTerminate();
  return 0;
}