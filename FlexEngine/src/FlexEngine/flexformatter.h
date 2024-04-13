#pragma once

//#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "datetime.h" // includes iostream

#include <RapidJSON/document.h>
#include <RapidJSON/istreamwrapper.h>
#include <RapidJSON/ostreamwrapper.h>
#include <RapidJSON/writer.h>
#include <RapidJSON/prettywriter.h>
using namespace rapidjson;

/// <summary>
/// The current version of the Flex Formatter.
/// <para>Do not change this value!</para>
/// </summary>
constexpr int FLXFMT_VERSION = 1;

namespace FlexEngine
{

  /// <summary>
  /// Contains the required header information for the FlexFormat file.
  /// </summary>
  struct FlexFormatHeader;

  /// <summary>
  /// Formatter for the FlexFormat file specification.
  /// <para>This class does not handle the actual reading and writing of the file.</para>
  /// </summary>
  class FlexFormatter
  {
  public:
    /// <summary>
    /// Loads the header information and parses the data using RapidJSON.
    /// <para>If there is a parse error, an empty rapidjson::Document is returned.</para>
    /// <para>Currently does not support reading multiple files. Old file header data will be overwritten.</para>
    /// </summary>
    static Document Load(std::istream& is);

    /// <summary>
    /// Formats the data into the Flex Format.
    /// </summary>
    static std::ostream& Format(std::ostream& os, const std::string& scene_data);

  private:
    static FlexFormatHeader header;
  };

}