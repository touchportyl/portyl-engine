#pragma once

#include "vector3.h"

namespace FlexEngine
{

  struct Vector4
  { FLX_REFL_SERIALIZABLE

    // Using directives

    using value_type = float;
    using const_value_type = const value_type;
    using size_type = std::size_t;
    using reference = value_type&;
    using const_reference = const_value_type&;
    using pointer = value_type*;
    using const_pointer = const_value_type*;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = iterator;
    using const_reverse_iterator = const_iterator;

    // Data members

    union
    {
    #pragma warning(push)
    #pragma warning(disable: 4201) // nonstandard extension used: nameless struct/union
      struct { value_type x, y, z, w; };
      struct { value_type r, g, b, a; };
      struct { value_type s, t, p, q; };
      struct { value_type first, second, third, fourth; };
    #pragma warning(pop)
      value_type data[4];
    };

#pragma region Standard Functions

    // Shorthand, follows Unity conventions

    static const Vector4 Zero;
    static const Vector4 One;

    // Conversion operators

    operator bool() const;
    operator Vector2() const;
    operator Vector3() const;

    // Stringify the vector

    std::string ToString() const;
    operator std::string() const;

#ifdef _DEBUG
    void Dump() const;
#endif

#pragma endregion

#pragma region Constructors

    Vector4(value_type _x = 0.0f, value_type _y = 0.0f, value_type _z = 0.0f, value_type _w = 0.0f);
    Vector4(const Vector4& other);
    Vector4(const Vector3& xyz, value_type _w = 0.0f);
    Vector4(const Vector2& xy, value_type _z = 0.0f, value_type _w = 0.0f);

#pragma endregion

#pragma region Getters

    // Getters for the different combinations of the vector

    Vector2 XY() const;
    Vector2 XZ() const;
    Vector2 XW() const;

    Vector2 YX() const;
    Vector2 YZ() const;
    Vector2 YW() const;

    Vector2 ZX() const;
    Vector2 ZY() const;
    Vector2 ZW() const;

    Vector2 WX() const;
    Vector2 WY() const;
    Vector2 WZ() const;


    Vector3 XYZ() const;
    Vector3 XYW() const;
    Vector3 XZY() const;
    Vector3 XZW() const;
    Vector3 XWY() const;
    Vector3 XWZ() const;

    Vector3 YXZ() const;
    Vector3 YXW() const;
    Vector3 YZX() const;
    Vector3 YZW() const;
    Vector3 YWX() const;
    Vector3 YWZ() const;

    Vector3 ZXY() const;
    Vector3 ZXW() const;
    Vector3 ZYX() const;
    Vector3 ZYW() const;
    Vector3 ZWX() const;
    Vector3 ZWY() const;

    Vector3 WXY() const;
    Vector3 WXZ() const;
    Vector3 WYX() const;
    Vector3 WYZ() const;
    Vector3 WZX() const;
    Vector3 WZY() const;


    Vector4 XYZW() const;
    Vector4 XYWZ() const;
    Vector4 XZYW() const;
    Vector4 XZWY() const;
    Vector4 XWYZ() const;
    Vector4 XWZY() const;

    Vector4 YXZW() const;
    Vector4 YXWZ() const;
    Vector4 YZXW() const;
    Vector4 YZWX() const;
    Vector4 YWZX() const;
    Vector4 YWXZ() const;

    Vector4 ZXYW() const;
    Vector4 ZXWY() const;
    Vector4 ZYXW() const;
    Vector4 ZYWX() const;
    Vector4 ZWXY() const;
    Vector4 ZWYX() const;

    Vector4 WXYZ() const;
    Vector4 WXZY() const;
    Vector4 WYXZ() const;
    Vector4 WYZX() const;
    Vector4 WZXY() const;
    Vector4 WZYX() const;

#pragma endregion

#pragma region Operator Overloading

    // - =
    // += -= *= /=
    // == !=
    // + - * /

    Vector4 operator-() const;

    Vector4& operator=(const Vector4& other);

    Vector4& operator+=(const Vector4& other);
    Vector4& operator+=(const_value_type value);

    Vector4& operator-=(const Vector4& other);
    Vector4& operator-=(const_value_type value);

    Vector4& operator*=(const Vector4& other);
    Vector4& operator*=(const_value_type& value);

    Vector4& operator/=(const Vector4& other);
    Vector4& operator/=(const_value_type value);

    bool operator==(const Vector4& other) const;
    bool operator!=(const Vector4& other) const;

    // Note: Returns the magnitude of the xyz component
    value_type Magnitude() const;

    // Note: Returns the length of the xyz component
    value_type Length() const;

    // Note: Returns the length of the xyz component
    value_type LengthSqr() const;

    // Note: Returns the normalized xyz component
    Vector4 Normalize() const;

#pragma endregion

#pragma region Passthrough Functions

    // Accessors

    value_type at(const size_type index) const;
    value_type operator[](const size_type index);
    const_value_type operator[](const size_type index) const;

    // Iterators

    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const;
    iterator end();
    const_iterator end() const;
    const_iterator cend() const;
    //reverse_iterator rbegin();
    //const_reverse_iterator rbegin() const;
    //const_reverse_iterator crbegin() const;
    //reverse_iterator rend();
    //const_reverse_iterator rend() const;
    //const_reverse_iterator crend() const;

    // Capacity

    size_type size() const;

#pragma endregion

  };

#pragma region Vector4 Helper Fns

  Vector4 operator+(const Vector4& point_a, const Vector4& point_b);
  Vector4 operator+(Vector4::const_value_type value, const Vector4& point);
  Vector4 operator+(const Vector4& point, Vector4::const_value_type value);

  Vector4 operator-(const Vector4& point_a, const Vector4& point_b);
  Vector4 operator-(Vector4::const_value_type value, const Vector4& point);
  Vector4 operator-(const Vector4& point, Vector4::const_value_type value);

  //Vector4 operator*(const Vector4& point_a, const Vector4& point_b);
  Vector4 operator*(Vector4::const_value_type value, const Vector4& point);
  Vector4 operator*(const Vector4& point, Vector4::const_value_type value);

  //Vector4 operator/(const Vector4& point_a, const Vector4& point_b);
  Vector4 operator/(Vector4::const_value_type value, const Vector4& point);
  Vector4 operator/(const Vector4& point, Vector4::const_value_type value);

  std::istream& operator>>(std::istream& is, Vector4& point);
  std::ostream& operator<<(std::ostream& os, const Vector4& point);

#pragma endregion

}