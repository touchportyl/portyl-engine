#pragma once

#include <unordered_map>
#include <typeindex>
#include <memory>
#include <functional>

#include "uuid.h"
#include "propertymap.h"


// reflection system
// this is a simple reflection system that allows for serialization and deserialization of classes
// it is based on the UUID system and is opt-in


// register a class for serialization
// this is done by adding a UUID to the class
#define FLX_REFL_SERIALIZABLE \
  public: \
  FlexEngine::UUID uuid; \
  private:

// standardized header to serialize a class
// define the methods in the cpp file
#define FLX_REFL_SERIALIZE \
  public: \
  void Serialize(std::ostream& stream) const; \
  private:

// standardized virtual header to serialize a class
// this is used for the base class of a hierarchy
#define FLX_REFL_SERIALIZE_VIRTUAL \
  public: \
  virtual void Serialize(std::ostream& stream) const = 0; \
  private:

// standardized override header to serialize a class
// this is used for the derived classes of a hierarchy
// define the methods in the cpp file
#define FLX_REFL_SERIALIZE_OVERRIDE \
  public: \
  void Serialize(std::ostream& stream) const override; \
  private:


// allow access to private/protected members
#define FLX_REFL_FRIEND \
  friend class PropertyBase; \
  friend class PropertyMap;


// register a class for serialization

// start the registration of a class
// put this at the very bottom of the class definition
// this is to ensure that the properties are registered after the class is fully defined
// 
// example:
// FLX_REFL_REGISTER
// {
//   FLX_REFL_REGISTER_PROPERTY(x)
// };
#define FLX_REFL_REGISTER \
  public: \
  FlexEngine::PropertyMap properties =

// add a property to the reflection system
#define FLX_REFL_REGISTER_PROPERTY(PROPERTY) \
  { \
    #PROPERTY, \
    std::make_shared< \
      FlexEngine::Property< \
      decltype(PROPERTY), \
      std::function<decltype(PROPERTY)()>, \
      std::function<void(decltype(PROPERTY))> \
      > \
    >( \
      [this]() { return PROPERTY; }, \
      [this](decltype(PROPERTY) value) { PROPERTY = value; } \
    ) \
  }
