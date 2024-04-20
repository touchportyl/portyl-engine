#pragma once

#include <iostream>
#include <chrono>
#include <string>


namespace FlexEngine
{

  /// <summary>
  /// Ease of use wrapper class that provides date and time functionality.
  /// <para>Format the date using the GetFormattedDateTime method.</para>
  /// </summary>
  class DateTime
  {
  public:
    /// <summary>
    /// Date structure that holds the year, month, and day.
    /// </summary>
    struct Date
    {
      int year;
      int month;
      int day;

      /// <summary>
      /// Default constructor that initializes the date with the current date.
      /// </summary>
      Date();

      /// <summary>
      /// Parameterized constructor that initializes the date with the given year, month, and day.
      /// </summary>
      Date(int year, int month, int day);

      /// <returns>Converts the date to a string in the format: %Y-%m-%d</returns>
      std::string ToString() const;

      // comparison operators

      bool operator==(const Date& other) const;
      bool operator!=(const Date& other) const;
      bool operator<(const Date& other) const;
      bool operator<=(const Date& other) const;
      bool operator>(const Date& other) const;
      bool operator>=(const Date& other) const;
    };

  public:
    // remove constructor
    DateTime() = delete;
    DateTime(const DateTime&) = delete;
    DateTime(DateTime&&) = delete;
    DateTime& operator=(const DateTime&) = delete;
    DateTime& operator=(DateTime&&) = delete;

    /// <summary>
    /// Gets the date in std::tm format.
    /// </summary>
    static std::tm GetDateTime();

    /// <summary>
    /// Gets the formatted date in the string.
    /// </summary>
    static std::string GetFormattedDateTime(const char* fmt = "%Y-%m-%d %X");

    /// <summary>
    /// Gets the date in the Date structure.
    /// </summary>
    static Date GetDate();

  };

}