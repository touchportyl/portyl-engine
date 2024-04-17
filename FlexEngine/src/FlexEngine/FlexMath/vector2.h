#pragma once

#include "values.h"

#include "Reflection/base.h"

#include <cmath> // std::cos, std::sin, std::sqrt
#include <iostream> // std::istream, std::ostream

namespace FlexEngine
{

  struct Vector2
  { FLX_REFL_SERIALIZABLE

    // Using directives

    using value_type = float;
    using const_value_type = const value_type;
    using size_type = std::size_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = iterator;
    using const_reverse_iterator = const_iterator;

    // Data members

    union
    {
    #pragma warning(push)
    #pragma warning(disable: 4201) // nonstandard extension used: nameless struct/union
      struct { value_type x, y; };
      struct { value_type u, v; };
      struct { value_type first, second; };
    #pragma warning(pop)
      value_type data[2];
    };

#pragma region Standard Functions

    // Shorthand, follows Unity conventions

    static const Vector2 Zero;
    static const Vector2 One;
    static const Vector2 Up;
    static const Vector2 Down;
    static const Vector2 Left;
    static const Vector2 Right;

    // Conversion operators

    operator bool() const;

    // Stringify the vector

    std::string ToString() const;
    operator std::string() const;

#ifdef _DEBUG
    void Dump() const;
#endif

#pragma endregion

#pragma region Constructors

    Vector2(value_type _x = 0.0f, value_type _y = 0.0f);
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
    Vector2& operator+=(const value_type value);

    Vector2& operator-=(const Vector2& other);
    Vector2& operator-=(const value_type value);

    Vector2& operator*=(const Vector2& other);
    Vector2& operator*=(const value_type value);

    Vector2& operator/=(const Vector2& other);
    Vector2& operator/=(const value_type& value);

    bool operator==(const Vector2& other) const;
    bool operator!=(const Vector2& other) const;

    Vector2 RotateRad(const value_type radians) const;
    Vector2 RotateDeg(const value_type degrees) const;

    value_type Magnitude() const;
    value_type Length() const;
    value_type LengthSqr() const;
    Vector2 Normalize() const;

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

#pragma region Vector2 Helper Fns

  Vector2 operator+(const Vector2& point_a, const Vector2& point_b);
  Vector2 operator+(Vector2::const_value_type value, const Vector2& point);
  Vector2 operator+(const Vector2& point, Vector2::const_value_type value);

  Vector2 operator-(const Vector2& point_a, const Vector2& point_b);
  Vector2 operator-(Vector2::const_value_type value, const Vector2& point);
  Vector2 operator-(const Vector2& point, Vector2::const_value_type value);

  //Vector2 operator*(const Vector2& point_a, const Vector2& point_b);

  // Dot product of two vectors
  Vector2::value_type Dot(const Vector2& a, const Vector2& b);
  Vector2 operator*(const Vector2& point, Vector2::const_value_type value);
  Vector2 operator*(Vector2::const_value_type value, const Vector2& point);

  //Vector2 operator/(const Vector2& point_a, const Vector2& point_b);
  Vector2 operator/(Vector2::const_value_type value, const Vector2& point);
  Vector2 operator/(const Vector2& point, Vector2::const_value_type value);

  // Cross product of two vectors
  // Two crossed vectors return a scalar
  Vector2::value_type Cross(const Vector2& a, const Vector2& b);
  // Cross product of a vector with a scalar
  // with a vector v and scalar a, both returning a vector
  Vector2 Cross(const Vector2& v, Vector2::value_type a);
  // Cross product of a vector with a scalar
  // with a vector v and scalar a, both returning a vector
  Vector2 Cross(Vector2::value_type a, const Vector2& v);
  Vector2::value_type Distance(const Vector2& a, const Vector2& b);

  std::istream& operator>>(std::istream& is, Vector2& point);
  std::ostream& operator<<(std::ostream& os, const Vector2& point);

#pragma endregion

}