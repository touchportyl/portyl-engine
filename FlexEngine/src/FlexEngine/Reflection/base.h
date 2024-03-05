#include <vector>
#include <iostream>
#include <string>
#include <cstddef>

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
      virtual std::string GetFullName() const { return name; }
      virtual void Dump(const void* obj, int indentLevel = 0) const = 0;
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

      TypeDescriptor_Struct(const char* name, size_t size, const std::initializer_list<Member>& init)
        : TypeDescriptor{ nullptr, 0 }, members{ init }
      {
      }

      virtual void Dump(const void* obj, int indentLevel) const override
      {
        std::cout << name << " {" << std::endl;
        for (const Member& member : members) {
          std::cout << std::string(4 * (indentLevel + 1), ' ') << member.name << " = ";
          member.type->Dump((char*)obj + member.offset, indentLevel + 1);
          std::cout << std::endl;
        }
        std::cout << std::string(4 * indentLevel, ' ') << "}";
      }
    };

#define FLX_REFL_SERIALIZABLE \
    friend struct FlexEngine::Reflection::DefaultResolver; \
    static FlexEngine::Reflection::TypeDescriptor_Struct Reflection; \
    static void InitReflection(FlexEngine::Reflection::TypeDescriptor_Struct*);

#define FLX_REFL_REGISTER_START(TYPE) \
    FlexEngine::Reflection::TypeDescriptor_Struct TYPE::Reflection{TYPE::InitReflection}; \
    void TYPE::InitReflection(FlexEngine::Reflection::TypeDescriptor_Struct* type_desc) { \
        using T = TYPE; \
        type_desc->name = #TYPE; \
        type_desc->size = sizeof(T); \
        type_desc->members = {

#define FLX_REFL_REGISTER_PROPERTY(NAME) \
            {#NAME, offsetof(T, NAME), FlexEngine::Reflection::TypeResolver<decltype(T::NAME)>::Get()},

#define FLX_REFL_REGISTER_END \
        }; \
    }

    //--------------------------------------------------------
    // Type descriptors for std::vector
    //--------------------------------------------------------

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

      virtual std::string GetFullName() const override
      {
        return std::string("std::vector<") + item_type->GetFullName() + ">";
      }

      virtual void Dump(const void* obj, int indentLevel) const override
      {
        size_t num_items = get_size(obj);
        std::cout << GetFullName();
        if (num_items == 0)
        {
          std::cout << "{}";
        }
        else
        {
          std::cout << "{" << std::endl;
          for (size_t index = 0; index < num_items; index++)
          {
            std::cout << std::string(4 * (indentLevel + 1), ' ') << "[" << index << "] ";
            item_type->Dump(get_item(obj, index), indentLevel + 1);
            std::cout << std::endl;
          }
          std::cout << std::string(4 * indentLevel, ' ') << "}";
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
