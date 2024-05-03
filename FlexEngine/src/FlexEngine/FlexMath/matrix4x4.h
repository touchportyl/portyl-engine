#pragma once

#include "flx_api.h"

#include "vector4.h"

namespace FlexEngine
{

  struct __FLX_API Matrix4x4
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

    union __FLX_ALIGN_AS(64)
    {
      #pragma warning(push)
      #pragma warning(disable: 4201) // nonstandard extension used: nameless struct/union
      struct
      {
        value_type m00, m01, m02, m03;
        value_type m10, m11, m12, m13;
        value_type m20, m21, m22, m23;
        value_type m30, m31, m32, m33;
      };
      #pragma warning(pop)
      value_type data[16];
    };

#pragma region Standard Functions

    // Shorthand

    static const Matrix4x4 Zero;
    static const Matrix4x4 Identity;

    // Conversion operators


    // Stringify the matrix

    std::string ToString() const;
    std::string ToPrettyString() const; // Pretty print
    operator std::string() const;

#ifdef _DEBUG
    void Dump() const;
#endif

#pragma endregion

#pragma region Constructors

    Matrix4x4(
      value_type _m00 = 0.0f, value_type _m01 = 0.0f, value_type _m02 = 0.0f, value_type _m03 = 0.0f,
      value_type _m10 = 0.0f, value_type _m11 = 0.0f, value_type _m12 = 0.0f, value_type _m13 = 0.0f,
      value_type _m20 = 0.0f, value_type _m21 = 0.0f, value_type _m22 = 0.0f, value_type _m23 = 0.0f,
      value_type _m30 = 0.0f, value_type _m31 = 0.0f, value_type _m32 = 0.0f, value_type _m33 = 0.0f
    );
    Matrix4x4(const Matrix4x4& other);

#pragma endregion

#pragma region Operator Overloading

    // - =
    // += -= *= /=
    // == !=
    // + - * /

    //Matrix4x4 operator-() const;

    Matrix4x4& operator=(const Matrix4x4& other);

    Matrix4x4& operator+=(const Matrix4x4& other);
    //Matrix4x4& operator+=(const_value_type value);

    Matrix4x4& operator-=(const Matrix4x4& other);
    //Matrix4x4& operator-=(const_value_type value);

    Matrix4x4& operator*=(const Matrix4x4& other);
    Matrix4x4& operator*=(const_value_type& value);

    //Matrix4x4& operator/=(const Matrix4x4& other);
    Matrix4x4& operator/=(const_value_type value);

    bool operator==(const Matrix4x4& other) const;
    bool operator!=(const Matrix4x4& other) const;

    Matrix4x4 Transpose() const;
    value_type Determinant() const;
    Matrix4x4 Inverse() const;

#pragma endregion

#pragma region Passthrough Functions

    // Accessors

    reference at(const size_type index);
    const_reference at(const size_type index) const;
    reference operator[](const size_type index);
    const_reference operator[](const size_type index) const;

    // Special Matrix Accessors

    reference at(const size_type row, const size_type column);
    const_reference at(const size_type row, const size_type column) const;
    reference operator()(const size_type row, const size_type column);
    const_reference operator()(const size_type row, const size_type column) const;

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

    constexpr size_type size() const;

#pragma endregion

  };

#pragma region Matrix4x4 Helper Fns

  Matrix4x4 operator+(const Matrix4x4& matrix_a, const Matrix4x4& matrix_b);
  //Matrix4x4 operator+(Matrix4x4::const_value_type value, const Matrix4x4& matrix);
  //Matrix4x4 operator+(const Matrix4x4& matrix, Matrix4x4::const_value_type value);

  Matrix4x4 operator-(const Matrix4x4& matrix_a, const Matrix4x4& matrix_b);
  //Matrix4x4 operator-(Matrix4x4::const_value_type value, const Matrix4x4& matrix);
  //Matrix4x4 operator-(const Matrix4x4& matrix, Matrix4x4::const_value_type value);

  Matrix4x4 operator*(const Matrix4x4& matrix_a, const Matrix4x4& matrix_b);
  Matrix4x4 operator*(Matrix4x4::const_value_type value, const Matrix4x4& matrix);
  Matrix4x4 operator*(const Matrix4x4& matrix, Matrix4x4::const_value_type value);

  //Matrix4x4 operator/(const Matrix4x4& matrix_a, const Matrix4x4& matrix_b);
  //Matrix4x4 operator/(Matrix4x4::const_value_type value, const Matrix4x4& matrix);
  Matrix4x4 operator/(const Matrix4x4& matrix, Matrix4x4::const_value_type value);

  std::istream& operator>>(std::istream& is, Matrix4x4& matrix);
  std::ostream& operator<<(std::ostream& os, const Matrix4x4& matrix);

#pragma endregion

}