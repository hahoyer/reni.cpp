#pragma once
//#pragma message(__FILE__ "(" STRING(__LINE__) "): ")

#include "Array.h"
#include "DefaultOperators.h"
#include <string>
#include "ExtensionMethodBase.h"
#include "Properties.h"

namespace HWLib
{
  template <typename T>
  class Optional;

  class String final
  {
    using thisType = String;
    std::string _data;

  public:
    String();
    String(char data);
    String(const char* data);
    String(size_t count, const char* data);
    String(const std::string& other);
    String(const Array<const char>& other);

    HW_DO_PLACEMENT_ASSIGN;

    HW_PR_GET(size_t, Count) { return _data.length(); }
    HW_PR_GET(size_t, HashCode);
    HW_PR_GET(String, Quote) { return GetQuote(Data); };
    HW_PR_GET(std::string, Data) { return _data; };
    HW_PR_GET(char const*, RawData);
    HW_PR_GET(char *, RawDataCopy);

    static bool BeginsWith(const std::string& target, std::string expected);
    static auto CastLeft(const std::string& target, size_t count, char padChar) -> std::string;
    static auto CastRight(const std::string& target, size_t count, char padChar) -> std::string;
    static bool Contains1(const std::string& target, char expected, size_t start);
    static bool Contains(const std::string& target, std::string expected, size_t start);
    static bool EndsWith(const std::string& target, std::string expected);

    static Optional<size_t> Find(const std::string& target, std::string expected, size_t start);
    String operator+(const String& other) const { return String(Data + other.Data); };
    char operator[](size_t index) const;
    bool operator==(const String& other) const;
    bool operator<(const String& other) const;
    void operator+=(const String& other);
    static std::string Replace(const std::string& target, std::string oldValue, std::string newValue);
    static CtrlRef<Enumerable<std::string>> Split(const std::string& target, std::string delimiter);

    static std::string Convert(bool value);
    static std::string Convert(int value, int radix = 10) { return Convert(static_cast<long long>(value), radix); };

    static std::string Convert(unsigned __int32 value, int radix = 10)
    {
      return Convert(static_cast<long long>(value), radix);
    };
    static std::string Convert(unsigned __int64 value, int radix = 10);
    static std::string Convert(__int64 value, int radix = 10);
    static std::string Surround(const std::string& left, const Enumerable<std::string>& list, const std::string& right,
                                int maxCount = 100);
    static std::string Stringify(const Enumerable<std::string>& list, std::string delimiter);

    static std::string FilePosition(const std::string& fileName, size_t line, size_t column, std::string flag);
    static std::string Indent(const std::string& target, bool isLineStart, size_t count, std::string tabString);
    static std::string GetQuote(const std::string& target);
    static Array<const char> GetToArray(const std::string& target);
  };

  static String operator+(const char* left, const String& right) { return String(left) + right; };

  std::string operator*(const std::string&, size_t count);

  inline auto FilePosition(size_t line, size_t column, const std::string& flag)
  {
    return Pivot(&String::FilePosition, line, column, flag);
  }

  inline auto FilePosition(size_t line, size_t column)
  {
    return Pivot(&String::FilePosition, line, column, std::string());
  }

  inline auto Indent(bool isLineStart = false, size_t count = 1, const std::string& tabString = "    ")
  {
    return Pivot(&String::Indent, isLineStart, count, tabString);
  }

  inline auto Split(std::string delimiter) { return Pivot(&String::Split, delimiter); }
  inline auto Quote() { return Pivot(&String::GetQuote); }
  inline auto ToArray() { return Pivot(&String::GetToArray); }
  inline auto BeginsWith(std::string expected) { return Pivot(&String::BeginsWith, expected); }
  inline auto CastLeft(size_t count, char padChar = ' ') { return Pivot(&String::CastLeft, count, padChar); }
  inline auto CastRight(size_t count, char padChar = ' ') { return Pivot(&String::CastRight, count, padChar); }
  inline auto Contains(char expected, size_t start = 0) { return Pivot(&String::Contains1, expected, start); }
  inline auto Contains(std::string expected, size_t start = 0) { return Pivot(&String::Contains, expected, start); }
  inline auto EndsWith(std::string expected) { return Pivot(&String::EndsWith, expected); }

  inline auto Replace(std::string oldValue, std::string newValue)
  {
    return Pivot(&String::Replace, oldValue, newValue);
  }

  function<Optional<size_t> (const std::string&)> Find(std::string expected, size_t start = 0);
  inline auto Stringify(std::string delimiter) { return Pivot(&String::Stringify, delimiter); }
}

namespace std
{
  template <>
  struct hash<const String>
  {
    size_t operator()(const String& key) const
    {
      return key.HashCode;
    }
  };

  template <>
  struct hash<String>
  {
    size_t operator()(const String& key) const
    {
      return key.HashCode;
    }
  };
}


//#pragma message(__FILE__ "(" STRING(__LINE__) "):")
