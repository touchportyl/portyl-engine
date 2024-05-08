#include "pch.h"

#include "CppUnitTest.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <FlexEngine.h>
using namespace FlexEngine;


#pragma region Helper Functions

#pragma warning(push)
#pragma warning(disable: 4505) // unreferenced function with internal linkage has been removed


// Special functions for comparing Vector# and glm::vec#
#pragma region AreEqualVector

static void AreEqualVector(const Vector2& expected, const Vector2& actual)
{
  Assert::AreEqual(expected.x, actual.x);
  Assert::AreEqual(expected.y, actual.y);
}
static void AreEqualVector(const Vector3& expected, const Vector3& actual)
{
  Assert::AreEqual(expected.x, actual.x);
  Assert::AreEqual(expected.y, actual.y);
  Assert::AreEqual(expected.z, actual.z);
}
static void AreEqualVector(const Vector4& expected, const Vector4& actual)
{
  Assert::AreEqual(expected.x, actual.x);
  Assert::AreEqual(expected.y, actual.y);
  Assert::AreEqual(expected.z, actual.z);
  Assert::AreEqual(expected.w, actual.w);
}
static void AreEqualVector(const glm::vec2& expected, const Vector2& actual)
{
  Assert::AreEqual(expected.x, actual.x);
  Assert::AreEqual(expected.y, actual.y);
}
static void AreEqualVector(const glm::vec3& expected, const Vector3& actual)
{
  Assert::AreEqual(expected.x, actual.x);
  Assert::AreEqual(expected.y, actual.y);
  Assert::AreEqual(expected.z, actual.z);
}
static void AreEqualVector(const glm::vec4& expected, const Vector4& actual)
{
  Assert::AreEqual(expected.x, actual.x);
  Assert::AreEqual(expected.y, actual.y);
  Assert::AreEqual(expected.z, actual.z);
  Assert::AreEqual(expected.w, actual.w);
}

#pragma endregion

#pragma region AreEqualVector (with float tolerance)

static void AreEqualVector(const Vector2& expected, const Vector2& actual, const float tolerance)
{
  Assert::AreEqual(expected.x, actual.x, tolerance);
  Assert::AreEqual(expected.y, actual.y, tolerance);
}
static void AreEqualVector(const Vector3& expected, const Vector3& actual, const float tolerance)
{
  Assert::AreEqual(expected.x, actual.x, tolerance);
  Assert::AreEqual(expected.y, actual.y, tolerance);
  Assert::AreEqual(expected.z, actual.z, tolerance);
}
static void AreEqualVector(const Vector4& expected, const Vector4& actual, const float tolerance)
{
  Assert::AreEqual(expected.x, actual.x, tolerance);
  Assert::AreEqual(expected.y, actual.y, tolerance);
  Assert::AreEqual(expected.z, actual.z, tolerance);
  Assert::AreEqual(expected.w, actual.w, tolerance);
}
static void AreEqualVector(const glm::vec2& expected, const Vector2& actual, const float tolerance)
{
  Assert::AreEqual(expected.x, actual.x, tolerance);
  Assert::AreEqual(expected.y, actual.y, tolerance);
}
static void AreEqualVector(const glm::vec3& expected, const Vector3& actual, const float tolerance)
{
  Assert::AreEqual(expected.x, actual.x, tolerance);
  Assert::AreEqual(expected.y, actual.y, tolerance);
  Assert::AreEqual(expected.z, actual.z, tolerance);
}
static void AreEqualVector(const glm::vec4& expected, const Vector4& actual, const float tolerance)
{
  Assert::AreEqual(expected.x, actual.x, tolerance);
  Assert::AreEqual(expected.y, actual.y, tolerance);
  Assert::AreEqual(expected.z, actual.z, tolerance);
  Assert::AreEqual(expected.w, actual.w, tolerance);
}

#pragma endregion


// Special functions for comparing Matrix4x4 and glm::mat4
#pragma region AreEqualMatrix

static void AreEqualMatrix(const Matrix4x4& expected, const Matrix4x4& actual)
{
  for (int i = 0; i < 16; i++)
  {
    Assert::AreEqual(expected.data[i], actual.data[i]);
  }
}

static void AreEqualMatrix(const glm::mat4& expected, const Matrix4x4& actual)
{
  for (int i = 0; i < 16; i++)
  {
    Assert::AreEqual(expected[i / 4][i % 4], actual.data[i]);
  }
}

#pragma endregion

#pragma region AreEqualMatrix (with float tolerance)

static void AreEqualMatrix(const Matrix4x4& expected, const Matrix4x4& actual, const float tolerance)
{
  for (int i = 0; i < 16; i++)
  {
    Assert::AreEqual(expected[i], actual[i], tolerance);
  }
}

static void AreEqualMatrix(const glm::mat4& expected, const Matrix4x4& actual, const float tolerance)
{
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      Assert::AreEqual(expected[i][j], actual(i, j), tolerance);
    }
  }
}

#pragma endregion


#pragma warning(pop)

#pragma endregion


namespace UnitTests_FlexMath
{

