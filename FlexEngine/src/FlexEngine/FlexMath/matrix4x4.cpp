#include "matrix4x4.h"

namespace FlexEngine
{

  #pragma region Reflection

  FLX_REFL_REGISTER_START(Matrix4x4)
    FLX_REFL_REGISTER_PROPERTY(m00)
    FLX_REFL_REGISTER_PROPERTY(m01)
    FLX_REFL_REGISTER_PROPERTY(m02)
    FLX_REFL_REGISTER_PROPERTY(m03)
    FLX_REFL_REGISTER_PROPERTY(m10)
    FLX_REFL_REGISTER_PROPERTY(m11)
    FLX_REFL_REGISTER_PROPERTY(m12)
    FLX_REFL_REGISTER_PROPERTY(m13)
    FLX_REFL_REGISTER_PROPERTY(m20)
    FLX_REFL_REGISTER_PROPERTY(m21)
    FLX_REFL_REGISTER_PROPERTY(m22)
    FLX_REFL_REGISTER_PROPERTY(m23)
    FLX_REFL_REGISTER_PROPERTY(m30)
    FLX_REFL_REGISTER_PROPERTY(m31)
    FLX_REFL_REGISTER_PROPERTY(m32)
    FLX_REFL_REGISTER_PROPERTY(m33)
  FLX_REFL_REGISTER_END;

  #pragma endregion

  #pragma region Standard Functions

  const Matrix4x4 Matrix4x4::Zero = {
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0
  };

  const Matrix4x4 Matrix4x4::Identity = {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
  };

  std::string Matrix4x4::ToString() const
  {
    std::string str = "(";
    for (size_type i = 0; i < 16; ++i)
    {
      str += std::to_string(data[i]);
      str += ", ";
    }
    str.pop_back();
    str.pop_back();
    str += ")";
    return str;
  }
  std::string Matrix4x4::ToPrettyString() const
  {
    std::string str = "";
    for (size_type i = 0; i < 4; ++i)
    {
      str += "(";
      for (size_type j = 0; j < 4; ++j)
      {
        str += std::to_string(data[i * 4 + j]);
        str += ", ";
      }
      str.pop_back();
      str.pop_back();
      str += ")\n";
    }
    return str;
  }
  Matrix4x4::operator std::string() const { return ToString(); }

#ifdef _DEBUG
  void Matrix4x4::Dump() const { Log::Debug(ToPrettyString()); }
#endif

  #pragma endregion

  #pragma region Constructors

  Matrix4x4::Matrix4x4(
    value_type _m00, value_type _m01, value_type _m02, value_type _m03,
    value_type _m10, value_type _m11, value_type _m12, value_type _m13,
    value_type _m20, value_type _m21, value_type _m22, value_type _m23,
    value_type _m30, value_type _m31, value_type _m32, value_type _m33
  )
  {
    m00 = _m00; m01 = _m01; m02 = _m02; m03 = _m03;
    m10 = _m10; m11 = _m11; m12 = _m12; m13 = _m13;
    m20 = _m20; m21 = _m21; m22 = _m22; m23 = _m23;
    m30 = _m30; m31 = _m31; m32 = _m32; m33 = _m33;
  }

  Matrix4x4::Matrix4x4(const Matrix4x4& other)
  {
    m00 = other.m00; m01 = other.m01; m02 = other.m02; m03 = other.m03;
    m10 = other.m10; m11 = other.m11; m12 = other.m12; m13 = other.m13;
    m20 = other.m20; m21 = other.m21; m22 = other.m22; m23 = other.m23;
    m30 = other.m30; m31 = other.m31; m32 = other.m32; m33 = other.m33;
  }

  #pragma endregion

  #pragma region Operator Overloading

  // - =
  // += -= *= /=
  // == !=
  // + - * /

  //Matrix4x4 Matrix4x4::operator-() const;

