#pragma once

#include "vector3.h"

namespace FlexEngine
{

  struct Vector4
  {
    union
    {
      float x, y, z, w;
      float r, g, b, a;
      float first, second, third, fourth;
    };

#pragma region Constructors

    Vector4(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f, float _w = 0.0f);
    Vector4(const Vector4& other);
    Vector4(const Vector3& xyz, float _w = 0.0f);
    Vector4(const Vector2& xy, float _z = 0.0f, float _w = 0.0f);

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
    Vector4& operator+=(const float value);

    Vector4& operator-=(const Vector4& other);
    Vector4& operator-=(const float value);

    Vector4& operator*=(const Vector4& other);
    Vector4& operator*=(const float& value);

    Vector4& operator/=(const Vector4& other);
    Vector4& operator/=(const float value);

    bool operator==(const Vector4& other) const;
    bool operator!=(const Vector4& other) const;

    // Note: Returns the magnitude of the xyz component
    float Magnitude() const;

    // Note: Returns the length of the xyz component
    float Length() const;

    // Note: Returns the length of the xyz component
    float LengthSqr() const;

    // Note: Returns the normalized xyz component
    Vector4 Normalize() const;

#pragma endregion

  };

#pragma region Vector4 Helper Fns

  Vector4 operator+(const Vector4& point_a, const Vector4& point_b);
  Vector4 operator+(const float value, const Vector4& point);
  Vector4 operator+(const Vector4& point, const float value);

  Vector4 operator-(const Vector4& point_a, const Vector4& point_b);
  Vector4 operator-(const float value, const Vector4& point);
  Vector4 operator-(const Vector4& point, const float value);

  //Vector4 operator*(const Vector4& point_a, const Vector4& point_b);
  Vector4 operator*(const float value, const Vector4& point);
  Vector4 operator*(const Vector4& point, const float value);

  //Vector4 operator/(const Vector4& point_a, const Vector4& point_b);
  Vector4 operator/(const float value, const Vector4& point);
  Vector4 operator/(const Vector4& point, const float value);

  std::istream& operator>>(std::istream& is, Vector4& point);
  std::ostream& operator<<(std::ostream& os, const Vector4& point);

#pragma endregion

}