  namespace UnitTests_Vector3
  {

    TEST_CLASS(UnitTests_Accessors)
    {
      Vector3 a;

    public:

      TEST_METHOD_INITIALIZE(Initialize)
      {
        a = { 1.0f, 2.0f, 3.0f };
      }

      TEST_METHOD_CLEANUP(Cleanup)
      {
      }
      
      TEST_METHOD(XYZ)
      {
        Assert::AreEqual(1.0f, a.x);
        Assert::AreEqual(2.0f, a.y);
        Assert::AreEqual(3.0f, a.z);
      }

      TEST_METHOD(RGB)
      {
        Assert::AreEqual(1.0f, a.r);
        Assert::AreEqual(2.0f, a.g);
        Assert::AreEqual(3.0f, a.b);
      }

      TEST_METHOD(STP)
      {
        Assert::AreEqual(1.0f, a.s);
        Assert::AreEqual(2.0f, a.t);
        Assert::AreEqual(3.0f, a.p);
      }

      TEST_METHOD(FirstSecondThird)
      {
        Assert::AreEqual(1.0f, a.first);
        Assert::AreEqual(2.0f, a.second);
        Assert::AreEqual(3.0f, a.third);
      }

      TEST_METHOD(Array)
      {
        Assert::AreEqual(1.0f, a[0]);
        Assert::AreEqual(2.0f, a[1]);
        Assert::AreEqual(3.0f, a[2]);
      }

      TEST_METHOD(DataArray)
      {
        Assert::AreEqual(1.0f, a.data[0]);
        Assert::AreEqual(2.0f, a.data[1]);
        Assert::AreEqual(3.0f, a.data[2]);
      }

    };

    TEST_CLASS(UnitTests_ConversionOperators)
    {
      Vector3 a;

    public:

      TEST_METHOD_INITIALIZE(Initialize)
      {
        a = { 1.0f, 2.0f, 3.0f };
      }

      TEST_METHOD_CLEANUP(Cleanup)
      {
      }

      TEST_METHOD(ToVector2)
      {
        AreEqualVector(Vector2(1.0f, 2.0f), (Vector2)a);
      }

      TEST_METHOD(ToVector1)
      {
        Assert::AreEqual(1.0f, (Vector1)a);
      }

      TEST_METHOD(ToBool)
      {
        Assert::IsTrue((bool)a);
      }

      TEST_METHOD(ToBool_EmptyVector)
      {
        Assert::IsFalse((bool)Vector3(0, 0, 0));
      }

    };