  Matrix4x4& Matrix4x4::operator=(const Matrix4x4& other)
  {
    m00 = other.m00; m01 = other.m01; m02 = other.m02; m03 = other.m03;
    m10 = other.m10; m11 = other.m11; m12 = other.m12; m13 = other.m13;
    m20 = other.m20; m21 = other.m21; m22 = other.m22; m23 = other.m23;
    m30 = other.m30; m31 = other.m31; m32 = other.m32; m33 = other.m33;
    return *this;
  }

  Matrix4x4& Matrix4x4::operator+=(const Matrix4x4& other)
  {
    m00 += other.m00; m01 += other.m01; m02 += other.m02; m03 += other.m03;
    m10 += other.m10; m11 += other.m11; m12 += other.m12; m13 += other.m13;
    m20 += other.m20; m21 += other.m21; m22 += other.m22; m23 += other.m23;
    m30 += other.m30; m31 += other.m31; m32 += other.m32; m33 += other.m33;
    return *this;
  }

  //Matrix4x4& Matrix4x4::operator+=(const_value_type value);

  Matrix4x4& Matrix4x4::operator-=(const Matrix4x4& other)
  {
    m00 -= other.m00; m01 -= other.m01; m02 -= other.m02; m03 -= other.m03;
    m10 -= other.m10; m11 -= other.m11; m12 -= other.m12; m13 -= other.m13;
    m20 -= other.m20; m21 -= other.m21; m22 -= other.m22; m23 -= other.m23;
    m30 -= other.m30; m31 -= other.m31; m32 -= other.m32; m33 -= other.m33;
    return *this;
  }

  //Matrix4x4& Matrix4x4::operator-=(const_value_type value);

  Matrix4x4& Matrix4x4::operator*=(const Matrix4x4& other)
  {
    Matrix4x4 result;

    result.m00 = m00 * other.m00 + m01 * other.m10 + m02 * other.m20 + m03 * other.m30;
    result.m01 = m00 * other.m01 + m01 * other.m11 + m02 * other.m21 + m03 * other.m31;
    result.m02 = m00 * other.m02 + m01 * other.m12 + m02 * other.m22 + m03 * other.m32;
    result.m03 = m00 * other.m03 + m01 * other.m13 + m02 * other.m23 + m03 * other.m33;

    result.m10 = m10 * other.m00 + m11 * other.m10 + m12 * other.m20 + m13 * other.m30;
    result.m11 = m10 * other.m01 + m11 * other.m11 + m12 * other.m21 + m13 * other.m31;
    result.m12 = m10 * other.m02 + m11 * other.m12 + m12 * other.m22 + m13 * other.m32;
    result.m13 = m10 * other.m03 + m11 * other.m13 + m12 * other.m23 + m13 * other.m33;

    result.m20 = m20 * other.m00 + m21 * other.m10 + m22 * other.m20 + m23 * other.m30;
    result.m21 = m20 * other.m01 + m21 * other.m11 + m22 * other.m21 + m23 * other.m31;
    result.m22 = m20 * other.m02 + m21 * other.m12 + m22 * other.m22 + m23 * other.m32;
    result.m23 = m20 * other.m03 + m21 * other.m13 + m22 * other.m23 + m23 * other.m33;

    result.m30 = m30 * other.m00 + m31 * other.m10 + m32 * other.m20 + m33 * other.m30;
    result.m31 = m30 * other.m01 + m31 * other.m11 + m32 * other.m21 + m33 * other.m31;
    result.m32 = m30 * other.m02 + m31 * other.m12 + m32 * other.m22 + m33 * other.m32;
    result.m33 = m30 * other.m03 + m31 * other.m13 + m32 * other.m23 + m33 * other.m33;

    return *this = result;
  }

  Matrix4x4& Matrix4x4::operator*=(const_value_type& value)
  {
    m00 *= value; m01 *= value; m02 *= value; m03 *= value;
    m10 *= value; m11 *= value; m12 *= value; m13 *= value;
    m20 *= value; m21 *= value; m22 *= value; m23 *= value;
    m30 *= value; m31 *= value; m32 *= value; m33 *= value;
    return *this;
  }

  //Matrix4x4& Matrix4x4::operator/=(const Matrix4x4& other);

