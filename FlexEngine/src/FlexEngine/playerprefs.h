#pragma once

#include "RapidJSON/document.h"

using namespace rapidjson;

namespace FlexEngine
{

  class PlayerPrefs
  {
  public:
    /// <summary>
    /// Loads the preferences from disk.
    /// </summary>
    static void Load();
    /// <summary>
    /// Writes all modified preferences to disk.
    /// </summary>
    /// <param name="prettify: ">Whether to output the json file with indentation and spacing.</param>
    static void Save(bool prettify = false);
    /// <summary>
    /// Removes all keys and values from the preferences. Use with caution.
    /// </summary>
    static void DeleteAll();

    /// <summary>
    /// Removes key and its corresponding value from the preferences.
    /// </summary>
    /// <param name="key: ">The key to remove.</param>
    static void DeleteKey(std::string const& key);
    /// <summary>
    /// Returns true if key exists in the preferences.
    /// </summary>
    /// <param name="key: ">The key to check.</param>
    /// <returns>Returns true if key exists in the preferences.</returns>
    static inline bool HasKey(std::string const& key);

    /// <summary>
    /// Sets the value of the preference identified by key.
    /// </summary>
    /// <param name="key: ">The key to set.</param>
    /// <param name="value: ">The value to set.</param>
    static void SetFloat(std::string const& key, float const& value);
    /// <summary>
    /// Sets the value of the preference identified by key.
    /// </summary>
    /// <param name="key: ">The key to set.</param>
    /// <param name="value: ">The value to set.</param>
    static void SetInt(std::string const& key, int const& value);
    /// <summary>
    /// Sets the value of the preference identified by key.
    /// </summary>
    /// <param name="key: ">The key to set.</param>
    /// <param name="value: ">The value to set.</param>
    static void SetString(std::string const& key, std::string const& value);
    /// <summary>
    /// Sets the value of the preference identified by key.
    /// </summary>
    /// <param name="key: ">The key to set.</param>
    static void SetBool(std::string const& key, bool const& value);

    /// <summary>
    /// Returns the value corresponding to key in the preference file if it exists.
    /// </summary>
    /// <param name="key: ">The key to get.</param>
    /// <param name="default_value: ">The default value to return if key does not exist.</param>
    /// <returns>Returns the value of the key if found, else returns a default value (0.f)</returns>
    static float GetFloat(std::string const& key, float const& default_value = 0.f);
    /// <summary>
    /// Returns the value corresponding to key in the preference file if it exists.
    /// </summary>
    /// <param name="key: ">The key to get.</param>
    /// <param name="default_value: ">The default value to return if key does not exist.</param>
    /// <returns>Returns the value of the key if found, else returns a default value (0)</returns>
    static int GetInt(std::string const& key, int const& default_value = 0);
    /// <summary>
    /// Returns the value corresponding to key in the preference file if it exists.
    /// </summary>
    /// <param name="key: ">The key to get.</param>
    /// <param name="default_value: ">The default value to return if key does not exist.</param>
    /// <returns>Returns the value of the key if found, else returns a default value ("")</returns>
    static std::string GetString(std::string const& key, std::string const& default_value = "");
    /// <summary>
    /// Returns the value corresponding to key in the preference file if it exists.
    /// </summary>
    /// <param name="key: ">The key to get.</param>
    /// <param name="default_value: ">The default value to return if key does not exist.</param>
    /// <returns>Returns the value of the key if found, else returns a default value (false)</returns>
    static bool GetBool(std::string const& key, bool const& default_value = false);

  private:
    PlayerPrefs() = default;
    ~PlayerPrefs() = default;

    static void Create();

    static Document m_document;
  };

}