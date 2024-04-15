#include "vector2.h"

namespace FlexEngine
{

#pragma region Constructors

  Vector2::Vector2(float _x, float _y)
  {
    x = _x;
    y = _y;
  }

  Vector2::Vector2(const Vector2& other)
    {
      x = other.x;
      y = other.y;
    }

#pragma endregion

#pragma region Getters

  // Getters for the different combinations of the vector

  Vector2 Vector2::XY() const { return { x, y }; }
  Vector2 Vector2::YX() const { return { y, x }; }

#pragma endregion

#pragma region Operator Overloading

  // - =
  // += -= *= /=
  // == !=
  // + - * /

  Vector2 Vector2::operator-() const
  {
    return { -x, -y };
  }

  Vector2& Vector2::operator=(const Vector2& other)
  {
    this->x = other.x;
    this->y = other.y;
    return *this;
  }

  Vector2& Vector2::operator+=(const Vector2& other)
  {
    this->x += other.x;
    this->y += other.y;
    return *this;
  }

  Vector2& Vector2::operator+=(const float value)
  {
    this->x += value;
    this->y += value;
    return *this;
  }

  Vector2& Vector2::operator-=(const Vector2& other)
  {
    this->x -= other.x;
    this->y -= other.y;
    return *this;
  }

  Vector2& Vector2::operator-=(const float value)
  {
    this->x -= value;
    this->y -= value;
    return *this;
  }

  Vector2& Vector2::operator*=(const Vector2& other)
  {
    this->x *= other.x;
    this->y *= other.y;
    return *this;
  }

  Vector2& Vector2::operator*=(const float value)
  {
    this->x *= value;
    this->y *= value;
    return *this;
  }

  Vector2& Vector2::operator/=(const Vector2& other)
  {
    if (other.x == 0 || other.y == 0) return *this;
    this->x /= other.x;
    this->y /= other.y;
    return *this;
  }

  Vector2& Vector2::operator/=(const float& value)
  {
    if (value == 0) return *this;
    this->x /= value;
    this->y /= value;
    return *this;
  }

  bool Vector2::operator==(const Vector2& other) const
  {
    return x - other.x < EPSILON_f && y - other.y < EPSILON_f;
  }

  bool Vector2::operator!=(const Vector2& other) const
  {
    return !(x - other.x < EPSILON_f && y - other.y < EPSILON_f);
  }

  Vector2 Vector2::RotateRad(const float radians) const
  {
    const float cos = std::cos(radians);
    const float sin = std::sin(radians);
    return { x * cos - y * sin, x * sin + y * cos };
  }

  Vector2 Vector2::RotateDeg(const float degrees) const
  {
    const float radians = degrees * PI_f / 180.0f;
    return RotateRad(radians);
  }

  float Vector2::Magnitude() const
  {
    return std::sqrt(x * x + y * y);
  }

  float Vector2::Length() const
  {
    return Magnitude();
  }

  float Vector2::LengthSqr() const
  {
    return x * x + y * y;
  }

  Vector2 Vector2::Normalize() const
    {
      const float mag = Magnitude();
      if (mag == 0) return { 0, 0 };
      return { x / mag, y / mag };
    }

#pragma endregion

#pragma region Vector2 Helper Fns

  Vector2 operator+(const Vector2& point_a, const Vector2& point_b)
  {
    return { point_a.x + point_b.x, point_a.y + point_b.y };
  }

  Vector2 operator+(const float value, const Vector2& point)
  {
    return { point.x + value, point.y + value };
  }

  Vector2 operator+(const Vector2& point, const float value)
  {
    return { point.x + value, point.y + value };
  }

  Vector2 operator-(const Vector2& point_a, const Vector2& point_b)
  {
    return { point_a.x - point_b.x, point_a.y - point_b.y };
  }

  Vector2 operator-(const float value, const Vector2& point)
  {
    return { point.x - value, point.y - value };
  }

  Vector2 operator-(const Vector2& point, const float value)
  {
    return { point.x - value, point.y - value };
  }

  //Vector2 operator*(const Vector2& point_a, const Vector2& point_b)
  //{
  //  return { point_a.x * point_b.x, point_a.y * point_b.y };
  //}

  // Dot product of two vectors
  float Dot(const Vector2& a, const Vector2& b)
  {
    return a.x * b.x + a.y * b.y;
  }

  Vector2 operator*(const Vector2& point, const float value)
  {
    return { point.x * value, point.y * value };
  }

  Vector2 operator*(const float value, const Vector2& point)
  {
    return { point.x * value, point.y * value };
  }

  //Vector2 operator/(const Vector2& point_a, const Vector2& point_b)
  //{
  //  return { point_a.x / point_b.x, point_a.y / point_b.y };
  //}

  Vector2 operator/(const float value, const Vector2& point)
  {
    if (point.x == 0 || point.y == 0) return { 0, 0 };
    return { value / point.x, value / point.y };
  }

  Vector2 operator/(const Vector2& point, const float value)
  {
    if (value == 0) return { 0, 0 };
    return { point.x / value, point.y / value };
  }

  // Cross product of two vectors
  // Two crossed vectors return a scalar
  float Cross(const Vector2& a, const Vector2& b)
  {
    return a.x * b.y - a.y * b.x;
  }

  // Cross product of a vector with a scalar
  // with a vector v and scalar a, both returning a vector
  Vector2 Cross(const Vector2& v, float a)
  {
    return { a * v.y, -a * v.x };
  }

  Vector2 Cross(float a, const Vector2& v)
  {
    return { -a * v.y, a * v.x };
  }

  float Distance(const Vector2& a, const Vector2& b)
  {
    return (a - b).Magnitude();
  }

  std::istream& operator>>(std::istream& is, Vector2& point)
  {
    return is >> point.x >> point.y;
  }

  std::ostream& operator<<(std::ostream& os, const Vector2& point)
  {
    return os << "(" << point.x << ", " << point.y << ")";
  }

#pragma endregion

}