  Matrix4x4& Matrix4x4::operator/=(const_value_type value)
  {
    m00 /= value; m01 /= value; m02 /= value; m03 /= value;
    m10 /= value; m11 /= value; m12 /= value; m13 /= value;
    m20 /= value; m21 /= value; m22 /= value; m23 /= value;
    m30 /= value; m31 /= value; m32 /= value; m33 /= value;
    return *this;
  }

  bool Matrix4x4::operator==(const Matrix4x4& other) const
  {
    return
      m00 == other.m00 && m01 == other.m01 && m02 == other.m02 && m03 == other.m03 &&
      m10 == other.m10 && m11 == other.m11 && m12 == other.m12 && m13 == other.m13 &&
      m20 == other.m20 && m21 == other.m21 && m22 == other.m22 && m23 == other.m23 &&
      m30 == other.m30 && m31 == other.m31 && m32 == other.m32 && m33 == other.m33
    ;
  }

  bool Matrix4x4::operator!=(const Matrix4x4& other) const
  {
    return !(*this == other);
  }

  Matrix4x4 Matrix4x4::Transpose() const
  {
    return {
      m00, m10, m20, m30,
      m01, m11, m21, m31,
      m02, m12, m22, m32,
      m03, m13, m23, m33
    };
  }

  Matrix4x4::value_type Matrix4x4::Determinant() const
  {
    return
      m00 * m11 * m22 * m33 + m00 * m12 * m23 * m31 + m00 * m13 * m21 * m32 +
      m01 * m10 * m23 * m32 + m01 * m12 * m20 * m33 + m01 * m13 * m22 * m30 +
      m02 * m10 * m21 * m33 + m02 * m11 * m23 * m30 + m02 * m13 * m20 * m31 +
      m03 * m10 * m22 * m31 + m03 * m11 * m20 * m32 + m03 * m12 * m21 * m30 -
      m00 * m11 * m23 * m32 - m00 * m12 * m21 * m33 - m00 * m13 * m22 * m31 -
      m01 * m10 * m22 * m33 - m01 * m12 * m23 * m30 - m01 * m13 * m20 * m32 -
      m02 * m10 * m23 * m31 - m02 * m11 * m20 * m33 - m02 * m13 * m21 * m30 -
      m03 * m10 * m21 * m32 - m03 * m11 * m22 * m30 - m03 * m12 * m20 * m31
    ;
  }

