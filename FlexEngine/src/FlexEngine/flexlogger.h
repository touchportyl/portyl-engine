#pragma once

#include <filesystem>
#include <fstream>
#include <string>

#define FLX_FLOW_FUNCTION()   FlexEngine::Log::Flow(__FUNCTION__)
#define FLX_FLOW_BEGINSCOPE() FLX_FLOW_FUNCTION(); FlexEngine::Log::UpdateFlowScope(1)
#define FLX_FLOW_ENDSCOPE()   FlexEngine::Log::UpdateFlowScope(-1); FLX_FLOW_FUNCTION()

namespace FlexEngine
{
  class Log
  {
  public:
    enum WarningLevel
    {
      _Debug = 0,
      _Flow,
      _Info,
      _Warning,
      _Error,
      _Fatal,
      Last = _Fatal
    };

    Log();
    ~Log();

    /// <summary>
    /// User-defined debug messages
    /// </summary>
    static void Debug(const char* message)   { Logger(_Debug, message); }
    static void Debug(std::string message)   { Debug(message.c_str()); }
    /// <summary>
    /// Messages for debugging the flow of the application
    /// <para>Use the FLX_FLOW_FUNCTION() macro</para>
    /// <para>This is not meant to be used directly</para>
    /// </summary>
    static void Flow (const char* message)   { Logger(_Flow, message); }
    static void Flow (std::string message)   { Flow(message.c_str()); }
    /// <summary>
    /// Informational messages
    /// </summary>
    static void Info (const char* message)   { Logger(_Info, message); }
    static void Info (std::string message)   { Info(message.c_str()); }
    /// <summary>
    /// Warn if it could cause issues when not addressed
    /// </summary>
    static void Warning(const char* message) { Logger(_Warning, message); }
    static void Warning(std::string message) { Warning(message.c_str()); }
    /// <summary>
    /// Warn about potentially damaging errors, systems can recover if dealt with
    /// </summary>
    static void Error(const char* message)   { Logger(_Error, message); }
    static void Error(std::string message)   { Error(message.c_str()); }
    /// <summary>
    /// Kills the application asap to prevent it from damaging the system
    /// </summary>
    static void Fatal(const char* message)   { Logger(_Fatal, message); }
    static void Fatal(std::string message)   { Fatal(message.c_str()); }

    /// <summary>
    /// Dumps the logs to a file
    /// </summary>
    static void DumpLogs(void);

    /// <summary>
    /// This flag is set to true when a fatal error is logged
    /// <para>It is used to determine if the application should dump logs when closing</para>
    /// </summary>
    static bool IsFatal(void) { return is_fatal; }

    /// <summary>
    /// Adds indentations to the flow to make it easier to read
    /// <para>Use the FLX_FLOW_BEGINSCOPE() and FLX_FLOW_ENDSCOPE() macros</para>
    /// <para>This is not meant to be used directly</para>
    /// </summary>
    static void UpdateFlowScope(int indent) { flow_scope += indent; }

  private:
    static void Logger(WarningLevel level, const char* message);

    static std::filesystem::path log_base_path;
    static std::filesystem::path log_file_path;
    static std::fstream log_stream;
    static bool is_fatal;
    static int flow_scope;
  };
}
