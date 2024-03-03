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

#pragma warning(disable: 4100) // unreferenced formal parameter
#pragma warning(disable: 4010) // single-line comment contains line-continuation character

namespace FlexEngine
{

  // map of property maps
  using PropertyMapMap = std::unordered_map<UUID, std::shared_ptr<PropertyMap>>;

  class ReflectionSystem
  {
  public:
    /// <summary>
    /// Tracks the class in the reflection system
    /// </summary>
    static void TrackClass(PropertyMap& properties)
    {
      //property_maps[properties.GetUUID()] = std::make_shared<PropertyMap>(properties);
    }

    /// <summary>
    /// Removes the class from the reflection system
    /// </summary>
    static void UntrackClass(const std::string& name)
    { 

      for (auto it = property_maps.begin(); it != property_maps.end(); ++it)
      {
        if (it->second->GetName() == name)
        {
          //property_maps.erase(it);
          return;
        }
      }

    }

    static PropertyMapMap property_maps;
  };

}


// register a class for serialization
// adds the necessary methods to serialize a class
#define FLX_REFL_SERIALIZABLE \
  private:
  //public: \
  //FlexEngine::UUID uuid; \

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


// register a class for serialization

// start the registration of a class
// put this at the very bottom of the class declaration
// this is to ensure that the properties are registered after the class is fully defined
// 
// example:
// FLX_REFL_REGISTER_START
//   FLX_REFL_REGISTER_PROPERTY(x)
//   FLX_REFL_REGISTER_REFERENCE(array)
// FLX_REFL_REGISTER_END
#define FLX_REFL_REGISTER_START \
  public: \
  FlexEngine::PropertyMap properties = {{


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
      #PROPERTY, \
      [this]() { return PROPERTY; }, \
      [this](decltype(PROPERTY) value) { PROPERTY = value; } \
    ) \
  },

// add a reference to the reflection system
#define FLX_REFL_REGISTER_REFERENCE(REFERENCE) \
  { \
    #REFERENCE, \
    std::make_shared< \
      FlexEngine::Property< \
      decltype(REFERENCE), \
      std::function<decltype(REFERENCE)()>, \
      std::function<void(decltype(REFERENCE))> \
      > \
    >( \
      #REFERENCE, \
      &REFERENCE \
    ) \
  },


// end the registration of a class
#define FLX_REFL_REGISTER_END \
  }};

// end the registration of a class
// and link the class to the reflection system
#define FLX_REFL_REGISTER_END_AND_LINK(CLASS) \
  }, #CLASS }; \
  friend class PropertyBase; \
  friend class PropertyMap; //\
  void REFL_TrackClass() { FlexEngine::ReflectionSystem::TrackClass(#CLASS, properties); } \
  void REFL_UntrackClass() { FlexEngine::ReflectionSystem::UntrackClass(#CLASS); }



// place in the constructor to add the class's properties to the reflection system
#define FLX_REFL_TRACK(CLASS) REFL_TrackClass();

// place in the destructor to remove the class's properties from the reflection system
#define FLX_REFL_UNTRACK(CLASS) REFL_UntrackClass();