  Matrix4x4 Matrix4x4::Inverse() const
  {
    Matrix4x4 result;

    value_type det = Determinant();
    if (det == 0) return result;

    result.m00 = m11 * m22 * m33 + m12 * m23 * m31 + m13 * m21 * m32 - m11 * m23 * m32 - m12 * m21 * m33 - m13 * m22 * m31;
    result.m01 = m01 * m23 * m32 + m02 * m21 * m33 + m03 * m22 * m31 - m01 * m22 * m33 - m02 * m23 * m31 - m03 * m21 * m32;
    result.m02 = m01 * m12 * m33 + m02 * m13 * m31 + m03 * m11 * m32 - m01 * m13 * m32 - m02 * m11 * m33 - m03 * m12 * m31;
    result.m03 = m01 * m13 * m22 + m02 * m11 * m23 + m03 * m12 * m21 - m01 * m12 * m23 - m02 * m13 * m21 - m03 * m11 * m22;

    result.m10 = m10 * m23 * m32 + m12 * m20 * m33 + m13 * m22 * m30 - m10 * m22 * m33 - m12 * m23 * m30 - m13 * m20 * m32;
    result.m11 = m00 * m22 * m33 + m02 * m23 * m30 + m03 * m20 * m32 - m00 * m23 * m32 - m02 * m20 * m33 - m03 * m22 * m30;
    result.m12 = m00 * m13 * m32 + m02 * m10 * m33 + m03 * m12 * m30 - m00 * m12 * m33 - m02 * m13 * m30 - m03 * m10 * m32;
    result.m13 = m00 * m12 * m23 + m02 * m13 * m20 + m03 * m10 * m22 - m00 * m13 * m22 - m02 * m10 * m23 - m03 * m12 * m20;

    result.m20 = m10 * m21 * m33 + m11 * m23 * m30 + m13 * m20 * m31 - m10 * m23 * m31 - m11 * m20 * m33 - m13 * m21 * m30;
    result.m21 = m00 * m23 * m31 + m01 * m20 * m33 + m03 * m21 * m30 - m00 * m21 * m33 - m01 * m23 * m30 - m03 * m20 * m31;
    result.m22 = m00 * m11 * m33 + m01 * m13 * m30 + m03 * m10 * m31 - m00 * m13 * m31 - m01 * m10 * m33 - m03 * m11 * m30;
    result.m23 = m00 * m13 * m21 + m01 * m10 * m23 + m03 * m11 * m20 - m00 * m11 * m23 - m01 * m13 * m20 - m03 * m10 * m21;

    result.m30 = m10 * m22 * m31 + m11 * m20 * m32 + m12 * m21 * m30 - m10 * m21 * m32 - m11 * m22 * m30 - m12 * m20 * m31;
    result.m31 = m00 * m21 * m32 + m01 * m22 * m30 + m02 * m20 * m31 - m00 * m22 * m31 - m01 * m20 * m32 - m02 * m21 * m30;
    result.m32 = m00 * m12 * m31 + m01 * m10 * m32 + m02 * m11 * m30 - m00 * m11 * m32 - m01 * m12 * m30 - m02 * m10 * m31;
    result.m33 = m00 * m11 * m22 + m01 * m12 * m20 + m02 * m10 * m21 - m00 * m12 * m21 - m01 * m10 * m22 - m02 * m11 * m20;

    return result / det;
  }

  #pragma endregion

  #pragma region Passthrough Functions

  Matrix4x4::reference Matrix4x4::at(const Matrix4x4::size_type index) { return data[index]; }
  Matrix4x4::const_reference Matrix4x4::at(const Matrix4x4::size_type index) const { return data[index]; }
  Matrix4x4::reference Matrix4x4::operator[](const Matrix4x4::size_type index) { return at(index); }
  Matrix4x4::const_reference Matrix4x4::operator[](const Matrix4x4::size_type index) const { return at(index); }

  Matrix4x4::reference Matrix4x4::at(const Matrix4x4::size_type row, const Matrix4x4::size_type column) { return data[row * 4 + column]; }
  Matrix4x4::const_reference Matrix4x4::at(const Matrix4x4::size_type row, const Matrix4x4::size_type column) const { return data[row * 4 + column]; }
  Matrix4x4::reference Matrix4x4::operator()(const Matrix4x4::size_type row, const Matrix4x4::size_type column) { return at(row, column); }
  Matrix4x4::const_reference Matrix4x4::operator()(const Matrix4x4::size_type row, const Matrix4x4::size_type column) const { return at(row, column); }

  Matrix4x4::iterator                 Matrix4x4::begin() { return data; }
  Matrix4x4::const_iterator           Matrix4x4::begin() const { return data; }
  Matrix4x4::const_iterator           Matrix4x4::cbegin() const { return data; }
  Matrix4x4::iterator                 Matrix4x4::end() { return data + size(); }
  Matrix4x4::const_iterator           Matrix4x4::end() const { return data + size(); }
  Matrix4x4::const_iterator           Matrix4x4::cend() const { return data + size(); }
  //Matrix4x4::reverse_iterator         Matrix4x4::rbegin()         { return data + size() - 1; }
  //Matrix4x4::const_reverse_iterator   Matrix4x4::rbegin() const   { return data + size() - 1; }
  //Matrix4x4::const_reverse_iterator   Matrix4x4::crbegin() const  { return data + size() - 1; }
  //Matrix4x4::reverse_iterator         Matrix4x4::rend()           { return data - 1; }
  //Matrix4x4::const_reverse_iterator   Matrix4x4::rend() const     { return data - 1; }
  //Matrix4x4::const_reverse_iterator   Matrix4x4::crend() const    { return data - 1; }

