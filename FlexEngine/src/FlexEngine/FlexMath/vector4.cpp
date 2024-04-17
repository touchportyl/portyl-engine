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

  Vector4::operator bool() const { return *this != Zero; }
  Vector4::operator Vector2() const { return { x, y }; }
  Vector4::operator Vector3() const { return { x, y, z }; }

  Vector4 Vector4::Swizzle(const std::string& swizzle) const
  {
    // build new vector based on swizzle
    Vector3 new_vector;
    for (size_type i = 0; i < size(); ++i)
    {
      switch (swizzle[i])
      {
      case '0': case 'x': case 'r': case 's': new_vector[i] = x; break;
      case '1': case 'y': case 'g': case 't': new_vector[i] = y; break;
      case '2': case 'z': case 'b': case 'p': new_vector[i] = z; break;
      case '3': case 'w': case 'a': case 'q': new_vector[i] = w; break;
      default: new_vector[i] = data[i]; break;
      }
    }
    return new_vector;
  }

  void Vector4::Swizzle(Vector4& other, const std::string& swizzle)
  {
    other = other.Swizzle(swizzle);
  }

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

  Vector4::reference Vector4::at(const Vector4::size_type index) { return data[index]; }
  Vector4::const_reference Vector4::at(const Vector4::size_type index) const { return data[index]; }
  Vector4::reference Vector4::operator[](const Vector4::size_type index) { return at(index); }
  Vector4::const_reference Vector4::operator[](const Vector4::size_type index) const { return at(index); }

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

  constexpr Vector4::size_type Vector4::size() const { return 3; }

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