    TEST_CLASS(UnitTests_Swizzling)
    {
      Vector3 a;

    public:

      TEST_METHOD_INITIALIZE(Initialize)
      {
        a = { 1.0f, 2.0f, 3.0f };
      }

      TEST_METHOD_CLEANUP(Cleanup)
      {
      }

      TEST_METHOD(Swizzling_Vector3_Same)
      {
        AreEqualVector(a, a.Swizzle());
        AreEqualVector(a, a.Swizzle("xyz"));
      }

      TEST_METHOD(Swizzling_Vector3)
      {
        AreEqualVector(Vector3(a.x, a.x, a.x), a.Swizzle("xxx"));
        AreEqualVector(Vector3(a.x, a.x, a.y), a.Swizzle("xxy"));
        AreEqualVector(Vector3(a.x, a.x, a.z), a.Swizzle("xxz"));
        AreEqualVector(Vector3(a.x, a.y, a.x), a.Swizzle("xyx"));
        AreEqualVector(Vector3(a.x, a.y, a.y), a.Swizzle("xyy"));
        AreEqualVector(Vector3(a.x, a.y, a.z), a.Swizzle("xyz"));
        AreEqualVector(Vector3(a.x, a.z, a.x), a.Swizzle("xzx"));
        AreEqualVector(Vector3(a.x, a.z, a.y), a.Swizzle("xzy"));
        AreEqualVector(Vector3(a.x, a.z, a.z), a.Swizzle("xzz"));

        AreEqualVector(Vector3(a.y, a.x, a.x), a.Swizzle("yxx"));
        AreEqualVector(Vector3(a.y, a.x, a.y), a.Swizzle("yxy"));
        AreEqualVector(Vector3(a.y, a.x, a.z), a.Swizzle("yxz"));
        AreEqualVector(Vector3(a.y, a.y, a.x), a.Swizzle("yyx"));
        AreEqualVector(Vector3(a.y, a.y, a.y), a.Swizzle("yyy"));
        AreEqualVector(Vector3(a.y, a.y, a.z), a.Swizzle("yyz"));
        AreEqualVector(Vector3(a.y, a.z, a.x), a.Swizzle("yzx"));
        AreEqualVector(Vector3(a.y, a.z, a.y), a.Swizzle("yzy"));
        AreEqualVector(Vector3(a.y, a.z, a.z), a.Swizzle("yzz"));

        AreEqualVector(Vector3(a.z, a.x, a.x), a.Swizzle("zxx"));
        AreEqualVector(Vector3(a.z, a.x, a.y), a.Swizzle("zxy"));
        AreEqualVector(Vector3(a.z, a.x, a.z), a.Swizzle("zxz"));
        AreEqualVector(Vector3(a.z, a.y, a.x), a.Swizzle("zyx"));
        AreEqualVector(Vector3(a.z, a.y, a.y), a.Swizzle("zyy"));
        AreEqualVector(Vector3(a.z, a.y, a.z), a.Swizzle("zyz"));
        AreEqualVector(Vector3(a.z, a.z, a.x), a.Swizzle("zzx"));
        AreEqualVector(Vector3(a.z, a.z, a.y), a.Swizzle("zzy"));
        AreEqualVector(Vector3(a.z, a.z, a.z), a.Swizzle("zzz"));
      }

      TEST_METHOD(Swizzling_Vector3_2)
      {
        AreEqualVector(Vector3(a.x, a.x), a.Swizzle("xx"));
        AreEqualVector(Vector3(a.x, a.y), a.Swizzle("xy"));
        AreEqualVector(Vector3(a.x, a.z), a.Swizzle("xz"));
        AreEqualVector(Vector3(a.y, a.x), a.Swizzle("yx"));
        AreEqualVector(Vector3(a.y, a.y), a.Swizzle("yy"));
        AreEqualVector(Vector3(a.y, a.z), a.Swizzle("yz"));
        AreEqualVector(Vector3(a.z, a.x), a.Swizzle("zx"));
        AreEqualVector(Vector3(a.z, a.y), a.Swizzle("zy"));
        AreEqualVector(Vector3(a.z, a.z), a.Swizzle("zz"));
      }

      TEST_METHOD(Swizzling_Vector3_1)
      {
        AreEqualVector(Vector3(a.x), a.Swizzle("x"));
        AreEqualVector(Vector3(a.y), a.Swizzle("y"));
        AreEqualVector(Vector3(a.z), a.Swizzle("z"));
      }

      TEST_METHOD(Swizzling_Vector2)
      {
        AreEqualVector(Vector2(a.x, a.x), (Vector2)a.Swizzle("xx"));
        AreEqualVector(Vector2(a.x, a.y), (Vector2)a.Swizzle("xy"));
        AreEqualVector(Vector2(a.x, a.z), (Vector2)a.Swizzle("xz"));
        AreEqualVector(Vector2(a.y, a.x), (Vector2)a.Swizzle("yx"));
        AreEqualVector(Vector2(a.y, a.y), (Vector2)a.Swizzle("yy"));
        AreEqualVector(Vector2(a.y, a.z), (Vector2)a.Swizzle("yz"));
        AreEqualVector(Vector2(a.z, a.x), (Vector2)a.Swizzle("zx"));
        AreEqualVector(Vector2(a.z, a.y), (Vector2)a.Swizzle("zy"));
        AreEqualVector(Vector2(a.z, a.z), (Vector2)a.Swizzle("zz"));
      }

      TEST_METHOD(Swizzling_Vector2_1)
      {
        AreEqualVector(Vector2(a.x), (Vector2)a.Swizzle("x"));
        AreEqualVector(Vector2(a.y), (Vector2)a.Swizzle("y"));
        AreEqualVector(Vector2(a.z), (Vector2)a.Swizzle("z"));
      }

      TEST_METHOD(Swizzling_Vector1)
      {
        Assert::AreEqual(Vector1(a.x), (Vector1)a.Swizzle("x"));
        Assert::AreEqual(Vector1(a.y), (Vector1)a.Swizzle("y"));
        Assert::AreEqual(Vector1(a.z), (Vector1)a.Swizzle("z"));
      }

    };

    TEST_CLASS(UnitTests_UnaryOperators)
    {
      Vector3 a, b;

    public:

      TEST_METHOD_INITIALIZE(Initialize)
      {
        a = { 1.0f, 2.0f, 3.0f };
        b = { 4.0f, 5.0f, 6.0f };
      }

      TEST_METHOD_CLEANUP(Cleanup)
      {
      }

      TEST_METHOD(Negation)
      {
        AreEqualVector(Vector3(-a.x, -a.y, -a.z), -a);
      }

      TEST_METHOD(Comparison_Equality)
      {
        Assert::IsTrue(a == Vector3(1.0f, 2.0f, 3.0f));
        Assert::IsFalse(a == Vector3(4.0f, 5.0f, 6.0f));
      }

      TEST_METHOD(Comparison_Inequality)
      {
        Assert::IsFalse(a != Vector3(1.0f, 2.0f, 3.0f));
        Assert::IsTrue(a != Vector3(4.0f, 5.0f, 6.0f));
      }

      TEST_METHOD(Addition_Vector)
      {
        a += b;
        AreEqualVector(Vector3(5.0f, 7.0f, 9.0f), a);
      }

      TEST_METHOD(Addition_Value)
      {
        a += 4.0f;
        AreEqualVector(Vector3(5.0f, 6.0f, 7.0f), a);
      }

      TEST_METHOD(Subtraction_Vector)
      {
        a -= b;
        AreEqualVector(Vector3(-3.0f, -3.0f, -3.0f), a);
      }

      TEST_METHOD(Subtraction_Value)
      {
        a -= 4.0f;
        AreEqualVector(Vector3(-3.0f, -2.0f, -1.0f), a);
      }

      TEST_METHOD(Multiplication_Vector)
      {
        a *= b;
        AreEqualVector(Vector3(4.0f, 10.0f, 18.0f), a);
      }

      TEST_METHOD(Multiplication_Value)
      {
        a *= 4.0f;
        AreEqualVector(Vector3(4.0f, 8.0f, 12.0f), a);
      }

      TEST_METHOD(Division_Vec)
      {
        a /= b;
        AreEqualVector(Vector3(1.0f / 4.0f, 2.0f / 5.0f, 3.0f / 6.0f), a);
      }

      TEST_METHOD(Division_Value)
      {
        a /= 4.0f;
        AreEqualVector(Vector3(1.0f / 4.0f, 2.0f / 4.0f, 3.0f / 4.0f), a);
      }

      TEST_METHOD(Division_ValueZero)
      {
        a /= 0.0f;
        AreEqualVector(Vector3(1.0f, 2.0f, 3.0f), a);
      }

    };

