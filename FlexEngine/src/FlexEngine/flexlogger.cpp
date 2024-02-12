#include "pch.h"

#include "flexlogger.h"

#include <Windows.h> // SetFileAttributes

#include <chrono>

#include "Core/application.h"

namespace FlexEngine
{
  std::filesystem::path Log::log_base_path{ std::filesystem::current_path() / ".log" };
  std::filesystem::path Log::log_file_path{ log_base_path / "~$app.log" };
  File Log::log_history{};
  bool Log::is_fatal = false;
  int Log::flow_scope = 0;

  Log::Log()
  {
    FE_FLOW_BEGINSCOPE();

    // create logs directory
    if (!std::filesystem::exists(log_base_path))
    {
      std::filesystem::create_directory(log_base_path);
    }
    // hide log folder
    SetFileAttributes(log_base_path.c_str(), FILE_ATTRIBUTE_HIDDEN);

    // create temporary log file (so that it can be viewed in the folder)
    std::ofstream ofs(log_file_path.string(), std::ios::out);
    if (ofs)
    {
      ofs.close();
    }
    else
    {
      Fatal("Could not create log file.");
    }

    // open log file
    log_history.open(log_file_path.string(), std::ios::out | std::ios::trunc);
    if (!log_history.is_open())
    {
      Fatal("Could not open log file.");
    }

    // hide temporary log file
    SetFileAttributes(log_file_path.c_str(), FILE_ATTRIBUTE_HIDDEN);

#ifdef _DEBUG
    // file will be opened and updated frequently in debug mode
    log_history.close();
#endif
  }
  Log::~Log()
  {
    // close file
    log_history.close();

    // remove temporary log file
    try
    {
      std::filesystem::remove(log_file_path);
    }
    catch (const std::filesystem::filesystem_error& e)
    {
      std::stringstream error{};
      error << "Error removing file: " << e.what() << '\n';
      Error(error.str());
    }

    FE_FLOW_ENDSCOPE();
  }

  std::tm Log::GetTime(void)
  {
    auto const now = std::chrono::system_clock::now();
    auto const time = std::chrono::system_clock::to_time_t(now);
    std::tm local_time{};
    localtime_s(&local_time, &time);
    return local_time;
  }

  std::string Log::GetFormattedTime(const char* fmt = "%Y-%m-%d %X")
  {
    auto time = Log::GetTime();

    // Convert time to a local time string
    char buffer[2048]{ 0 };
    std::strftime(buffer, sizeof(buffer), fmt, &time);
    return buffer;
  }

  void Log::Logger(WarningLevel level, const char* message)
  {
    // create string stream
    std::stringstream ss;

    // print time
    ss << "[" << GetFormattedTime() << "] ";

    // append warning level
    switch (level)
    {
    case WarningLevel::_Debug:   ss << "Debug: ";   break;
    case WarningLevel::_Flow:    ss << "Flow: ";    break;
    case WarningLevel::_Info:    ss << "Info: ";    break;
    case WarningLevel::_Warning: ss << "Warning: "; break;
    case WarningLevel::_Error:   ss << "Error: ";   break;
    case WarningLevel::_Fatal:   ss << "Fatal: ";   break;
    default:                     ss << "Unknown: "; break;
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
    log_history.open(log_file_path.string(), std::ios::out | std::ios::app);
    log_history << ss.str();
    log_history.close();
#else
    // only log debug messages to console
    if (level == WarningLevel::_Debug)
    {
      std::cout << ss.str();
    }

    // full logs will be saved in release mode
    if (!log_history.is_open())
    {
      log_history.open(log_file_path.string(), std::ios::out | std::ios::trunc);
    }
    log_history << ss.str();
#endif

    // quit application if fatal
    if (level == WarningLevel::_Fatal)
    {
      is_fatal = true;
      Application::Get().Close();
    }
  }
  
  void Log::DumpLogs(void)
  {
    // get filename
    std::stringstream filename{};
    filename << GetFormattedTime("%Y-%m-%d") << ".log";
    std::filesystem::path save_path = log_base_path / filename.str();

    // save log file
    try
    {
      std::filesystem::copy_file(log_file_path, save_path, std::filesystem::copy_options::overwrite_existing);
      SetFileAttributes(save_path.c_str(), FILE_ATTRIBUTE_NORMAL);
      Debug("Log file saved to " + save_path.string());
    }
    catch (const std::filesystem::filesystem_error& e)
    {
      std::stringstream error{};
      error << "Error copying file: " << e.what() << '\n';
      Error(error.str());
    }
  }
}
