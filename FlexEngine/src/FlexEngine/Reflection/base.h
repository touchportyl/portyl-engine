#pragma once

#include <cstddef>
#include <iostream>
#include <vector>
#include <string>


/* Reflection system for C++
 * 
 * A reflection system (also called a property system)
 * is a coding methodology or language feature which allows
 * exposing certain information about your code to other systems
 * at runtime.
 * This is often used in serialization, GUIs, and other systems.
 * 
 * References:
 *  A Complete Example for C++ Reflection
 *    https://yingnanwang.com/coding/2020-10-05-c-reflection/
 *  A Flexible Reflection System in C++: Part 1
 *    https://preshing.com/20180116/a-primitive-reflection-system-in-cpp-part-1/
 *  Unreal Engine Implementation
 *    https://www.unrealengine.com/en-US/blog/unreal-property-system-reflection
 *  properties by LIONant
 *    https://gitlab.com/LIONant/properties/blob/master/docs/Documentation.md
 *  RTTR (Run Time Type Reflection)
 *    https://www.rttr.org/doc/rttr-0-9-6/five_minute_tutorial_page.html
 */


/// <summary>
/// Enables reflection for a custom type (struct/class)
/// <para>Place at the top of the class definition</para>
/// </summary>
#define FLX_REFL_SERIALIZABLE \
  friend struct FlexEngine::Reflection::DefaultResolver; \
  static FlexEngine::Reflection::TypeDescriptor_Struct Reflection; \
  static void InitReflection(FlexEngine::Reflection::TypeDescriptor_Struct*);