  constexpr Matrix4x4::size_type Matrix4x4::size() const { return 16; }

  #pragma endregion

  #pragma region Vector3 Helper Fns

  Matrix4x4 operator+(const Matrix4x4& matrix_a, const Matrix4x4& matrix_b)
  {
    return {
      matrix_a.m00 + matrix_b.m00, matrix_a.m01 + matrix_b.m01, matrix_a.m02 + matrix_b.m02, matrix_a.m03 + matrix_b.m03,
      matrix_a.m10 + matrix_b.m10, matrix_a.m11 + matrix_b.m11, matrix_a.m12 + matrix_b.m12, matrix_a.m13 + matrix_b.m13,
      matrix_a.m20 + matrix_b.m20, matrix_a.m21 + matrix_b.m21, matrix_a.m22 + matrix_b.m22, matrix_a.m23 + matrix_b.m23,
      matrix_a.m30 + matrix_b.m30, matrix_a.m31 + matrix_b.m31, matrix_a.m32 + matrix_b.m32, matrix_a.m33 + matrix_b.m33
    };
  }

  //Matrix4x4 operator+(Matrix4x4::const_value_type value, const Matrix4x4& matrix);
  
  //Matrix4x4 operator+(const Matrix4x4& matrix, Matrix4x4::const_value_type value);

  Matrix4x4 operator-(const Matrix4x4& matrix_a, const Matrix4x4& matrix_b)
  {
    return {
      matrix_a.m00 - matrix_b.m00, matrix_a.m01 - matrix_b.m01, matrix_a.m02 - matrix_b.m02, matrix_a.m03 - matrix_b.m03,
      matrix_a.m10 - matrix_b.m10, matrix_a.m11 - matrix_b.m11, matrix_a.m12 - matrix_b.m12, matrix_a.m13 - matrix_b.m13,
      matrix_a.m20 - matrix_b.m20, matrix_a.m21 - matrix_b.m21, matrix_a.m22 - matrix_b.m22, matrix_a.m23 - matrix_b.m23,
      matrix_a.m30 - matrix_b.m30, matrix_a.m31 - matrix_b.m31, matrix_a.m32 - matrix_b.m32, matrix_a.m33 - matrix_b.m33
    };
  }

  //Matrix4x4 operator-(Matrix4x4::const_value_type value, const Matrix4x4& matrix);
  
  //Matrix4x4 operator-(const Matrix4x4& matrix, Matrix4x4::const_value_type value);

