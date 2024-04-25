#pragma once

#include <iostream>
#include <chrono>

#include "flexlogger.h"

#ifdef _DEBUG
#define FLX_SCOPED_TIMER() FlexEngine::Timer timer(__FUNCTION__);
#else
#define FLX_SCOPED_TIMER()
#endif

namespace FlexEngine
{

  /// <summary>
  /// Timer class
  /// <para>Logs the duration of the timer when it goes out of scope</para>
  /// </summary>
  class Timer
  {
  public:
    /// <summary>
    /// Constructor for Timer
    /// </summary>
    /// <param name="log_message">The message to be logged when the timer ends</param>
    Timer(std::string log_message = "")
      : m_log_message(log_message)
    {
      start = std::chrono::steady_clock::now();
    }

    /// <summary>
    /// Destructor for Timer
    /// Logs the message and the duration of the timer
    /// </summary>
    ~Timer()
    {
      end = std::chrono::steady_clock::now();
      duration = end - start;

      float ms = duration.count() * 1000.0f;
      Log::Debug("[Timer] " + m_log_message + " (" + std::to_string(ms) + "ms)");
    }

  private:
    std::chrono::time_point<std::chrono::steady_clock> start, end;
    std::chrono::duration<float> duration{ 0 };
    std::string m_log_message;
  };

}