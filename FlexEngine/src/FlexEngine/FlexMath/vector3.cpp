#include "vector3.h"

namespace FlexEngine
{

#pragma region Constructors

  Vector3::Vector3(float _x, float _y, float _z)
  {
    x = _x;
    y = _y;
    z = _z;
  }

  Vector3::Vector3(const Vector3& other)
  {
    x = other.x;
    y = other.y;
    z = other.z;
  }

  Vector3::Vector3(const Vector2& xy, float _z)
  {
    x = xy.x;
    y = xy.y;
    z = _z;
  }

  Vector3::Vector3(float _x, const Vector2& yz)
  {
    x = _x;
    y = yz.x;
    z = yz.y;
  }

#pragma endregion

#pragma region Getters

  // Getters for the different combinations of the vector

  Vector2 Vector3::XY() const { return { x, y }; }
  Vector2 Vector3::YX() const { return { y, x }; }
  Vector2 Vector3::XZ() const { return { x, z }; }
  Vector2 Vector3::ZX() const { return { z, x }; }
  Vector2 Vector3::YZ() const { return { y, z }; }
  Vector2 Vector3::ZY() const { return { z, y }; }

  Vector3 Vector3::XYZ() const { return { x, y, z }; }
  Vector3 Vector3::XZY() const { return { x, z, y }; }
  Vector3 Vector3::YXZ() const { return { y, x, z }; }
  Vector3 Vector3::YZX() const { return { y, z, x }; }
  Vector3 Vector3::ZXY() const { return { z, x, y }; }
  Vector3 Vector3::ZYX() const { return { z, y, x }; }

#pragma endregion

#pragma region Operator Overloading

  // - =
  // += -= *= /=
  // == !=
  // + - * /

  Vector3 Vector3::operator-() const
  {
    return { -x, -y, -z };
  }

  Vector3& Vector3::operator=(const Vector3& other)
  {
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;
    return *this;
  }

  Vector3& Vector3::operator+=(const Vector3& other)
  {
    this->x += other.x;
    this->y += other.y;
    this->z += other.z;
    return *this;
  }

  Vector3& Vector3::operator+=(const float value)
  {
    this->x += value;
    this->y += value;
    this->z += value;
    return *this;
  }

  Vector3& Vector3::operator-=(const Vector3& other)
  {
    this->x -= other.x;
    this->y -= other.y;
    this->z -= other.z;
    return *this;
  }

  Vector3& Vector3::operator-=(const float value)
  {
    this->x -= value;
    this->y -= value;
    this->z -= value;
    return *this;
  }

  Vector3& Vector3::operator*=(const Vector3& other)
  {
    this->x *= other.x;
    this->y *= other.y;
    this->z *= other.z;
    return *this;
  }

  Vector3& Vector3::operator*=(const float value)
  {
    this->x *= value;
    this->y *= value;
    this->z *= value;
    return *this;
  }

  Vector3& Vector3::operator/=(const Vector3& other)
  {
    if (other.x == 0 || other.y == 0 || other.z == 0) return *this;
    this->x /= other.x;
    this->y /= other.y;
    this->z /= other.z;
    return *this;
  }

  Vector3& Vector3::operator/=(const float value)
  {
    if (value == 0) return *this;
    this->x /= value;
    this->y /= value;
    this->z /= value;
    return *this;
  }

  bool Vector3::operator==(const Vector3& other) const
  {
    return x - other.x < EPSILON_f && y - other.y < EPSILON_f && z - other.z < EPSILON_f;
  }

  bool Vector3::operator!=(const Vector3& other) const
  {
    return !(x - other.x < EPSILON_f && y - other.y < EPSILON_f && z - other.z < EPSILON_f);
  }

  float Vector3::Magnitude() const
  {
    return std::sqrt(x * x + y * y + z * z);
  }

  float Vector3::Length() const
  {
    return Magnitude();
  }

  float Vector3::LengthSqr() const
  {
    return x * x + y * y + z * z;
  }

  Vector3 Vector3::Normalize() const
  {
    float length = Magnitude();
    if (length == 0) return { 0, 0, 0 };
    return { x / length, y / length, z / length };
  }

#pragma endregion

#pragma region Vector3 Helper Fns

  Vector3 operator+(const Vector3& point_a, const Vector3& point_b)
  {
    return { point_a.x + point_b.x, point_a.y + point_b.y, point_a.z + point_b.z };
  }

  Vector3 operator+(const float value, const Vector3& point)
  {
    return { point.x + value, point.y + value, point.z + value };
  }

  Vector3 operator+(const Vector3& point, const float value)
  {
    return { point.x + value, point.y + value, point.z + value };
  }

  Vector3 operator-(const Vector3& point_a, const Vector3& point_b)
  {
    return { point_a.x - point_b.x, point_a.y - point_b.y, point_a.z - point_b.z };
  }

  Vector3 operator-(const float value, const Vector3& point)
  {
    return { point.x - value, point.y - value, point.z - value };
  }

  Vector3 operator-(const Vector3& point, const float value)
  {
    return { point.x - value, point.y - value, point.z - value };
  }

  //Vector3 operator*(const Vector3& point_a, const Vector3& point_b)
  //{
  //  return { point_a.x * point_b.x, point_a.y * point_b.y, point_a.z * point_b.z };
  //}

  float Dot(const Vector3& point_a, const Vector3& point_b)
  {
    return point_a.x * point_b.x + point_a.y * point_b.y + point_a.z * point_b.z;
  }

  Vector3 operator*(const float value, const Vector3& point)
  {
    return { point.x * value, point.y * value, point.z * value };
  }

  Vector3 operator*(const Vector3& point, const float value)
  {
    return { point.x * value, point.y * value, point.z * value };
  }

  //Vector3 operator/(const Vector3& point, const float value)
  //{
  //  return { point.x / value, point.y / value, point.z / value };
  //}

  Vector3 operator/(const float value, const Vector3& point)
  {
    if (point.x == 0 || point.y == 0 || point.z == 0) return { 0, 0, 0 };
    return { value / point.x, value / point.y, value / point.z };
  }

  Vector3 operator/(const Vector3& point, const float value)
  {
    if (value == 0) return { 0, 0, 0 };
    return { point.x / value, point.y / value, point.z / value };
  }

  Vector3 Cross(const Vector3& a, const Vector3& b)
  {
    return { a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x };
  }

  std::istream& operator>>(std::istream& is, Vector3& point)
  {
    return is >> point.x >> point.y >> point.z;
  }

  std::ostream& operator<<(std::ostream& os, const Vector3& point)
  {
    return os << "(" << point.x << ", " << point.y << ", " << point.z << ")";
  }

#pragma endregion

}