#include "vector4.h"

namespace FlexEngine
{

#pragma region Reflection

  FLX_REFL_REGISTER_START(Vector4)
    FLX_REFL_REGISTER_PROPERTY(x)
    FLX_REFL_REGISTER_PROPERTY(y)
    FLX_REFL_REGISTER_PROPERTY(z)
    FLX_REFL_REGISTER_PROPERTY(w)
  FLX_REFL_REGISTER_END;

#pragma endregion
  
#pragma region Standard Functions

  const Vector4 Vector4::Zero = { 0, 0, 0, 0 };
  const Vector4 Vector4::One  = { 1, 1, 1, 1 };

  Vector4::operator bool() const { return XYZW() != Zero; }
  Vector4::operator Vector2() const { return XY(); }
  Vector4::operator Vector3() const { return XYZ(); }

  std::string Vector4::ToString() const
  {
    return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ", " + std::to_string(w) + ")";
  }
  Vector4::operator std::string() const { return ToString(); }

#ifdef _DEBUG
  void Vector4::Dump() const { Log::Debug(ToString()); }
#endif

#pragma endregion
  
#pragma region Constructors

  Vector4::Vector4(value_type _x, value_type _y, value_type _z, value_type _w)
  {
    x = _x;
    y = _y;
    z = _z;
    w = _w;
  }

  Vector4::Vector4(const Vector4& other)
  {
    x = other.x;
    y = other.y;
    z = other.z;
    w = other.w;
  }

  Vector4::Vector4(const Vector3& xyz, value_type _w)
  {
    x = xyz.x;
    y = xyz.y;
    z = xyz.z;
    w = _w;
  }

  Vector4::Vector4(const Vector2& xy, value_type _z, value_type _w)
  {
    x = xy.x;
    y = xy.y;
    z = _z;
    w = _w;
  }

#pragma endregion
  
#pragma region Getters

  // Getters for the different combinations of the vector

  Vector2 Vector4::XY() const { return { x, y }; }
  Vector2 Vector4::XZ() const { return { x, z }; }
  Vector2 Vector4::XW() const { return { x, w }; }

  Vector2 Vector4::YX() const { return { y, x }; }
  Vector2 Vector4::YZ() const { return { y, z }; }
  Vector2 Vector4::YW() const { return { y, w }; }

  Vector2 Vector4::ZX() const { return { z, x }; }
  Vector2 Vector4::ZY() const { return { z, y }; }
  Vector2 Vector4::ZW() const { return { z, w }; }

  Vector2 Vector4::WX() const { return { w, x }; }
  Vector2 Vector4::WY() const { return { w, y }; }
  Vector2 Vector4::WZ() const { return { w, z }; }


  Vector3 Vector4::XYZ() const { return { x, y, z }; }
  Vector3 Vector4::XYW() const { return { x, y, w }; }
  Vector3 Vector4::XZY() const { return { x, z, y }; }
  Vector3 Vector4::XZW() const { return { x, z, w }; }
  Vector3 Vector4::XWY() const { return { x, w, y }; }
  Vector3 Vector4::XWZ() const { return { x, w, z }; }

  Vector3 Vector4::YXZ() const { return { y, x, z }; }
  Vector3 Vector4::YXW() const { return { y, x, w }; }
  Vector3 Vector4::YZX() const { return { y, z, x }; }
  Vector3 Vector4::YZW() const { return { y, z, w }; }
  Vector3 Vector4::YWX() const { return { y, w, x }; }
  Vector3 Vector4::YWZ() const { return { y, w, z }; }

  Vector3 Vector4::ZXY() const { return { z, x, y }; }
  Vector3 Vector4::ZXW() const { return { z, x, w }; }
  Vector3 Vector4::ZYX() const { return { z, y, x }; }
  Vector3 Vector4::ZYW() const { return { z, y, w }; }
  Vector3 Vector4::ZWX() const { return { z, w, x }; }
  Vector3 Vector4::ZWY() const { return { z, w, y }; }

  Vector3 Vector4::WXY() const { return { w, x, y }; }
  Vector3 Vector4::WXZ() const { return { w, x, z }; }
  Vector3 Vector4::WYX() const { return { w, y, x }; }
  Vector3 Vector4::WYZ() const { return { w, y, z }; }
  Vector3 Vector4::WZX() const { return { w, z, x }; }
  Vector3 Vector4::WZY() const { return { w, z, y }; }


