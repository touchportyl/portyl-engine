#pragma once

#include <iostream>
#include <chrono>


namespace FlexEngine
{

  /// <summary>
  /// Ease of use wrapper class that provides date and time functionality.
  /// <para>Format the date using the GetFormattedDate method.</para>
  /// </summary>
  class Date
  {
  public:

    /// <summary>
    /// Gets the date in std::tm format.
    /// </summary>
    static std::tm GetDate()
    {
      auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
      std::tm tm{}; localtime_s(&tm, &now);
      return tm;
    }

    /// <summary>
    /// Gets the formatted date in the string.
    /// </summary>
    static std::string GetFormattedDate(const char* fmt = "%Y-%m-%d %X")
    {
      auto time = GetDate();

      // convert time to a local time string
      char buffer[512]{ 0 };
      std::strftime(buffer, sizeof(buffer), fmt, &time);
      return buffer;
    }

    /// <returns>Date in the string format.</returns>
    std::string ToString() const { return GetFormattedDate(); }

  };

  std::ostream& operator<<(std::ostream& os, const Date& date)
  {
    os << date.ToString();
    return os;
  }

}