#include "vector3.h"

namespace FlexEngine
{

#pragma region Reflection

  FLX_REFL_REGISTER_START(Vector3)
    FLX_REFL_REGISTER_PROPERTY(x)
    FLX_REFL_REGISTER_PROPERTY(y)
    FLX_REFL_REGISTER_PROPERTY(z)
  FLX_REFL_REGISTER_END;

#pragma endregion
  
#pragma region Standard Functions

  const Vector3 Vector3::Zero     = {  0,  0,  0 };
  const Vector3 Vector3::One      = {  1,  1,  1 };
  const Vector3 Vector3::Up       = {  0,  1,  0 };
  const Vector3 Vector3::Down     = {  0, -1,  0 };
  const Vector3 Vector3::Left     = {  1,  0,  0 };
  const Vector3 Vector3::Right    = { -1,  0,  0 };
  const Vector3 Vector3::Forward  = {  0,  0,  1 };
  const Vector3 Vector3::Back     = {  0,  0, -1 };

  Vector3::operator bool() const { return XYZ() != Zero; }

  Vector3::operator Vector2() const { return XY(); }

  std::string Vector3::ToString() const
  {
    return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
  }
  Vector3::operator std::string() const { return ToString(); }

#ifdef _DEBUG
  void Vector3::Dump() const { Log::Debug(ToString()); }
#endif

#pragma endregion

#pragma region Constructors

  Vector3::Vector3(value_type _x, value_type _y, value_type _z)
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

  Vector3::Vector3(const Vector2& xy, value_type _z)
  {
    x = xy.x;
    y = xy.y;
    z = _z;
  }

  Vector3::Vector3(value_type _x, const Vector2& yz)
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

  Vector3& Vector3::operator+=(const_value_type value)
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

  Vector3& Vector3::operator-=(const_value_type value)
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

  Vector3& Vector3::operator*=(const_value_type value)
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

  Vector3& Vector3::operator/=(const_value_type value)
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

  Vector3::value_type Vector3::Magnitude() const
  {
    return std::sqrt(x * x + y * y + z * z);
  }

  Vector3::value_type Vector3::Length() const
  {
    return Magnitude();
  }

  Vector3::value_type Vector3::LengthSqr() const
  {
    return x * x + y * y + z * z;
  }

  Vector3 Vector3::Normalize() const
  {
    value_type length = Magnitude();
    if (length == 0) return { 0, 0, 0 };
    return { x / length, y / length, z / length };
  }

#pragma endregion

#pragma region Passthrough Functions

  Vector3::value_type Vector3::at(const Vector3::size_type index) const { return data[index]; }
  Vector3::value_type Vector3::operator[](const Vector3::size_type index) { return at(index); }
  Vector3::const_value_type Vector3::operator[](const Vector3::size_type index) const { return at(index); }

  Vector3::iterator                 Vector3::begin() { return data; }
  Vector3::const_iterator           Vector3::begin() const { return data; }
  Vector3::const_iterator           Vector3::cbegin() const { return data; }
  Vector3::iterator                 Vector3::end() { return data + size(); }
  Vector3::const_iterator           Vector3::end() const { return data + size(); }
  Vector3::const_iterator           Vector3::cend() const { return data + size(); }
  //Vector3::reverse_iterator         Vector3::rbegin()         { return data + size() - 1; }
  //Vector3::const_reverse_iterator   Vector3::rbegin() const   { return data + size() - 1; }
  //Vector3::const_reverse_iterator   Vector3::crbegin() const  { return data + size() - 1; }
  //Vector3::reverse_iterator         Vector3::rend()           { return data - 1; }
  //Vector3::const_reverse_iterator   Vector3::rend() const     { return data - 1; }
  //Vector3::const_reverse_iterator   Vector3::crend() const    { return data - 1; }

  Vector3::size_type Vector3::size() const { return 3; }

#pragma endregion

#pragma region Vector3 Helper Fns

  Vector3 operator+(const Vector3& point_a, const Vector3& point_b)
  {
    return { point_a.x + point_b.x, point_a.y + point_b.y, point_a.z + point_b.z };
  }

  Vector3 operator+(Vector3::const_value_type value, const Vector3& point)
  {
    return { point.x + value, point.y + value, point.z + value };
  }

  Vector3 operator+(const Vector3& point, Vector3::const_value_type value)
  {
    return { point.x + value, point.y + value, point.z + value };
  }

  Vector3 operator-(const Vector3& point_a, const Vector3& point_b)
  {
    return { point_a.x - point_b.x, point_a.y - point_b.y, point_a.z - point_b.z };
  }

  Vector3 operator-(Vector3::const_value_type value, const Vector3& point)
  {
    return { point.x - value, point.y - value, point.z - value };
  }

  Vector3 operator-(const Vector3& point, Vector3::const_value_type value)
  {
    return { point.x - value, point.y - value, point.z - value };
  }

  //Vector3 operator*(const Vector3& point_a, const Vector3& point_b)
  //{
  //  return { point_a.x * point_b.x, point_a.y * point_b.y, point_a.z * point_b.z };
  //}

  Vector3::value_type Dot(const Vector3& point_a, const Vector3& point_b)
  {
    return point_a.x * point_b.x + point_a.y * point_b.y + point_a.z * point_b.z;
  }

  Vector3 operator*(Vector3::const_value_type value, const Vector3& point)
  {
    return { point.x * value, point.y * value, point.z * value };
  }

  Vector3 operator*(const Vector3& point, Vector3::const_value_type value)
  {
    return { point.x * value, point.y * value, point.z * value };
  }

  //Vector3 operator/(const Vector3& point, const_value_type value)
  //{
  //  return { point.x / value, point.y / value, point.z / value };
  //}

  Vector3 operator/(Vector3::const_value_type value, const Vector3& point)
  {
    if (point.x == 0 || point.y == 0 || point.z == 0) return { 0, 0, 0 };
    return { value / point.x, value / point.y, value / point.z };
  }

  Vector3 operator/(const Vector3& point, Vector3::const_value_type value)
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