  Vector4 Vector4::XYZW() const { return { x, y, z, w }; }
  Vector4 Vector4::XYWZ() const { return { x, y, w, z }; }
  Vector4 Vector4::XZYW() const { return { x, z, y, w }; }
  Vector4 Vector4::XZWY() const { return { x, z, w, y }; }
  Vector4 Vector4::XWYZ() const { return { x, w, y, z }; }
  Vector4 Vector4::XWZY() const { return { x, w, z, y }; }

  Vector4 Vector4::YXZW() const { return { y, x, z, w }; }
  Vector4 Vector4::YXWZ() const { return { y, x, w, z }; }
  Vector4 Vector4::YZXW() const { return { y, z, x, w }; }
  Vector4 Vector4::YZWX() const { return { y, z, w, x }; }
  Vector4 Vector4::YWZX() const { return { y, w, z, x }; }
  Vector4 Vector4::YWXZ() const { return { y, w, x, z }; }

  Vector4 Vector4::ZXYW() const { return { z, x, y, w }; }
  Vector4 Vector4::ZXWY() const { return { z, x, w, y }; }
  Vector4 Vector4::ZYXW() const { return { z, y, x, w }; }
  Vector4 Vector4::ZYWX() const { return { z, y, w, x }; }
  Vector4 Vector4::ZWXY() const { return { z, w, x, y }; }
  Vector4 Vector4::ZWYX() const { return { z, w, y, x }; }

  Vector4 Vector4::WXYZ() const { return { w, x, y, z }; }
  Vector4 Vector4::WXZY() const { return { w, x, z, y }; }
  Vector4 Vector4::WYXZ() const { return { w, y, x, z }; }
  Vector4 Vector4::WYZX() const { return { w, y, z, x }; }
  Vector4 Vector4::WZXY() const { return { w, z, x, y }; }
  Vector4 Vector4::WZYX() const { return { w, z, y, x }; }

#pragma endregion

#pragma region Operator Overloading

  // - =
  // += -= *= /=
  // == !=
  // + - * /

  Vector4 Vector4::operator-() const
  {
    return { -x, -y, -z, -w };
  }

  Vector4& Vector4::operator=(const Vector4& other)
  {
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;
    this->w = other.w;
    return *this;
  }

  Vector4& Vector4::operator+=(const Vector4& other)
  {
    this->x += other.x;
    this->y += other.y;
    this->z += other.z;
    this->w += other.w;
    return *this;
  }

  Vector4& Vector4::operator+=(const_value_type value)
  {
    this->x += value;
    this->y += value;
    this->z += value;
    this->w += value;
    return *this;
  }

  Vector4& Vector4::operator-=(const Vector4& other)
  {
    this->x -= other.x;
    this->y -= other.y;
    this->z -= other.z;
    this->w -= other.w;
    return *this;
  }

  Vector4& Vector4::operator-=(const_value_type value)
  {
    this->x -= value;
    this->y -= value;
    this->z -= value;
    this->w -= value;
    return *this;
  }

  Vector4& Vector4::operator*=(const Vector4& other)
  {
    this->x *= other.x;
    this->y *= other.y;
    this->z *= other.z;
    this->w *= other.w;
    return *this;
  }

  Vector4& Vector4::operator*=(const_value_type& value)
  {
    this->x *= value;
    this->y *= value;
    this->z *= value;
    this->w *= value;
    return *this;
  }

  Vector4& Vector4::operator/=(const Vector4& other)
  {
    if (other.x == 0 || other.y == 0 || other.z == 0 || other.w == 0) return *this;
    this->x /= other.x;
    this->y /= other.y;
    this->z /= other.z;
    this->w /= other.w;
    return *this;
  }

  Vector4& Vector4::operator/=(const_value_type value)
  {
    if (value == 0) return *this;
    this->x /= value;
    this->y /= value;
    this->z /= value;
    this->w /= value;
    return *this;
  }

  bool Vector4::operator==(const Vector4& other) const
  {
    return x - other.x < EPSILON_f && y - other.y < EPSILON_f && z - other.z < EPSILON_f && w - other.w < EPSILON_f;
  }

  bool Vector4::operator!=(const Vector4& other) const
  {
    return !(x - other.x < EPSILON_f && y - other.y < EPSILON_f && z - other.z < EPSILON_f && w - other.w < EPSILON_f);
  }

  // Note: Returns the magnitude of the xyz component
  Vector4::value_type Vector4::Magnitude() const
  {
    return std::sqrt(x * x + y * y + z * z);
  }

  // Note: Returns the length of the xyz component
  Vector4::value_type Vector4::Length() const
  {
    return Magnitude();
  }

