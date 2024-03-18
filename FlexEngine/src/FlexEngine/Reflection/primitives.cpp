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
    TYPE_DESC(Bool, bool)
    TYPE_DESC(Int, int)
    TYPE_DESC(Unsigned, unsigned)
    TYPE_DESC(LongLong, int64_t) // long long
    TYPE_DESC(UnsignedLongLong, uint64_t) // unsigned long long
    TYPE_DESC(Double, double)
    TYPE_DESC(Float, float)
    // no support for const char*, just use std::string
    TYPE_DESC(StdString, std::string)
  }

}
