#include "quaternion.h"

#define QUAT_EPSILONf 0.0001f

namespace FlexEngine
{

#pragma region Reflection

  FLX_REFL_REGISTER_START(Quaternion)
    FLX_REFL_REGISTER_PROPERTY(x)
    FLX_REFL_REGISTER_PROPERTY(y)
    FLX_REFL_REGISTER_PROPERTY(z)
    FLX_REFL_REGISTER_PROPERTY(w)
  FLX_REFL_REGISTER_END;

#pragma endregion
  
#pragma region Standard Functions

  const Quaternion Quaternion::Zero = { 0, 0, 0, 0 };
  const Quaternion Quaternion::Identity = { 0, 0, 0, 1 };

  Quaternion::operator bool() const { return *this != Zero; }

  Quaternion::operator Vector3() const { return ToEulerAngles(); }
  // Follows the wikipedia article on conversion from quaternion to euler angles
  Vector3 Quaternion::ToEulerAngles() const
  {
    // early out if the quaternion is identity or zero
    if (*this == Quaternion::Identity || *this == Quaternion::Zero) return { 0, 0, 0 };

    Quaternion q = *this;

    const_value_type sqw = q.w * q.w;
    const_value_type sqx = q.x * q.x;
    const_value_type sqy = q.y * q.y;
    const_value_type sqz = q.z * q.z;
    const_value_type unit = sqx + sqy + sqz + sqw; // if normalised is one, otherwise is correction factor
    const_value_type test = q.x * q.y + q.z * q.w;

    // singularity at north pole
    if (test > 0.499f * unit)
    {
      return {
        2.0f * atan2(q.x, q.w),
        PIf / 2.0f,
        0.0f
      };
    }

    // singularity at south pole
    if (test < -0.499f * unit)
    {
      return {
        -2.0f * atan2(q.x, q.w),
        -PIf / 2.0f,
        0.0f
      };
    }

    return {
      atan2(2.0f * q.y * q.w - 2.0f * q.x * q.z, sqx - sqy - sqz + sqw),
      asin(2.0f * test / unit),
      atan2(2.0f * q.x * q.w - 2.0f * q.y * q.z, -sqx + sqy - sqz + sqw)
    };
  }

  Quaternion::operator Matrix4x4() const { return ToRotationMatrix(); }
  // Follows the euclideanspace website on conversion from quaternion to matrix
  // The matrix is in column-major order.
  Matrix4x4 Quaternion::ToRotationMatrix() const
  {
    const_value_type xx = x * x;
    const_value_type xy = x * y;
    const_value_type xz = x * z;
    const_value_type xw = x * w;
    const_value_type yy = y * y;
    const_value_type yz = y * z;
    const_value_type yw = y * w;
    const_value_type zz = z * z;
    const_value_type zw = z * w;

    return Matrix4x4(
      1 - (2 * (yy + zz)),     (2 * (xy - zw)),     (2 * (xz + yw)), 0,
          (2 * (xy + zw)), 1 - (2 * (xx + zz)),     (2 * (yz - xw)), 0,
          (2 * (xz - yw)),     (2 * (yz + xw)), 1 - (2 * (xx + yy)), 0,
                        0,                   0,                   0, 1
    ).Transpose();
  }

  std::string Quaternion::ToString() const
  {
    return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ", " + std::to_string(w) + ")";
  }
  Quaternion::operator std::string() const { return ToString(); }

#ifdef _DEBUG
  void Quaternion::Dump() const { Log::Debug(ToString()); }
#endif

#pragma endregion
  
#pragma region Constructors

  Quaternion::Quaternion(value_type _x, value_type _y, value_type _z, value_type _w)
  {
    x = _x;
    y = _y;
    z = _z;
    w = _w;
  }

  Quaternion::Quaternion(const Quaternion& other)
  {
    x = other.x;
    y = other.y;
    z = other.z;
    w = other.w;
  }

  Quaternion::Quaternion(const Vector4& other)
  {
    x = other.x;
    y = other.y;
    z = other.z;
    w = other.w;
  }

  Quaternion Quaternion::FromEulerAngles(const Vector3& angles_in_radians)
  {
    const_value_type cx = cosf(angles_in_radians.x / 2.0f);
    const_value_type sx = sinf(angles_in_radians.x / 2.0f);
    const_value_type cy = cosf(angles_in_radians.y / 2.0f);
    const_value_type sy = sinf(angles_in_radians.y / 2.0f);
    const_value_type cz = cosf(angles_in_radians.z / 2.0f);
    const_value_type sz = sinf(angles_in_radians.z / 2.0f);

    return {
      sx * cy * cz + cx * sy * sz,
      cx * sy * cz - sx * cy * sz,
      cx * cy * sz + sx * sy * cz,
      cx * cy * cz - sx * sy * sz
    };
  }