  Matrix4x4 operator*(const Matrix4x4& matrix_a, const Matrix4x4& matrix_b)
  {
    return {
      matrix_a.m00 * matrix_b.m00 + matrix_a.m01 * matrix_b.m10 + matrix_a.m02 * matrix_b.m20 + matrix_a.m03 * matrix_b.m30,
      matrix_a.m00 * matrix_b.m01 + matrix_a.m01 * matrix_b.m11 + matrix_a.m02 * matrix_b.m21 + matrix_a.m03 * matrix_b.m31,
      matrix_a.m00 * matrix_b.m02 + matrix_a.m01 * matrix_b.m12 + matrix_a.m02 * matrix_b.m22 + matrix_a.m03 * matrix_b.m32,
      matrix_a.m00 * matrix_b.m03 + matrix_a.m01 * matrix_b.m13 + matrix_a.m02 * matrix_b.m23 + matrix_a.m03 * matrix_b.m33,

      matrix_a.m10 * matrix_b.m00 + matrix_a.m11 * matrix_b.m10 + matrix_a.m12 * matrix_b.m20 + matrix_a.m13 * matrix_b.m30,
      matrix_a.m10 * matrix_b.m01 + matrix_a.m11 * matrix_b.m11 + matrix_a.m12 * matrix_b.m21 + matrix_a.m13 * matrix_b.m31,
      matrix_a.m10 * matrix_b.m02 + matrix_a.m11 * matrix_b.m12 + matrix_a.m12 * matrix_b.m22 + matrix_a.m13 * matrix_b.m32,
      matrix_a.m10 * matrix_b.m03 + matrix_a.m11 * matrix_b.m13 + matrix_a.m12 * matrix_b.m23 + matrix_a.m13 * matrix_b.m33,

      matrix_a.m20 * matrix_b.m00 + matrix_a.m21 * matrix_b.m10 + matrix_a.m22 * matrix_b.m20 + matrix_a.m23 * matrix_b.m30,
      matrix_a.m20 * matrix_b.m01 + matrix_a.m21 * matrix_b.m11 + matrix_a.m22 * matrix_b.m21 + matrix_a.m23 * matrix_b.m31,
      matrix_a.m20 * matrix_b.m02 + matrix_a.m21 * matrix_b.m12 + matrix_a.m22 * matrix_b.m22 + matrix_a.m23 * matrix_b.m32,
      matrix_a.m20 * matrix_b.m03 + matrix_a.m21 * matrix_b.m13 + matrix_a.m22 * matrix_b.m23 + matrix_a.m23 * matrix_b.m33,

      matrix_a.m30 * matrix_b.m00 + matrix_a.m31 * matrix_b.m10 + matrix_a.m32 * matrix_b.m20 + matrix_a.m33 * matrix_b.m30,
      matrix_a.m30 * matrix_b.m01 + matrix_a.m31 * matrix_b.m11 + matrix_a.m32 * matrix_b.m21 + matrix_a.m33 * matrix_b.m31,
      matrix_a.m30 * matrix_b.m02 + matrix_a.m31 * matrix_b.m12 + matrix_a.m32 * matrix_b.m22 + matrix_a.m33 * matrix_b.m32,
      matrix_a.m30 * matrix_b.m03 + matrix_a.m31 * matrix_b.m13 + matrix_a.m32 * matrix_b.m23 + matrix_a.m33 * matrix_b.m33
    };
  }

  Matrix4x4 operator*(Matrix4x4::const_value_type value, const Matrix4x4& matrix)
  {
    return {
      matrix.m00 * value, matrix.m01 * value, matrix.m02 * value, matrix.m03 * value,
      matrix.m10 * value, matrix.m11 * value, matrix.m12 * value, matrix.m13 * value,
      matrix.m20 * value, matrix.m21 * value, matrix.m22 * value, matrix.m23 * value,
      matrix.m30 * value, matrix.m31 * value, matrix.m32 * value, matrix.m33 * value
    };
  }

  Matrix4x4 operator*(const Matrix4x4& matrix, Matrix4x4::const_value_type value)
  {
    return value * matrix;
  }

  //Matrix4x4 operator/(const Matrix4x4& matrix_a, const Matrix4x4& matrix_b);
  
  //Matrix4x4 operator/(Matrix4x4::const_value_type value, const Matrix4x4& matrix);

  Matrix4x4 operator/(const Matrix4x4& matrix, Matrix4x4::const_value_type value)
  {
    if (value == 0) return Matrix4x4::Zero;
    return {
      matrix.m00 / value, matrix.m01 / value, matrix.m02 / value, matrix.m03 / value,
      matrix.m10 / value, matrix.m11 / value, matrix.m12 / value, matrix.m13 / value,
      matrix.m20 / value, matrix.m21 / value, matrix.m22 / value, matrix.m23 / value,
      matrix.m30 / value, matrix.m31 / value, matrix.m32 / value, matrix.m33 / value
    };
  }

  std::istream& operator>>(std::istream& is, Matrix4x4& matrix)
  {
    for (Matrix4x4::size_type i = 0; i < matrix.size(); ++i)
    {
      is >> matrix.data[i];
    }
    return is;
  }

  std::ostream& operator<<(std::ostream& os, const Matrix4x4& matrix)
  {
    for (Matrix4x4::size_type i = 0; i < matrix.size(); ++i)
    {
      os << matrix.data[i];
      if (i < matrix.size() - 1) os << " ";
    }
    return os;
  }

  #pragma endregion

}