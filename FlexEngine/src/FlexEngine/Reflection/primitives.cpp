#include "Reflection/base.h"

// abstracted for easy editing
#define CTOR(TYPE) \
  struct TypeDescriptor_Int : TypeDescriptor \
  { \
    TypeDescriptor_Int() : TypeDescriptor{ "int", sizeof(int) } {}

#define DUMP_TYPE_DESC(TYPE) \
    virtual void Dump(const void* obj, int) const override \
    { \
      std::cout << #TYPE << "{" << *(const TYPE*)obj << "}"; \
    }

namespace FlexEngine
{

  namespace Reflection
  {
    /* TypeDescriptor List
     * - int, float, double
     * - std::string
     */

    struct TypeDescriptor_Int : TypeDescriptor
    {
      TypeDescriptor_Int() : TypeDescriptor{ "int", sizeof(int) } {}
      DUMP_TYPE_DESC(int)
    };
    template <> TypeDescriptor* GetPrimitiveDescriptor<int>()
    { static TypeDescriptor_Int type_desc; return &type_desc; }


    struct TypeDescriptor_StdString : TypeDescriptor
    {
      TypeDescriptor_StdString() : TypeDescriptor{ "std::string", sizeof(std::string) } {}
      virtual void Dump(const void* obj, int) const override
      { std::cout << "std::string{\"" << *(const std::string*)obj << "\"}"; }
    };
    template <> TypeDescriptor* GetPrimitiveDescriptor<std::string>()
    { static TypeDescriptor_StdString typeDesc; return &typeDesc; }

  }

}
