#pragma once

#include "Math/values.h"

#include <cmath> // std::cos, std::sin, std::sqrt
#include <iostream> // std::istream, std::ostream

namespace FlexEngine
{

  struct Vector2
  {
    union
    {
      float x, y;
      float first, second;
    };

#pragma region Constructors

    Vector2(float _x = 0.0f, float _y = 0.0f);
    Vector2(const Vector2& other);

#pragma endregion

#pragma region Getters

    // Getters for the different combinations of the vector

    Vector2 XY() const;
    Vector2 YX() const;

#pragma endregion

#pragma region Operator Overloading

    // - =
    // += -= *= /=
    // == !=
    // + - * /

    Vector2 operator-() const;

    Vector2& operator=(const Vector2& other);

    Vector2& operator+=(const Vector2& other);
    Vector2& operator+=(const float value);

    Vector2& operator-=(const Vector2& other);
    Vector2& operator-=(const float value);

    Vector2& operator*=(const Vector2& other);
    Vector2& operator*=(const float value);

    Vector2& operator/=(const Vector2& other);
    Vector2& operator/=(const float& value);

    bool operator==(const Vector2& other) const;
    bool operator!=(const Vector2& other) const;

    Vector2 RotateRad(const float radians) const;
    Vector2 RotateDeg(const float degrees) const;

    float Magnitude() const;
    float Length() const;
    float LengthSqr() const;
    Vector2 Normalize() const;

#pragma endregion

  };

#pragma region Vector2 Helper Fns

  Vector2 operator+(const Vector2& point_a, const Vector2& point_b);
  Vector2 operator+(const float value, const Vector2& point);
  Vector2 operator+(const Vector2& point, const float value);

  Vector2 operator-(const Vector2& point_a, const Vector2& point_b);
  Vector2 operator-(const float value, const Vector2& point);
  Vector2 operator-(const Vector2& point, const float value);

  //Vector2 operator*(const Vector2& point_a, const Vector2& point_b);

  // Dot product of two vectors
  float Dot(const Vector2& a, const Vector2& b);
  Vector2 operator*(const Vector2& point, const float value);
  Vector2 operator*(const float value, const Vector2& point);

  //Vector2 operator/(const Vector2& point_a, const Vector2& point_b);
  Vector2 operator/(const float value, const Vector2& point);
  Vector2 operator/(const Vector2& point, const float value);

  // Cross product of two vectors
  // Two crossed vectors return a scalar
  float Cross(const Vector2& a, const Vector2& b);
  // Cross product of a vector with a scalar
  // with a vector v and scalar a, both returning a vector
  Vector2 Cross(const Vector2& v, float a);
  // Cross product of a vector with a scalar
  // with a vector v and scalar a, both returning a vector
  Vector2 Cross(float a, const Vector2& v);
  float Distance(const Vector2& a, const Vector2& b);

  std::istream& operator>>(std::istream& is, Vector2& point);
  std::ostream& operator<<(std::ostream& os, const Vector2& point);

#pragma endregion

}