    TEST_CLASS(UnitTests_BinaryOperators)
    {
      Vector3 a, b;

    public:

      TEST_METHOD_INITIALIZE(Initialize)
      {
        a = { 1.0f, 2.0f, 3.0f };
        b = { 4.0f, 5.0f, 6.0f };
      }

      TEST_METHOD_CLEANUP(Cleanup)
      {
      }

      TEST_METHOD(Addition_VectorToVector)
      {
        AreEqualVector(Vector3(5.0f, 7.0f, 9.0f), a + b);
      }

      TEST_METHOD(Addition_VectorToValue)
      {
        AreEqualVector(Vector3(5.0f, 6.0f, 7.0f), a + 4.0f);
      }

      TEST_METHOD(Addition_ValueToVector)
      {
        AreEqualVector(Vector3(5.0f, 6.0f, 7.0f), 4.0f + a);
      }

      TEST_METHOD(Subtraction_VectorToVector)
      {
        AreEqualVector(Vector3(-3.0f, -3.0f, -3.0f), a - b);
      }

      TEST_METHOD(Subtraction_VectorToValue)
      {
        AreEqualVector(Vector3(-3.0f, -2.0f, -1.0f), a - 4.0f);
      }

      TEST_METHOD(Subtraction_ValueToVector)
      {
        AreEqualVector(Vector3(3.0f, 2.0f, 1.0f), 4.0f - a);
      }

      TEST_METHOD(Multiplication_VectorToValue)
      {
        AreEqualVector(Vector3(4.0f, 8.0f, 12.0f), a * 4.0f);
      }

      TEST_METHOD(Multiplication_ValueToVector)
      {
        AreEqualVector(Vector3(4.0f, 8.0f, 12.0f), 4.0f * a);
      }

      TEST_METHOD(Division_VectorToValue)
      {
        AreEqualVector(Vector3(1.0f / 4.0f, 2.0f / 4.0f, 3.0f / 4.0f), a / 4.0f);
      }

      TEST_METHOD(Division_ValueToVector)
      {
        AreEqualVector(Vector3(4.0f / 1.0f, 4.0f / 2.0f, 4.0f / 3.0f), 4.0f / a);
      }

    };

    TEST_CLASS(UnitTests_Functions)
    {
      Vector3 a, b;
      glm::vec3 glm_a, glm_b;

    public:

      TEST_METHOD_INITIALIZE(Initialize)
      {
        a = { 1.0f, 2.0f, 3.0f };
        b = { 4.0f, 5.0f, 6.0f };
        glm_a = { 1.0f, 2.0f, 3.0f };
        glm_b = { 4.0f, 5.0f, 6.0f };
      }

      TEST_METHOD_CLEANUP(Cleanup)
      {
      }

      TEST_METHOD(Magnitude_LengthParity)
      {
        Assert::IsTrue(a.Magnitude() == a.Length());
      }

      TEST_METHOD(Magnitude_LengthSqrParity)
      {
        Assert::IsTrue(a.Length() == sqrt(a.LengthSqr()));
      }

      TEST_METHOD(Magnitude_Calculation)
      {
        Assert::AreEqual(glm::length(glm_a), a.Length());
      }

      TEST_METHOD(Normalize_StaticParity)
      {
        Vector3 my_normalize_static = Vector3::Normalize(a);
        Vector3 my_normalize = a.Normalize();
        Assert::IsTrue(my_normalize == my_normalize_static);
      }

      TEST_METHOD(Normalize_LengthCheck)
      {
        Assert::AreEqual(1.0f, Vector3::Normalize(a).Length(), EPSILONf);
      }

      TEST_METHOD(Normalize_Calculation)
      {
        AreEqualVector(glm::normalize(glm_a), Vector3::Normalize(a));
      }

      TEST_METHOD(DotProduct)
      {
        Assert::AreEqual(glm::dot(glm_a, glm_b), Dot(a, b));
      }

      TEST_METHOD(CrossProduct)
      {
        AreEqualVector(glm::cross(glm_a, glm_b), Cross(a, b));
      }

    };

