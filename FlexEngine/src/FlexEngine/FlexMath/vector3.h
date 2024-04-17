#pragma once

#include "vector2.h"

namespace FlexEngine
{

  struct Vector3
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
      struct { value_type x, y, z; };
      struct { value_type r, g, b; };
      struct { value_type u, v, w; };
      struct { value_type first, second, third; };
    #pragma warning(pop)
      value_type data[3];
    };

#pragma region Standard Functions

    // Shorthand, follows Unity conventions

    static const Vector3 Zero;
    static const Vector3 One;
    static const Vector3 Up;
    static const Vector3 Down;
    static const Vector3 Left;
    static const Vector3 Right;
    static const Vector3 Forward;
    static const Vector3 Back;

    // Conversion operators

    operator bool() const;
    operator Vector2() const;

    // Stringify the vector

    std::string ToString() const;
    operator std::string() const;

#ifdef _DEBUG
    void Dump() const;
#endif

#pragma endregion

#pragma region Constructors

    Vector3(value_type _x = 0.0f, value_type _y = 0.0f, value_type _z = 0.0f);
    Vector3(const Vector3& other);
    Vector3(const Vector2& xy, value_type _z = 0.0f);
    Vector3(value_type _x, const Vector2& yz);

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
    Vector3& operator+=(const_value_type value);

    Vector3& operator-=(const Vector3& other);
    Vector3& operator-=(const_value_type value);

    Vector3& operator*=(const Vector3& other);
    Vector3& operator*=(const_value_type value);

    Vector3& operator/=(const Vector3& other);
    Vector3& operator/=(const_value_type value);

    bool operator==(const Vector3& other) const;
    bool operator!=(const Vector3& other) const;

    value_type Magnitude() const;
    value_type Length() const;
    value_type LengthSqr() const;
    Vector3 Normalize() const;

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

#pragma region Vector3 Helper Fns

  Vector3 operator+(const Vector3& point_a, const Vector3& point_b);
  Vector3 operator+(Vector3::const_value_type value, const Vector3& point);
  Vector3 operator+(const Vector3& point, Vector3::const_value_type value);

  Vector3 operator-(const Vector3& point_a, const Vector3& point_b);
  Vector3 operator-(Vector3::const_value_type value, const Vector3& point);
  Vector3 operator-(const Vector3& point, Vector3::const_value_type value);

  //Vector3 operator*(const Vector3& point_a, const Vector3& point_b);
  Vector3::value_type Dot(const Vector3& point_a, const Vector3& point_b);
  Vector3 operator*(Vector3::const_value_type value, const Vector3& point);
  Vector3 operator*(const Vector3& point, Vector3::const_value_type value);

  //Vector3 operator/(const Vector3& point, const_value_type value);
  Vector3 operator/(Vector3::const_value_type value, const Vector3& point);
  Vector3 operator/(const Vector3& point, Vector3::const_value_type value);

  Vector3 Cross(const Vector3& a, const Vector3& b);

  std::istream& operator>>(std::istream& is, Vector3& point);
  std::ostream& operator<<(std::ostream& os, const Vector3& point);

#pragma endregion

}