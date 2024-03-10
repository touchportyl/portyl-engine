#include "datetime.h"


namespace FlexEngine
{

  #pragma region DateTime::Date

  DateTime::Date::Date()
  {
    auto time = GetDateTime();
    year = time.tm_year + 1900;
    month = time.tm_mon + 1;
    day = time.tm_mday;
  }

  DateTime::Date::Date(int year, int month, int day) : year(year), month(month), day(day) {}

  std::string DateTime::Date::ToString() const
  {
    return std::to_string(year) + "-" + std::to_string(month) + "-" + std::to_string(day);
  }

  #pragma region Comparison Operators

  bool DateTime::Date::operator==(const Date& other) const
  {
    return year == other.year && month == other.month && day == other.day;
  }

  bool DateTime::Date::operator!=(const Date& other) const
  {
    return !(*this == other);
  }

  bool DateTime::Date::operator<(const Date& other) const
  {
    if (year < other.year) return true;
    if (year > other.year) return false;
    if (month < other.month) return true;
    if (month > other.month) return false;
    return day < other.day;
  }

  bool DateTime::Date::operator>(const Date& other) const
  {
    if (year > other.year) return true;
    if (year < other.year) return false;
    if (month > other.month) return true;
    if (month < other.month) return false;
    return day > other.day;
  }

  bool DateTime::Date::operator<=(const Date& other) const
  {
    return *this < other || *this == other;
  }

  bool DateTime::Date::operator>=(const Date& other) const
  {
    return *this > other || *this == other;
  }

  #pragma endregion

  #pragma endregion

  #pragma region DateTime

  std::tm DateTime::GetDateTime()
  {
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm tm{}; localtime_s(&tm, &now);
    return tm;
  }

  std::string DateTime::GetFormattedDateTime(const char* fmt)
  {
    auto time = GetDateTime();

    // convert time to a local time string
    char buffer[256]{ 0 };
    std::strftime(buffer, sizeof(buffer), fmt, &time);
    return buffer;
  }

  DateTime::Date DateTime::GetDate()
  {
    auto time = GetDateTime();
    return Date(time.tm_year + 1900, time.tm_mon + 1, time.tm_mday);
  }

  #pragma endregion

}