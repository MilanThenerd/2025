#ifndef MATH_H
#define MATH_H

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


#endif