#pragma once

#include <cstddef>
#include <iostream>
#include <vector>
//#include <string>
//#include <sstream>
#include <map>
#include <functional>

#include "flexformatter.h"
#include <flexassert.h>

// Reflection system for C++
// 
// A reflection system (also called a property system) is a coding
// methodology or language feature which allows exposing certain
// information about your code to other systems at runtime.
// This is often used in serialization, GUIs, and other systems.
// 
// I use some of the tricks from the following articles to implement this.
// However, I have rewritten most of the code to fit my own needs.
// 
// References:
//  A Complete Example for C++ Reflection
//    https://yingnanwang.com/coding/2020-10-05-c-reflection/
//  A Flexible Reflection System in C++: Part 1
//    https://preshing.com/20180116/a-primitive-reflection-system-in-cpp-part-1/
//  Unreal Engine Implementation
//    https://www.unrealengine.com/en-US/blog/unreal-property-system-reflection
//  properties by LIONant
//    https://gitlab.com/LIONant/properties/blob/master/docs/Documentation.md
//  RTTR (Run Time Type Reflection)
//    https://www.rttr.org/doc/rttr-0-9-6/five_minute_tutorial_page.html


/// <summary>
/// Enables reflection for a custom type (struct/class)
/// <para>Place at the top of the class definition in the .h file</para>
/// </summary>
#define FLX_REFL_SERIALIZABLE \
  friend struct FlexEngine::Reflection::DefaultResolver; \
  static FlexEngine::Reflection::TypeDescriptor_Struct Reflection; \
  static void InitReflection(FlexEngine::Reflection::TypeDescriptor_Struct*);

/// <summary>
/// Starts the registration of member variables for reflection
/// <para>Remember to end with FLX_REFL_REGISTER_END</para>
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
/// <para>Place inside the FLX_REFL_REGISTER_START block</para>
/// <para>Use the name of the member variable as the argument</para>
/// <para>It's best practice to indent the block for readability</para>
/// </summary>
#define FLX_REFL_REGISTER_PROPERTY(VARIABLE) \
      {#VARIABLE, offsetof(T, VARIABLE), FlexEngine::Reflection::TypeResolver<decltype(T::VARIABLE)>::Get()},

/// <summary>
/// Ends the reflection registration
/// <para>Pair this with FLX_REFL_REGISTER_START</para>
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
    /// <para>A type descriptor is a class that describes a type,
    /// including its name, size, and how to serialize/deserialize it.</para>
    /// </summary>
    struct TypeDescriptor
    {
      using json = rapidjson::Value;

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
      /// <para>Defaults to using std::cout.</para>
      /// </summary>
      virtual void Dump(const void* obj, std::ostream& os = std::cout, int indentLevel = 0) const = 0;

      /// <summary>
      /// Serializes an object to a stream.
      /// <para>This recursively serializes the object into the json format.</para>
      /// </summary>
      virtual void Serialize(const void* obj, std::ostream& out) const = 0;

      /// <summary>
      /// Deserializes an object from a json document.
      /// <para>This recursively deserializes the object from the json format.</para>
      /// <para>The deserializer uses the rapidjson library.</para>
      /// </summary>
      virtual void Deserialize(void* obj, const json& value) const = 0;
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
    /// This is the primary class template for finding all TypeDescriptors.
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
    /// Type descriptor for user-defined structs/classes.
    /// <para>Specialized for structs/classes.</para>
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

      TypeDescriptor_Struct(const char*, size_t, const std::initializer_list<Member>& init)
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

      virtual void Deserialize(void* obj, const json& value) const override
      {
        const auto& arr = value["data"].GetArray();

        // guard against array size mismatch
        FLX_INTERNAL_ASSERT(arr.Size() != members.size(),
          "Array size mismatch while deserializing struct\nThis is most likely caused by a corrupted .flx file"
        );

        // deserialize each member
        for (SizeType i = 0; i < members.size(); i++)
        {
          members[i].type->Deserialize((char*)obj + members[i].offset, arr[i]);
        }
      }

    };



    /// <summary>
    /// TypeDescriptor for std::vector.
    /// <para>Specialized for std::vector.</para>
    /// </summary>
    struct TypeDescriptor_StdVector : TypeDescriptor
    {
      TypeDescriptor* item_type;
      size_t(*get_size)(const void*);
      const void* (*get_item)(const void*, size_t);
      void* (*set_item)(void*, size_t);

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
        set_item = [](void* vec_ptr, size_t index) -> void* {
          auto& vec = *(std::vector<ItemType>*) vec_ptr;
          if (index >= vec.size()) vec.resize(index + 1);
          return &vec[index];
        };
      }

      virtual std::string ToString() const override
      {
        return std::string("std::vector<") + item_type->ToString() + ">";
        //return std::string("std::vector<>");
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

      virtual void Deserialize(void* obj, const json& value) const override
      {
        const auto& arr = value["data"].GetArray();

        for (SizeType i = 0; i < arr.Size(); i++)
        {
          item_type->Deserialize(set_item(obj, i), arr[i]);
        }
      }

    };



    /// <summary>
    /// Partially specialize TypeResolver for std::vectors.
    /// </summary>
    template <typename T>
    struct TypeResolver<std::vector<T>>
    {
      static TypeDescriptor* Get()
      {
        static TypeDescriptor_StdVector typeDesc{ (T*) nullptr };
        return &typeDesc;
      }
    };


  }

}