    TEST_CLASS(UnitTests_ConversionOverloads)
    {
      Vector3 a;
      glm::vec3 glm_a;

    public:

      TEST_METHOD_INITIALIZE(Initialize)
      {
        a = { 1.0f, 2.0f, 3.0f };
        glm_a = { 1.0f, 2.0f, 3.0f };
      }

      TEST_METHOD_CLEANUP(Cleanup)
      {
      }

      TEST_METHOD(DegreeToRadians)
      {
        AreEqualVector(glm::radians(glm_a), radians(a));
      }

      TEST_METHOD(RadiansToDegrees)
      {
        AreEqualVector(glm::degrees(glm_a), degrees(a));
      }

    };

  }

  namespace UnitTests_Matrix4x4
  {

    TEST_CLASS(UnitTests_Accessors)
    {
      Matrix4x4 a;
      glm::mat4 glm_a;

    public:

      TEST_METHOD_INITIALIZE(Initialize)
      {
        a = {
           1.0f,  2.0f,  3.0f,  4.0f,
           5.0f,  6.0f,  7.0f,  8.0f,
           9.0f, 10.0f, 11.0f, 12.0f,
          13.0f, 14.0f, 15.0f, 16.0f
        };
        glm_a = {
           1.0f,  2.0f,  3.0f,  4.0f,
           5.0f,  6.0f,  7.0f,  8.0f,
           9.0f, 10.0f, 11.0f, 12.0f,
          13.0f, 14.0f, 15.0f, 16.0f
        };
      }

      TEST_METHOD_CLEANUP(Cleanup)
      {
      }

      TEST_METHOD(Individual)
      {
        Assert::AreEqual(1.0f, a.m00);
        Assert::AreEqual(2.0f, a.m01);
        Assert::AreEqual(3.0f, a.m02);
        Assert::AreEqual(4.0f, a.m03);
        Assert::AreEqual(5.0f, a.m10);
        Assert::AreEqual(6.0f, a.m11);
        Assert::AreEqual(7.0f, a.m12);
        Assert::AreEqual(8.0f, a.m13);
        Assert::AreEqual(9.0f, a.m20);
        Assert::AreEqual(10.0f, a.m21);
        Assert::AreEqual(11.0f, a.m22);
        Assert::AreEqual(12.0f, a.m23);
        Assert::AreEqual(13.0f, a.m30);
        Assert::AreEqual(14.0f, a.m31);
        Assert::AreEqual(15.0f, a.m32);
        Assert::AreEqual(16.0f, a.m33);
      }

      TEST_METHOD(Array)
      {
        Assert::AreEqual(1.0f, a.data[0]);
        Assert::AreEqual(2.0f, a.data[1]);
        Assert::AreEqual(3.0f, a.data[2]);
        Assert::AreEqual(4.0f, a.data[3]);
        Assert::AreEqual(5.0f, a.data[4]);
        Assert::AreEqual(6.0f, a.data[5]);
        Assert::AreEqual(7.0f, a.data[6]);
        Assert::AreEqual(8.0f, a.data[7]);
        Assert::AreEqual(9.0f, a.data[8]);
        Assert::AreEqual(10.0f, a.data[9]);
        Assert::AreEqual(11.0f, a.data[10]);
        Assert::AreEqual(12.0f, a.data[11]);
        Assert::AreEqual(13.0f, a.data[12]);
        Assert::AreEqual(14.0f, a.data[13]);
        Assert::AreEqual(15.0f, a.data[14]);
        Assert::AreEqual(16.0f, a.data[15]);
      }

      TEST_METHOD(Function)
      {
        Assert::AreEqual(1.0f, a(0, 0));
        Assert::AreEqual(2.0f, a(0, 1));
        Assert::AreEqual(3.0f, a(0, 2));
        Assert::AreEqual(4.0f, a(0, 3));
        Assert::AreEqual(5.0f, a(1, 0));
        Assert::AreEqual(6.0f, a(1, 1));
        Assert::AreEqual(7.0f, a(1, 2));
        Assert::AreEqual(8.0f, a(1, 3));
        Assert::AreEqual(9.0f, a(2, 0));
        Assert::AreEqual(10.0f, a(2, 1));
        Assert::AreEqual(11.0f, a(2, 2));
        Assert::AreEqual(12.0f, a(2, 3));
        Assert::AreEqual(13.0f, a(3, 0));
        Assert::AreEqual(14.0f, a(3, 1));
        Assert::AreEqual(15.0f, a(3, 2));
        Assert::AreEqual(16.0f, a(3, 3));
      }

      TEST_METHOD(MemberParity)
      {
        Assert::AreEqual(a.m00, a(0, 0));
        Assert::AreEqual(a.m01, a(0, 1));
        Assert::AreEqual(a.m02, a(0, 2));
        Assert::AreEqual(a.m03, a(0, 3));
        Assert::AreEqual(a.m10, a(1, 0));
        Assert::AreEqual(a.m11, a(1, 1));
        Assert::AreEqual(a.m12, a(1, 2));
        Assert::AreEqual(a.m13, a(1, 3));
        Assert::AreEqual(a.m20, a(2, 0));
        Assert::AreEqual(a.m21, a(2, 1));
        Assert::AreEqual(a.m22, a(2, 2));
        Assert::AreEqual(a.m23, a(2, 3));
        Assert::AreEqual(a.m30, a(3, 0));
        Assert::AreEqual(a.m31, a(3, 1));
        Assert::AreEqual(a.m32, a(3, 2));
        Assert::AreEqual(a.m33, a(3, 3));
      }

      TEST_METHOD(ParityWithGLM)
      {
        for (int i = 0; i < 4; i++)
        {
          for (int j = 0; j < 4; j++)
          {
            Assert::AreEqual(glm_a[i][j], a(i, j));
          }
        }
      }

    };

