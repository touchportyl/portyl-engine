#include "pch.h"

#include <Windows.h> // SetFileAttributes

#include "datetime.h"

#include "Core/application.h"

namespace FlexEngine
{
  std::filesystem::path Log::log_base_path{ std::filesystem::current_path() / ".log" };
  std::filesystem::path Log::log_file_path{ log_base_path / "~$flexapp.log" };
  File Log::log_stream{};
  bool Log::is_fatal = false;
  int Log::flow_scope = 0;

  Log::Log()
  {
    // create logs directory
    if (!std::filesystem::exists(log_base_path))
    {
      std::filesystem::create_directory(log_base_path);
    }
    // hide log folder
    SetFileAttributes(log_base_path.c_str(), FILE_ATTRIBUTE_HIDDEN);

    // open log file
    log_stream.open(log_file_path.string(), std::ios::out | std::ios::app); // can't use std::ios::trunc
    FLX_INTERNAL_ASSERT(log_stream.is_open(), "Could not open log file.");
    
    // hide temporary log file
    SetFileAttributes(log_file_path.c_str(), FILE_ATTRIBUTE_HIDDEN);

#ifdef _DEBUG
    // file will be opened and updated frequently in debug mode
    log_stream.close();
#endif

    FLX_FLOW_BEGINSCOPE();
  }
  Log::~Log()
  {
    FLX_FLOW_ENDSCOPE();

#ifdef _DEBUG
    // dump logs if debugging
    Log::DumpLogs();
#endif

    // close log file
    log_stream.close();

    // remove temporary log file
    FLX_INTERNAL_ASSERT(std::filesystem::remove(log_file_path), "Error removing temporary log file.");
  }

  void Log::Logger(WarningLevel level, const char* message)
  {
    // create string stream
    std::stringstream ss;

    // print time
    ss << "[" << DateTime::GetFormattedDateTime() << "] ";

    // append warning level
    switch (level)
    {
    case WarningLevel::_Debug:   ss << "Debug -> ";   break;
    case WarningLevel::_Flow:    ss << "Flow -> ";    break;
    case WarningLevel::_Info:    ss << "Info -> ";    break;
    case WarningLevel::_Warning: ss << "Warning -> "; break;
    case WarningLevel::_Error:   ss << "Error -> ";   break;
    case WarningLevel::_Fatal:   ss << "Fatal -> ";   break;
    default:                     ss << "Unknown -> "; break;
    }

    // append flow scope
    if (level == WarningLevel::_Flow)
    {
      for (int i = 0; i < flow_scope; i++)
      {
        ss << "| ";
      }
    }

    // append message
    ss << message << "\n";

    // log to console and file based on warning level
#ifdef _DEBUG
    std::cout << ss.str();

    // file will be opened and updated frequently in debug mode
    log_stream.open(log_file_path.string(), std::ios::out | std::ios::app);
    log_stream << ss.str();
    log_stream.close();
#else
    // log everything except debug messages to console
    if (level != WarningLevel::_Debug)
    {
      std::cout << ss.str();
    }

    // full logs will be saved in release mode
    if (!log_stream.is_open())
    {
      log_stream.open(log_file_path.string(), std::ios::out | std::ios::app);
    }
    log_stream << ss.str();
#endif

    // quit application if fatal
    if (level == WarningLevel::_Fatal)
    {
      Log::DumpLogs();
      std::exit(EXIT_FAILURE);
    }
  }
  
  void Log::DumpLogs(void)
  {
    // close temporary file to save contents
    log_stream.close();

    // get filename
    std::stringstream filename{};
    filename << DateTime::GetFormattedDateTime("%Y-%m-%d") << ".log";
    std::filesystem::path save_path = log_base_path / filename.str();

    // save log file
    bool success = false;
    success = std::filesystem::copy_file(log_file_path, save_path, std::filesystem::copy_options::overwrite_existing);
    if (success)
    {
      SetFileAttributes(save_path.c_str(), FILE_ATTRIBUTE_NORMAL);
      Debug("Log file saved to " + save_path.string());
    }
    else
    {
      Error("Error copying log file.");
    }

  }
}
