#include "pch.h"

#include "flexformatter.h"

namespace FlexEngine
{

  struct FlexFormatHeader
  {
    std::string format = "flxfmt";
    int format_version = FLXFMT_VERSION;
    DateTime::Date created = DateTime::Date();
    DateTime::Date last_edited = DateTime::Date();
    int save_version = 1;
  };

  // static member initialization
  FlexFormatHeader FlexFormatter::header;

  Document FlexFormatter::Load(std::istream& is)
  {
    Document document;
    // parse file into document
    IStreamWrapper isw(is);
    document.ParseStream(isw);

    // check for parse errors
    // if there is an error, return an empty document
    if (document.HasParseError())
    {
      Log::Error("FlexFormatter::Load: Parse error: " + document.GetParseError());
      Document empty;
      return empty;
    }

    // save header data
    header.format = document["format"].GetString();
    header.format_version = document["format_version"].GetInt();
    std::string created = document["created"].GetString();
    header.created = DateTime::Date(
      std::stoi(created.substr(0, 4)),
      std::stoi(created.substr(5, 2)),
      std::stoi(created.substr(8, 2))
    );
    std::string last_edited = document["last_edited"].GetString();
    header.last_edited = DateTime::Date(
      std::stoi(last_edited.substr(0, 4)),
      std::stoi(last_edited.substr(5, 2)),
      std::stoi(last_edited.substr(8, 2))
    );
    header.save_version = document["save_version"].GetInt();

    return document;
  }

  std::ostream& FlexFormatter::Format(std::ostream& os, const std::string& scene_data)
  {
    header.last_edited = DateTime::Date(); // update last edited date
    os
      << R"({"format":")" << header.format
      << R"(","format_version":)" << header.format_version
      << R"(,"created":")" << header.created.ToString()
      << R"(","last_edited":")" << header.last_edited.ToString()
      << R"(","save_version":)" << header.save_version
      << R"(,"data":[)" << scene_data
      << "]}";
    return os;
  }

}