    TEST_CLASS(UnitTests_UnaryOperators)
    {
      Matrix4x4 a, b, c;
      glm::mat4 glm_a, glm_b, glm_c;

    public:

      TEST_METHOD_INITIALIZE(Initialize)
      {
        a = {
           1.0f,  2.0f,  3.0f,  4.0f,
           5.0f,  6.0f,  7.0f,  8.0f,
           9.0f, 10.0f, 11.0f, 12.0f,
          13.0f, 14.0f, 15.0f, 16.0f
        };
        b = {
           1.0f,  2.0f,  3.0f,  4.0f,
           5.0f,  6.0f,  7.0f,  8.0f,
           9.0f, 10.0f, 11.0f, 12.0f,
          13.0f, 14.0f, 15.0f, 16.0f
        };
        c = {
           2.0f,  4.0f,  6.0f,  8.0f,
          18.0f, 20.0f, 22.0f, 24.0f,
          10.0f, 12.0f, 14.0f, 16.0f,
          26.0f, 28.0f, 30.0f, 32.0f
        };
        glm_a = {
           1.0f,  2.0f,  3.0f,  4.0f,
           5.0f,  6.0f,  7.0f,  8.0f,
           9.0f, 10.0f, 11.0f, 12.0f,
          13.0f, 14.0f, 15.0f, 16.0f
        };
        glm_b = {
           1.0f,  2.0f,  3.0f,  4.0f,
           5.0f,  6.0f,  7.0f,  8.0f,
           9.0f, 10.0f, 11.0f, 12.0f,
          13.0f, 14.0f, 15.0f, 16.0f
        };
        glm_c = {
           2.0f,  4.0f,  6.0f,  8.0f,
          18.0f, 20.0f, 22.0f, 24.0f,
          10.0f, 12.0f, 14.0f, 16.0f,
          26.0f, 28.0f, 30.0f, 32.0f
        };
      }

      TEST_METHOD_CLEANUP(Cleanup)
      {
      }

      TEST_METHOD(Comparison_Equality)
      {
        Assert::IsTrue(a == b);
        Assert::IsFalse(a == c);
      }

      TEST_METHOD(Comparison_Inequality)
      {
        Assert::IsFalse(a != b);
        Assert::IsTrue(a != c);
      }

      TEST_METHOD(Addition_Matrix)
      {
        a += c;
        glm_a += glm_c;
        AreEqualMatrix(glm_a, a);
      }

      TEST_METHOD(Subtraction_Matrix)
      {
        a -= c;
        glm_a -= glm_c;
        AreEqualMatrix(glm_a, a);
      }

      TEST_METHOD(Multiplication_Matrix)
      {
        a *= c;
        glm_a *= glm_c;
        AreEqualMatrix(glm_a, a);
      }

      TEST_METHOD(Multiplication_Value)
      {
        a *= 2.0f;
        glm_a *= 2.0f;
        AreEqualMatrix(glm_a, a);
      }

      TEST_METHOD(Division_Value)
      {
        a /= 2.0f;
        glm_a /= 2.0f;
        AreEqualMatrix(glm_a, a);
      }

    };

