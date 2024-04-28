#include "datetime.h"

namespace FlexEngine
{

  Date::Date()
  {
    auto time = DateTime::GetDateTime();
    year = time.tm_year + 1900;
    month = time.tm_mon + 1;
    day = time.tm_mday;
  }

  Date::Date(int year, int month, int day)
    : year(year), month(month), day(day)
  {
  }

  Date Date::Now()
  {
    return Date();
  }

  #pragma region Operator Overloads

  bool Date::operator==(const Date& other) const
  {
    return year == other.year && month == other.month && day == other.day;
  }

  bool Date::operator!=(const Date& other) const
  {
    return !(*this == other);
  }

  bool Date::operator<(const Date& other) const
  {
    if (year < other.year) return true;
    if (year > other.year) return false;
    if (month < other.month) return true;
    if (month > other.month) return false;
    return day < other.day;
  }

  bool Date::operator>(const Date& other) const
  {
    if (year > other.year) return true;
    if (year < other.year) return false;
    if (month > other.month) return true;
    if (month < other.month) return false;
    return day > other.day;
  }

  bool Date::operator<=(const Date& other) const
  {
    return *this < other || *this == other;
  }

  bool Date::operator>=(const Date& other) const
  {
    return *this > other || *this == other;
  }

  #pragma endregion

  #pragma region Stringify

  // Converts the date to a string in the format: %Y-%m-%d
  std::string Date::ToString() const
  {
    return std::to_string(year) + "-" + std::to_string(month) + "-" + std::to_string(day);
  }

  Date::operator std::string() const
  {
    return ToString();
  }

  #pragma endregion

}