/// <summary>
/// Starts the registration of member variables for reflection
/// <para>Place inside any .cpp file that includes the declaration of the custom type</para>
/// </summary>
#define FLX_REFL_REGISTER_START(TYPE) \
  FlexEngine::Reflection::TypeDescriptor_Struct TYPE::Reflection{TYPE::InitReflection}; \
  void TYPE::InitReflection(FlexEngine::Reflection::TypeDescriptor_Struct* type_desc) { \
    using T = TYPE; \
    type_desc->name = #TYPE; \
    type_desc->size = sizeof(T); \
    type_desc->members = {

/// <summary>
/// Registers a member variable for reflection
/// </summary>
#define FLX_REFL_REGISTER_PROPERTY(NAME) \
      {#NAME, offsetof(T, NAME), FlexEngine::Reflection::TypeResolver<decltype(T::NAME)>::Get()},

/// <summary>
/// Ends the reflection registration
/// </summary>
#define FLX_REFL_REGISTER_END \
    }; \
  }

namespace FlexEngine
{

  namespace Reflection
  {

    /// <summary>
    /// Base class for all type descriptors.
    /// </summary>
    struct TypeDescriptor
    {
      const char* name;
      size_t size;

      TypeDescriptor(const char* name, size_t size) : name{ name }, size{ size } {}
      virtual ~TypeDescriptor() {}

      /// <summary>
      /// Get the full name of the type, including any template parameters.
      /// </summary>
      virtual std::string ToString() const { return name; }

      /// <summary>
      /// Dumps the contents of an object to the console.
      /// </summary>
      virtual void Dump(const void* obj, std::ostream& os = std::cout, int indentLevel = 0) const = 0;

      /// <summary>
      /// Serializes an object to a stream.
      /// <para>This uses a special format that can be read back in by Deserialize.</para>
      /// </summary>
      virtual void Serialize(const void* obj, std::ostream& out) const = 0;
    };

    /// <summary>
    /// Declare the function template that handles primitive types
    /// such as int, std::string, etc.
    /// </summary>
    template <typename T>
    TypeDescriptor* GetPrimitiveDescriptor();

    /// <summary>
    /// A helper class to find TypeDescriptors in different ways.
    /// </summary>
    struct DefaultResolver
    {
      // Detects if the type T has a static member named "Reflection"
      // This is done by overloading a function with two different return types
      // and using SFINAE to select the right one.
      // Use as "IsReflected<T>::value"
      template <typename T> static char func(decltype(&T::Reflection));
      template <typename T> static int func(...);
      template <typename T>
      struct IsReflected {
        enum { value = (sizeof(func<T>(nullptr)) == sizeof(char)) };
      };

      /// <summary>
      /// This version is called if T has a static member named "Reflection"
      /// </summary>
      template <typename T, typename std::enable_if<IsReflected<T>::value, int>::type = 0>
      static TypeDescriptor* Get()
      {
        return &T::Reflection;
      }

      /// <summary>
      /// This version is called if T is a primitive type
      /// </summary>
      template <typename T, typename std::enable_if<!IsReflected<T>::value, int>::type = 0>
      static TypeDescriptor* Get()
      {
        return GetPrimitiveDescriptor<T>();
      }
    };

    /// <summary>
    /// This is the primary class template for finding all TypeDescriptors
    /// </summary>
    template <typename T>
    struct TypeResolver
    {
      static TypeDescriptor* Get()
      {
        return DefaultResolver::Get<T>();
      }
    };



    /// <summary>
    /// Type descriptor for user-defined structs/classes
    /// </summary>
    struct TypeDescriptor_Struct : TypeDescriptor
    {
      struct Member
      {
        const char* name;
        size_t offset;
        TypeDescriptor* type;
      };

      std::vector<Member> members;

      TypeDescriptor_Struct(void (*init)(TypeDescriptor_Struct*))
        : TypeDescriptor{ nullptr, 0 }
      {
        init(this);
      }

      #pragma warning(suppress: 4100) // "unused parameter"
      TypeDescriptor_Struct(const char* name, size_t size, const std::initializer_list<Member>& init)
        : TypeDescriptor{ nullptr, 0 }, members{ init }
      {
      }

      virtual void Dump(const void* obj, std::ostream& os, int indentLevel) const override
      {
        os << name << "\n" << std::string(4 * indentLevel, ' ') << "{\n";
        for (const Member& member : members)
        {
          os << std::string(4 * (indentLevel + 1), ' ') << member.name << " = ";
          member.type->Dump((char*)obj + member.offset, os, indentLevel + 1);
          os << "\n";
        }
        os << std::string(4 * indentLevel, ' ') << "}\n";
      }

      virtual void Serialize(const void* obj, std::ostream& os) const override
      {
        os << R"({"type":")" << name << R"(","data":[)";
        int index = 0;
        for (const Member& member : members)
        {
          member.type->Serialize((char*)obj + member.offset, os);
          if (index++ < members.size() - 1) os << ","; 
        }
        os << "]}";
      }
    };



    /// <summary>
    /// TypeDescriptor for std::vector
    /// <para>Specialized for std::vector</para>
    /// </summary>
    struct TypeDescriptor_StdVector : TypeDescriptor
    {
      TypeDescriptor* item_type;
      size_t(*get_size)(const void*);
      const void* (*get_item)(const void*, size_t);

      template <typename ItemType>
      TypeDescriptor_StdVector(ItemType*)
        : TypeDescriptor{ "std::vector<>", sizeof(std::vector<ItemType>) },
        item_type{ TypeResolver<ItemType>::Get() }
      {
        get_size = [](const void* vec_ptr) -> size_t {
          const auto& vec = *(const std::vector<ItemType>*) vec_ptr;
          return vec.size();
        };
        get_item = [](const void* vec_ptr, size_t index) -> const void* {
          const auto& vec = *(const std::vector<ItemType>*) vec_ptr;
          return &vec[index];
        };
      }

      virtual std::string ToString() const override
      {
        return std::string("std::vector<") + item_type->ToString() + ">";
      }

      virtual void Dump(const void* obj, std::ostream& os, int indentLevel) const override
      {
        size_t num_items = get_size(obj);
        os << "\n" << ToString();
        if (num_items == 0)
        {
          os << "{}";
        }
        else
        {
          os << "\n" << std::string(4 * indentLevel, ' ') << "{\n";
          for (size_t index = 0; index < num_items; index++)
          {
            os << std::string(4 * (indentLevel + 1), ' ') << "[" << index << "]\n"
              << std::string(4 * (indentLevel + 1), ' ');
            item_type->Dump(get_item(obj, index), os, indentLevel + 1);
            os << "\n";
          }
          os << std::string(4 * indentLevel, ' ') << "}\n";
        }
      }

      virtual void Serialize(const void* obj, std::ostream& os) const override
      {
        size_t num_items = get_size(obj);
        if (num_items != 0)
        {
          os << R"({"type":")" << ToString() << R"(","data":[)";
          for (size_t index = 0; index < num_items; index++)
          {
            item_type->Serialize(get_item(obj, index), os);
            if (index < num_items - 1) os << ",";
          }
          os << "]}";
        }
      }

    };

    /// <summary>
    /// Partially specialize TypeResolver<> for std::vectors
    /// </summary>
    template <typename T>
    class TypeResolver<std::vector<T>>
    {
    public:
      static TypeDescriptor* Get()
      {
        static TypeDescriptor_StdVector typeDesc{ (T*) nullptr };
        return &typeDesc;
      }
    };

  }

}