    TEST_CLASS(UnitTests_BinaryOperators)
    {
      Matrix4x4 a, b;
      glm::mat4 glm_a, glm_b;

    public:

      TEST_METHOD_INITIALIZE(Initialize)
      {
        a = {
           1.0f,  2.0f,  3.0f,  4.0f,
           5.0f,  6.0f,  7.0f,  8.0f,
           9.0f, 10.0f, 11.0f, 12.0f,
          13.0f, 14.0f, 15.0f, 16.0f
        };
        b = {
           2.0f,  4.0f,  6.0f,  8.0f,
          18.0f, 20.0f, 22.0f, 24.0f,
          10.0f, 12.0f, 14.0f, 16.0f,
          26.0f, 28.0f, 30.0f, 32.0f
        };
        glm_a = {
           1.0f,  2.0f,  3.0f,  4.0f,
           5.0f,  6.0f,  7.0f,  8.0f,
           9.0f, 10.0f, 11.0f, 12.0f,
          13.0f, 14.0f, 15.0f, 16.0f
        };
        glm_b = {
           2.0f,  4.0f,  6.0f,  8.0f,
          18.0f, 20.0f, 22.0f, 24.0f,
          10.0f, 12.0f, 14.0f, 16.0f,
          26.0f, 28.0f, 30.0f, 32.0f
        };
      }

      TEST_METHOD_CLEANUP(Cleanup)
      {
      }

      TEST_METHOD(Addition_MatrixToMatrix)
      {
        AreEqualMatrix(glm_a + glm_b, a + b);
      }

      TEST_METHOD(Subtraction_MatrixToMatrix)
      {
        AreEqualMatrix(glm_a - glm_b, a - b);
      }

      TEST_METHOD(Multiplication_MatrixToMatrix)
      {
        AreEqualMatrix(glm_a * glm_b, a * b);
      }

      TEST_METHOD(Multiplication_MatrixToValue)
      {
        AreEqualMatrix(glm_a * 2.0f, a * 2.0f);
      }

      TEST_METHOD(Multiplication_ValueToMatrix)
      {
        AreEqualMatrix(2.0f * glm_a, 2.0f * a);
      }

      TEST_METHOD(Division_MatrixToValue)
      {
        AreEqualMatrix(glm_a / 2.0f, a / 2.0f);
      }

    };

    TEST_CLASS(UnitTests_Functions)
    {
      Matrix4x4 a;
      glm::mat4 glm_a;

    public:

      TEST_METHOD_INITIALIZE(Initialize)
      {
        a = {
          5.0f, 6.0f, 6.0f, 8.0f,
          2.0f, 2.0f, 2.0f, 8.0f,
          6.0f, 6.0f, 2.0f, 8.0f,
          2.0f, 3.0f, 6.0f, 7.0f
        };
        glm_a = {
          5.0f, 6.0f, 6.0f, 8.0f,
          2.0f, 2.0f, 2.0f, 8.0f,
          6.0f, 6.0f, 2.0f, 8.0f,
          2.0f, 3.0f, 6.0f, 7.0f
        };
      }

      TEST_METHOD_CLEANUP(Cleanup)
      {
      }

      TEST_METHOD(Transpose)
      {
        AreEqualMatrix(glm::transpose(glm_a), a.Transpose());
      }

      TEST_METHOD(Determinant)
      {
        Assert::AreEqual(glm::determinant(glm_a), a.Determinant());
      }

      TEST_METHOD(Inverse)
      {
        AreEqualMatrix(glm::inverse(glm_a), a.Inverse());
      }

    };

    TEST_CLASS(UnitTests_Transformations_StaticParity)
    {
      Matrix4x4 a;
      glm::mat4 glm_a;

    public:

      TEST_METHOD_INITIALIZE(Initialize)
      {
        a = Matrix4x4::Identity;
        glm_a = glm::mat4(1.0f);
      }

      TEST_METHOD_CLEANUP(Cleanup)
      {
      }

      TEST_METHOD(Static_Translate)
      {
        Matrix4x4 static_translate = Matrix4x4::Translate(a, Vector3(1.0f, 2.0f, 3.0f));
        Assert::IsTrue(a.Translate(Vector3(1.0f, 2.0f, 3.0f)) == static_translate);
      }

      TEST_METHOD(Static_Rotate)
      {
        Matrix4x4 static_rotate = Matrix4x4::Rotate(a, radians(45.0f), Vector3(0.3f, 0.5f, 0.7f));
        Assert::IsTrue(a.Rotate(radians(45.0f), Vector3(0.3f, 0.5f, 0.7f)) == static_rotate);
      }

      TEST_METHOD(Static_RotateDeg)
      {
        Matrix4x4 static_rotate_deg = Matrix4x4::RotateDeg(a, 45.0f, Vector3(0.3f, 0.5f, 0.7f));
        Assert::IsTrue(a.RotateDeg(45.0f, Vector3(0.3f, 0.5f, 0.7f)) == static_rotate_deg);
      }

      TEST_METHOD(Static_RotateX)
      {
        Matrix4x4 static_rotate_x = Matrix4x4::RotateX(a, radians(45.0f));
        Assert::IsTrue(a.RotateX(radians(45.0f)) == static_rotate_x);
      }

      TEST_METHOD(Static_RotateY)
      {
        Matrix4x4 static_rotate_y = Matrix4x4::RotateY(a, radians(45.0f));
        Assert::IsTrue(a.RotateY(radians(45.0f)) == static_rotate_y);
      }

      TEST_METHOD(Static_RotateZ)
      {
        Matrix4x4 static_rotate_z = Matrix4x4::RotateZ(a, radians(45.0f));
        Assert::IsTrue(a.RotateZ(radians(45.0f)) == static_rotate_z);
      }

      TEST_METHOD(Static_RotateXDeg)
      {
        Matrix4x4 static_rotate_x_deg = Matrix4x4::RotateXDeg(a, 45.0f);
        Assert::IsTrue(a.RotateXDeg(45.0f) == static_rotate_x_deg);
      }

      TEST_METHOD(Static_RotateYDeg)
      {
        Matrix4x4 static_rotate_y_deg = Matrix4x4::RotateYDeg(a, 45.0f);
        Assert::IsTrue(a.RotateYDeg(45.0f) == static_rotate_y_deg);
      }

      TEST_METHOD(Static_RotateZDeg)
      {
        Matrix4x4 static_rotate_z_deg = Matrix4x4::RotateZDeg(a, 45.0f);
        Assert::IsTrue(a.RotateZDeg(45.0f) == static_rotate_z_deg);
      }

      TEST_METHOD(Static_Scale)
      {
        Matrix4x4 static_scale = Matrix4x4::Scale(a, Vector3(1.0f, 2.0f, 3.0f));
        Assert::IsTrue(a.Scale(Vector3(1.0f, 2.0f, 3.0f)) == static_scale);
      }

    };