  // Note: Returns the length of the xyz component
  Vector4::value_type Vector4::LengthSqr() const
  {
    return x * x + y * y + z * z;
  }

  // Note: Returns the normalized xyz component
  Vector4 Vector4::Normalize() const
  {
    value_type length = Magnitude();
    if (length == 0) return { 0, 0, 0, 0 };
    return { x / length, y / length, z / length, w / length };
  }

#pragma endregion

#pragma region Passthrough Functions

  Vector4::value_type Vector4::at(const Vector4::size_type index) const { return data[index]; }
  Vector4::value_type Vector4::operator[](const Vector4::size_type index) { return at(index); }
  Vector4::const_value_type Vector4::operator[](const Vector4::size_type index) const { return at(index); }

  Vector4::iterator                 Vector4::begin() { return data; }
  Vector4::const_iterator           Vector4::begin() const { return data; }
  Vector4::const_iterator           Vector4::cbegin() const { return data; }
  Vector4::iterator                 Vector4::end() { return data + size(); }
  Vector4::const_iterator           Vector4::end() const { return data + size(); }
  Vector4::const_iterator           Vector4::cend() const { return data + size(); }
  //Vector4::reverse_iterator         Vector4::rbegin()         { return data + size() - 1; }
  //Vector4::const_reverse_iterator   Vector4::rbegin() const   { return data + size() - 1; }
  //Vector4::const_reverse_iterator   Vector4::crbegin() const  { return data + size() - 1; }
  //Vector4::reverse_iterator         Vector4::rend()           { return data - 1; }
  //Vector4::const_reverse_iterator   Vector4::rend() const     { return data - 1; }
  //Vector4::const_reverse_iterator   Vector4::crend() const    { return data - 1; }

  Vector4::size_type Vector4::size() const { return 3; }

#pragma endregion

#pragma region Vector4 Helper Fns

  Vector4 operator+(const Vector4& point_a, const Vector4& point_b)
  {
    return { point_a.x + point_b.x, point_a.y + point_b.y, point_a.z + point_b.z, point_a.w + point_b.w };
  }

  Vector4 operator+(Vector4::const_value_type value, const Vector4& point)
  {
    return { point.x + value, point.y + value, point.z + value, point.w + value };
  }

  Vector4 operator+(const Vector4& point, Vector4::const_value_type value)
  {
    return { point.x + value, point.y + value, point.z + value, point.w + value };
  }

  Vector4 operator-(const Vector4& point_a, const Vector4& point_b)
  {
    return { point_a.x - point_b.x, point_a.y - point_b.y, point_a.z - point_b.z, point_a.w - point_b.w };
  }

  Vector4 operator-(Vector4::const_value_type value, const Vector4& point)
  {
    return { point.x - value, point.y - value, point.z - value, point.w - value };
  }

  Vector4 operator-(const Vector4& point, Vector4::const_value_type value)
  {
    return { point.x - value, point.y - value, point.z - value, point.w - value };
  }

  //Vector4 operator*(const Vector4& point_a, const Vector4& point_b)
  //{
  //  return { point_a.x * point_b.x, point_a.y * point_b.y, point_a.z * point_b.z, point_a.w * point_b.w };
  //}

  Vector4 operator*(Vector4::const_value_type value, const Vector4& point)
  {
    return { point.x * value, point.y * value, point.z * value, point.w * value };
  }

  Vector4 operator*(const Vector4& point, Vector4::const_value_type value)
  {
    return { point.x * value, point.y * value, point.z * value, point.w * value };
  }

  //Vector4 operator/(const Vector4& point_a, const Vector4& point_b)
  //{
  //  return { point_a.x / point_b.x, point_a.y / point_b.y, point_a.z / point_b.z, point_a.w / point_b.w };
  //}

  Vector4 operator/(Vector4::const_value_type value, const Vector4& point)
  {
    if (point.x == 0 || point.y == 0 || point.z == 0 || point.w == 0) return { 0, 0, 0, 0 };
    return { value / point.x, value / point.y, value / point.z, value / point.w };
  }

  Vector4 operator/(const Vector4& point, Vector4::const_value_type value)
  {
    if (value == 0) return { 0, 0, 0, 0 };
    return { point.x / value, point.y / value, point.z / value, point.w / value };
  }

  std::istream& operator>>(std::istream& is, Vector4& point)
  {
    return is >> point.x >> point.y >> point.z >> point.w;
  }

  std::ostream& operator<<(std::ostream& os, const Vector4& point)
  {
    return os << "(" << point.x << ", " << point.y << ", " << point.z << ", " << point.w << ")";
  }

#pragma endregion

}