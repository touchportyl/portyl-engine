#include "Reflection/base.h"

/// <summary>
/// TypeDescriptor for primitive types
/// <para>Supports the same types that rapidjson supports except const char*</para>
/// <para>Abstracted for easy editing</para>
/// </summary>
#define TYPE_DESC(NAME, TYPE) \
  struct TypeDescriptor_##NAME : TypeDescriptor \
  { \
    TypeDescriptor_##NAME() : TypeDescriptor{ #TYPE, sizeof(TYPE) } {} \
    virtual void Dump(const void* obj, std::ostream& os, int) const override \
    { \
      os << #TYPE << "{" << *(const TYPE*)obj << "}"; \
    } \
    virtual void Serialize(const void* obj, std::ostream& os) const override \
    { \
      os << R"({"type":")" << #TYPE << R"(","data":)" << *(const TYPE*)obj << "}"; \
    } \
    virtual void Deserialize(void* obj, const json& value) const override \
    { \
      TYPE data = value["data"].Get<TYPE>(); \
      *(TYPE*)obj = data; \
    } \
  }; \
  template <> TypeDescriptor* GetPrimitiveDescriptor<TYPE>() \
  { \
    static TypeDescriptor_##NAME type_desc; return &type_desc; \
  }



namespace FlexEngine
{

  namespace Reflection
  {
    // Primitive type registration
    //TYPE_DESC(Bool, bool) // specialized below
    TYPE_DESC(Int, int)
    TYPE_DESC(Unsigned, unsigned)
    TYPE_DESC(LongLong, int64_t) // long long
    TYPE_DESC(UnsignedLongLong, uint64_t) // unsigned long long
    TYPE_DESC(Double, double)
    TYPE_DESC(Float, float)
    // no support for const char*, just use std::string
    //TYPE_DESC(StdString, std::string) // specialized below

    /// <summary>
    /// TypeDescriptor for bool.
    /// <para>
    ///   Even though this is a primitive type, a specialization for it
    ///   is necessary because it is represented by TRUE/FALSE in the json format.
    /// </para>
    /// </summary>
    struct TypeDescriptor_Bool : TypeDescriptor
    {
      TypeDescriptor_Bool()
        : TypeDescriptor{ "bool", sizeof(bool) }
      {
      }
      virtual void Dump(const void* obj, std::ostream& os, int) const override
      {
        os << "bool" << "{" << *(const bool*)obj << "}";
      }
      virtual void Serialize(const void* obj, std::ostream& os) const override
      {
        os << R"({"type":")" << "bool" << R"(","data":)" << ((*(const bool*)obj) ? "true" : "false") << "}";
      }
      virtual void Deserialize(void* obj, const json& value) const override
      {
        bool data = value["data"].Get<bool>(); *(bool*)obj = data;
      }
    };
    template <> TypeDescriptor* GetPrimitiveDescriptor<bool>()
    {
      static TypeDescriptor_Bool type_desc;
      return &type_desc;
    }



    /// <summary>
    /// TypeDescriptor for std::string.
    /// <para>
    ///   Even though this is a primitive type, a specialization for it
    ///   is necessary because it needs to be wrapped by "" in the json format.
    /// </para>
    /// </summary>
    struct TypeDescriptor_StdString : TypeDescriptor
    {
      TypeDescriptor_StdString()
        : TypeDescriptor{ "std::string", sizeof(std::string) }
      {
      }

      virtual void Dump(const void* obj, std::ostream& os, int) const override
      {
        os << "std::string" << "{" << *(const std::string*)obj << "}";
      }
      virtual void Serialize(const void* obj, std::ostream& os) const override
      {
        os << R"({"type":")" << "std::string" << R"(","data":")" << *(const std::string*)obj << R"("})";
      }
      virtual void Deserialize(void* obj, const json& value) const override
      {
        std::string data = value["data"].Get<std::string>(); *(std::string*)obj = data;
      }
    };
    template <>
    TypeDescriptor* GetPrimitiveDescriptor<std::string>()
    {
      static TypeDescriptor_StdString type_desc;
      return &type_desc;
    }

  }

}