    TEST_CLASS(UnitTests_Transformations_Common)
    {
      Matrix4x4 a;
      glm::mat4 glm_a;

    public:

      TEST_METHOD_INITIALIZE(Initialize)
      {
        a = Matrix4x4::Identity;
        glm_a = glm::mat4(1.0f);
      }

      TEST_METHOD_CLEANUP(Cleanup)
      {
      }

      TEST_METHOD(Common_Translate)
      {
        glm_a = glm::translate(glm_a, glm::vec3(10.0f, -20.0f, 30.0f));
        AreEqualMatrix(glm_a, a.Translate(Vector3(10.0f, -20.0f, 30.0f)));
      }

      TEST_METHOD(Common_Rotate)
      {
        glm_a = glm::rotate(glm_a, glm::radians(45.0f), glm::vec3(0.3f, 0.5f, 0.7f));
        AreEqualMatrix(glm_a, a.Rotate(radians(45.0f), Vector3(0.3f, 0.5f, 0.7f)));
      }

      TEST_METHOD(Common_RotateDeg)
      {
        glm_a = glm::rotate(glm_a, glm::radians(45.0f), glm::vec3(0.3f, 0.5f, 0.7f));
        AreEqualMatrix(glm_a, a.RotateDeg(45.0f, Vector3(0.3f, 0.5f, 0.7f)));
      }

      TEST_METHOD(Common_RotateX)
      {
        glm_a = glm::rotate(glm_a, glm::radians(35.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        AreEqualMatrix(glm_a, a.RotateX(radians(35.0f)));
      }

      TEST_METHOD(Common_RotateY)
      {
        glm_a = glm::rotate(glm_a, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        AreEqualMatrix(glm_a, a.RotateY(radians(45.0f)));
      }

      TEST_METHOD(Common_RotateZ)
      {
        glm_a = glm::rotate(glm_a, glm::radians(55.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        AreEqualMatrix(glm_a, a.RotateZ(radians(55.0f)));
      }

      TEST_METHOD(Common_RotateXDeg)
      {
        glm_a = glm::rotate(glm_a, glm::radians(35.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        AreEqualMatrix(glm_a, a.RotateXDeg(35.0f));
      }

      TEST_METHOD(Common_RotateYDeg)
      {
        glm_a = glm::rotate(glm_a, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        AreEqualMatrix(glm_a, a.RotateYDeg(45.0f));
      }

      TEST_METHOD(Common_RotateZDeg)
      {
        glm_a = glm::rotate(glm_a, glm::radians(55.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        AreEqualMatrix(glm_a, a.RotateZDeg(55.0f));
      }

      TEST_METHOD(Common_Scale)
      {
        glm_a = glm::scale(glm_a, glm::vec3(10.0f, 20.0f, -30.0f));
        AreEqualMatrix(glm_a, a.Scale(Vector3(10.0f, 20.0f, -30.0f)));
      }

    };

    TEST_CLASS(UnitTests_Transformations_Others)
    {
      Matrix4x4 a;
      glm::mat4 glm_a;

    public:

      TEST_METHOD_INITIALIZE(Initialize)
      {
        a = Matrix4x4::Identity;
        glm_a = glm::mat4(1.0f);
      }

      TEST_METHOD_CLEANUP(Cleanup)
      {
      }

      TEST_METHOD(Others_LookAt)
      {
        glm_a = glm::lookAt(glm::vec3(1.0f, 2.0f, 3.0f), glm::vec3(4.0f, 5.0f, 6.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        a = Matrix4x4::LookAt(Vector3(1.0f, 2.0f, 3.0f), Vector3(4.0f, 5.0f, 6.0f), Vector3(0.0f, 1.0f, 0.0f));
        AreEqualMatrix(glm_a, a, 1e-06f);
#pragma message("Developer's Note: The LookAt test is not to EPSILONf precision but 1e-06f is close enough.")
      }

      TEST_METHOD(Others_Perspective)
      {
        glm_a = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        a = Matrix4x4::Perspective(radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        AreEqualMatrix(glm_a, a);
      }

      TEST_METHOD(Others_Orthographic)
      {
        glm_a = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
        a = Matrix4x4::Orthographic(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
        AreEqualMatrix(glm_a, a);
      }

    };

  }

}