  Quaternion Quaternion::FromEulerAnglesDeg(const Vector3& angles_in_degrees)
  {
    return FromEulerAngles(radians(angles_in_degrees));
  }

#pragma endregion

#pragma region Operator Overloading

  // - =
  // += -= *= /=
  // == !=
  // + - * /

  Quaternion Quaternion::operator-() const
  {
    return { -x, -y, -z, -w };
  }

  Quaternion& Quaternion::operator=(const Quaternion& other)
  {
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;
    this->w = other.w;
    return *this;
  }

  Quaternion& Quaternion::operator+=(const Quaternion& other)
  {
    this->x += other.x;
    this->y += other.y;
    this->z += other.z;
    this->w += other.w;
    return *this;
  }

  Quaternion& Quaternion::operator+=(const_value_type value)
  {
    this->x += value;
    this->y += value;
    this->z += value;
    this->w += value;
    return *this;
  }

  Quaternion& Quaternion::operator-=(const Quaternion& other)
  {
    this->x -= other.x;
    this->y -= other.y;
    this->z -= other.z;
    this->w -= other.w;
    return *this;
  }

  Quaternion& Quaternion::operator-=(const_value_type value)
  {
    this->x -= value;
    this->y -= value;
    this->z -= value;
    this->w -= value;
    return *this;
  }

  Quaternion& Quaternion::operator*=(const Quaternion& other)
  {
    this->x *= other.x;
    this->y *= other.y;
    this->z *= other.z;
    this->w *= other.w;
    return *this;
  }

  Quaternion& Quaternion::operator*=(const_value_type& value)
  {
    this->x *= value;
    this->y *= value;
    this->z *= value;
    this->w *= value;
    return *this;
  }

  Quaternion& Quaternion::operator/=(const Quaternion& other)
  {
    if (other.x == 0 || other.y == 0 || other.z == 0 || other.w == 0) return *this;
    this->x /= other.x;
    this->y /= other.y;
    this->z /= other.z;
    this->w /= other.w;
    return *this;
  }

  Quaternion& Quaternion::operator/=(const_value_type value)
  {
    if (value == 0) return *this;
    this->x /= value;
    this->y /= value;
    this->z /= value;
    this->w /= value;
    return *this;
  }

  bool Quaternion::operator==(const Quaternion& other) const
  {
    if constexpr (std::is_same_v<value_type, float>)
    {
      return abs(Dot(*this, other) - 1.0f) < QUAT_EPSILONf;
    }
    else if constexpr (std::is_same_v<value_type, double>)
    {
      return abs(Dot(*this, other) - 1.0) < QUAT_EPSILONf;
    }
  }

  bool Quaternion::operator!=(const Quaternion& other) const
  {
    return !(*this == other);
  }

  Quaternion::value_type Quaternion::Magnitude() const
  {
    return std::sqrt(x * x + y * y + z * z + w * w);
  }

  Quaternion::value_type Quaternion::Length() const
  {
    return Magnitude();
  }

  Quaternion::value_type Quaternion::LengthSqr() const
  {
    return x * x + y * y + z * z + w * w;
  }

  Quaternion& Quaternion::Normalize()
  {
    const_value_type length = Magnitude();
    if (length == 0) return *this;
    return *this /= length;
  }

  Quaternion Quaternion::Normalize(const Quaternion& other)
  {
    Quaternion result = other;
    return result.Normalize();
  }

#pragma endregion

#pragma region Passthrough Functions

  Quaternion::reference Quaternion::at(const Quaternion::size_type index) { return data[index]; }
  Quaternion::const_reference Quaternion::at(const Quaternion::size_type index) const { return data[index]; }
  Quaternion::reference Quaternion::operator[](const Quaternion::size_type index) { return at(index); }
  Quaternion::const_reference Quaternion::operator[](const Quaternion::size_type index) const { return at(index); }

  Quaternion::iterator                 Quaternion::begin() { return data; }
  Quaternion::const_iterator           Quaternion::begin() const { return data; }
  Quaternion::const_iterator           Quaternion::cbegin() const { return data; }
  Quaternion::iterator                 Quaternion::end() { return data + size(); }
  Quaternion::const_iterator           Quaternion::end() const { return data + size(); }
  Quaternion::const_iterator           Quaternion::cend() const { return data + size(); }
  //Quaternion::reverse_iterator         Quaternion::rbegin()         { return data + size() - 1; }
  //Quaternion::const_reverse_iterator   Quaternion::rbegin() const   { return data + size() - 1; }
  //Quaternion::const_reverse_iterator   Quaternion::crbegin() const  { return data + size() - 1; }
  //Quaternion::reverse_iterator         Quaternion::rend()           { return data - 1; }
  //Quaternion::const_reverse_iterator   Quaternion::rend() const     { return data - 1; }
  //Quaternion::const_reverse_iterator   Quaternion::crend() const    { return data - 1; }

