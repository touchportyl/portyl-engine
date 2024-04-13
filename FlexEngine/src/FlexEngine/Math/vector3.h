#pragma once

#include "vector2.h"

namespace FlexEngine
{

  struct Vector3
  {
    union
    {
      float x, y, z;
      float r, g, b;
      float first, second, third;
    };

#pragma region Constructors

    Vector3(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f);
    Vector3(const Vector3& other);
    Vector3(const Vector2& xy, float _z = 0.0f);
    Vector3(float _x, const Vector2& yz);

#pragma endregion

#pragma region Getters

    // Getters for the different combinations of the vector

    Vector2 XY() const;
    Vector2 YX() const;
    Vector2 XZ() const;
    Vector2 ZX() const;
    Vector2 YZ() const;
    Vector2 ZY() const;

    Vector3 XYZ() const;
    Vector3 XZY() const;
    Vector3 YXZ() const;
    Vector3 YZX() const;
    Vector3 ZXY() const;
    Vector3 ZYX() const;

#pragma endregion

#pragma region Operator Overloading

    // - =
    // += -= *= /=
    // == !=
    // + - * /

    Vector3 operator-() const;

    Vector3& operator=(const Vector3& other);

    Vector3& operator+=(const Vector3& other);
    Vector3& operator+=(const float value);

    Vector3& operator-=(const Vector3& other);
    Vector3& operator-=(const float value);

    Vector3& operator*=(const Vector3& other);
    Vector3& operator*=(const float value);

    Vector3& operator/=(const Vector3& other);
    Vector3& operator/=(const float value);

    bool operator==(const Vector3& other) const;
    bool operator!=(const Vector3& other) const;

    float Magnitude() const;
    float Length() const;
    float LengthSqr() const;
    Vector3 Normalize() const;

#pragma endregion

  };

#pragma region Vector3 Helper Fns

  Vector3 operator+(const Vector3& point_a, const Vector3& point_b);
  Vector3 operator+(const float value, const Vector3& point);
  Vector3 operator+(const Vector3& point, const float value);

  Vector3 operator-(const Vector3& point_a, const Vector3& point_b);
  Vector3 operator-(const float value, const Vector3& point);
  Vector3 operator-(const Vector3& point, const float value);

  //Vector3 operator*(const Vector3& point_a, const Vector3& point_b);
  float Dot(const Vector3& point_a, const Vector3& point_b);
  Vector3 operator*(const float value, const Vector3& point);
  Vector3 operator*(const Vector3& point, const float value);

  //Vector3 operator/(const Vector3& point, const float value);
  Vector3 operator/(const float value, const Vector3& point);
  Vector3 operator/(const Vector3& point, const float value);

  Vector3 Cross(const Vector3& a, const Vector3& b);

  std::istream& operator>>(std::istream& is, Vector3& point);
  std::ostream& operator<<(std::ostream& os, const Vector3& point);

#pragma endregion

}