  constexpr Quaternion::size_type Quaternion::size() const { return 4; }

#pragma endregion

#pragma region Quaternion Helper Fns

  Quaternion operator+(const Quaternion& point_a, const Quaternion& point_b)
  {
    return { point_a.x + point_b.x, point_a.y + point_b.y, point_a.z + point_b.z, point_a.w + point_b.w };
  }

  Quaternion operator+(Quaternion::const_value_type value, const Quaternion& point)
  {
    return { point.x + value, point.y + value, point.z + value, point.w + value };
  }

  Quaternion operator+(const Quaternion& point, Quaternion::const_value_type value)
  {
    return { point.x + value, point.y + value, point.z + value, point.w + value };
  }

  Quaternion operator-(const Quaternion& point_a, const Quaternion& point_b)
  {
    return { point_a.x - point_b.x, point_a.y - point_b.y, point_a.z - point_b.z, point_a.w - point_b.w };
  }

  Quaternion operator-(Quaternion::const_value_type value, const Quaternion& point)
  {
    return { value - point.x, value - point.y, value - point.z, value - point.w };
  }

  Quaternion operator-(const Quaternion& point, Quaternion::const_value_type value)
  {
    return { point.x - value, point.y - value, point.z - value, point.w - value };
  }

  Quaternion operator*(const Quaternion& point_a, const Quaternion& point_b)
  {
    return { point_a.x * point_b.x, point_a.y * point_b.y, point_a.z * point_b.z, point_a.w * point_b.w };
  }

  Quaternion operator*(Quaternion::const_value_type value, const Quaternion& point)
  {
    return { point.x * value, point.y * value, point.z * value, point.w * value };
  }

  Quaternion operator*(const Quaternion& point, Quaternion::const_value_type value)
  {
    return { point.x * value, point.y * value, point.z * value, point.w * value };
  }

  Quaternion operator/(const Quaternion& point_a, const Quaternion& point_b)
  {
    return { point_a.x / point_b.x, point_a.y / point_b.y, point_a.z / point_b.z, point_a.w / point_b.w };
  }

  Quaternion operator/(Quaternion::const_value_type value, const Quaternion& point)
  {
    if (point.x == 0 || point.y == 0 || point.z == 0 || point.w == 0) return { 0, 0, 0, 0 };
    return { value / point.x, value / point.y, value / point.z, value / point.w };
  }

  Quaternion operator/(const Quaternion& point, Quaternion::const_value_type value)
  {
    if (value == 0) return { 0, 0, 0, 0 };
    return { point.x / value, point.y / value, point.z / value, point.w / value };
  }

  std::istream& operator>>(std::istream& is, Quaternion& point)
  {
    for (Quaternion::size_type i = 0; i < point.size(); ++i)
    {
      is >> point.data[i];
    }
    return is;
  }

  std::ostream& operator<<(std::ostream& os, const Quaternion& point)
  {
    for (Quaternion::size_type i = 0; i < point.size(); ++i)
    {
      os << point.data[i];
      if (i < point.size() - 1) os << " ";
    }
    return os;
  }

  Quaternion Lerp(const Quaternion& a, const Quaternion& b, Quaternion::const_value_type t)
  {
    Quaternion::value_type angle = acos(Dot(a, b));
    Quaternion::value_type sin_angle = sin(angle);
    if (sin_angle == 0) return a;
    return Quaternion::Normalize( (a * sin((1 - t) * angle) + b * sin(t * angle)) / sin_angle );
  }

  Quaternion RotateTowards(const Quaternion& from, const Quaternion& to, Quaternion::const_value_type max_angle)
  {
    // guard: no rotation allowed
    if (max_angle < QUAT_EPSILONf) return from;

    Quaternion::value_type cos_angle = Dot(from, to);

    // guard: already at the target
    if (cos_angle > (1.0f - QUAT_EPSILONf)) return to;

    Quaternion target = to;

    if (cos_angle < 0)
    {
      // flip the sign of the target quaternion
      target = -target;
      cos_angle = -cos_angle;
    }

    Quaternion::value_type angle = acos(cos_angle);

    // guard: target is too close to the from quaternion
    if (angle < max_angle) return target;

    Quaternion::value_type t = max_angle / angle;
    angle = max_angle;

    return Quaternion::Normalize( (from * sin(angle * (1 - t)) + target * sin(angle * t)) / sin(angle) );
  }

#pragma endregion

}