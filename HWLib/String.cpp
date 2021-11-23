#include "Import.h"
#include "String.h"

#include "DumpMacros.h"
#include "DumpToString.h"
#include "File.h"
#include "Optional.h"
#include "TemplateInstances.h"

using namespace HWLib;
using namespace std;

String::String(const char data)
  : _data(1, data) {}

String::String(const char* data)
  : _data(data) {}

String::String(size_t count, const char* data)
  : _data(data, count) {}

String::String(const std::string& data)
  : _data(data) {}

String::String(const Array<const char>& other)
  : _data(other.RawData, other.Count) {}

String::String() = default;

Array<const char> String::GetToArray(const string& target)
{
  const auto string = target.c_str();
  return Numbers(target.size())
         ->Select<const char>([&](size_t i) { return string[i]; })
         ->ToArray;
}

function<Optional<size_t> (const std::string&)> HWLib::Find(std::string expected, size_t start)
{
  return Pivot(&String::Find, expected, start);
}


p_implementation(String, char const*, RawData) { return _data.c_str(); }


p_implementation(String, char*, RawDataCopy)
{
  const auto result = new char[Count + 1];
  const auto data = _data.c_str();
  memcpy(result, data, Count);
  result[Count] = '\0'; // don't forget the terminating 0
  return result;
}


p_implementation(String, std::size_t, HashCode) { return std::hash<std::string>()(_data); };


string CharQuote(const char character)
{
  switch(character)
  {
  case '\\':
  case '"':
  case '\n':
  case '\t':
  case '\r':
  case '\f':
    return "\\" + character;
  default: ;
    static auto hex = "0123456789abcdef";
    if(character < 16)
      return "\\0x0" + string(1, hex[character]);
    return string(1, character);
  }
}

string String::GetQuote(const string& target)
{
  return
    (target | ToArray())
    .Aggregate<string>
    (
      "\"",
      [=](string head, const char next)
      {
        return head + CharQuote(next);
      }
    )
    + "\"";
}

bool String::operator==(const String& other) const { return _data == other._data; }
bool String::operator<(const String& other) const { return _data < other._data; }

auto String::CastLeft(const string& target, size_t count, char padChar) -> string
{
  if(count == target.size())
    return target;
  if(count < target.size())
    return target.substr(target.size() - count, {});
  return string(count - target.size(), padChar) + target;
}

auto String::CastRight(const string& target, size_t count, char padChar) -> string
{
  if(count == target.size())
    return target;
  if(count < target.size())
    return target.substr(0, target.size() - count);
  return target + string(count - target.size(), padChar);
}

char String::operator[](size_t index) const { return _data[index]; }

string String::Indent(const std::string& target, bool isLineStart, size_t count, string tabString)
{
  if(count == 0)
    return target;

  const auto effectiveTabString = tabString * count;
  return (isLineStart ? effectiveTabString : "") + Replace(target, "\n", "\n" + effectiveTabString);
}

bool String::Contains(const std::string& target, std::string expected, size_t start)
{
  return Find(target, expected, start).IsValid;
}

Optional<size_t> String::Find(const string& target, string expected, size_t start)
{
  const auto result = target.find(expected, start);
  if(result == string::npos)
    return {};
  return result;
}

bool String::Contains1(const std::string& target, char expected, size_t start)
{
  for(; start < target.size(); start++)
    if(target[start] == expected)
      return true;
  return false;
}

std::string String::Stringify(const Enumerable<std::string>& list, std::string delimiter)
{
  const auto array = list.ToArray;

  const auto count = array.Count;
  if(count == 0)
    return "";
  std::string result;

  const auto counts =
    array
    .Select<size_t>([](const String& element) { return element.Count; })
    ->ToArray;
  const auto length = counts.Sum();
  result.reserve(length + (count - 1) * delimiter.size());

  auto useDelimiter = false;
  for(const auto element : array)
  {
    if(useDelimiter)
      result += delimiter;
    useDelimiter = true;
    result += element;
  }
  return result;
}

void String::operator+=(const String& other)
{
  _data += other._data;
}

bool String::BeginsWith(const std::string& target, std::string expected)
{
  for(auto i = 0; i < target.size() && i < expected.size(); i++)
    if(target[i] != expected[i])
      return false;
  return true;
}

bool String::EndsWith(const std::string& target, std::string expected)
{
  if(target.size() < expected.size())
    return false;
  const auto delta = target.size() - expected.size();

  for(auto i = 0; i < expected.size(); i++)
    if(target[delta + i] != expected[i])
      return false;

  return true;
}

string String::Replace(const string& target, string oldValue, string newValue)
{
  auto split = Split(target, oldValue);
  return split->Stringify(newValue);
}


class SplitIterator final : public Enumerable<string>::Iterator
{
  using baseType = Iterator;
  using thisType = SplitIterator;
  using parentType = Enumerable<char>::Iterator;
  const string _parent;
  const string _delimiter;
  size_t _index;

public:
  SplitIterator(const string& parent, const string& delimiter)
    : _parent(parent)
      , _delimiter(delimiter)
      , _index(0) { }

protected:
  p_function(bool, IsValid) override
  {
    return _index < _parent.size();
  }

  virtual string Step() override
  {
    const auto start = _index;
    auto newEnd = _parent | Find(_delimiter, start);
    if(!newEnd.IsValid)
      newEnd = _parent.size();
    _index = newEnd + _delimiter.size();
    return _parent.substr(start, newEnd - start);
  }
};

CtrlRef<Enumerable<string>> String::Split(const std::string& target, string delimiter)
{
  HW_ASSERT_IS(delimiter, !=, "");
  return new Enumerable<string>::Container(new SplitIterator(target, delimiter));
}

string String::Convert(unsigned __int64 value, int radix)
{
  if(value == 0)
    return "0";

  HW_ASSERT_IS(radix, >, 1);
  HW_ASSERT_IS(radix, <, 36);

  string result;
  do
  {
    constexpr auto digits = "0123456789abcdefghijklmnopqrstuvwxyz";
    const auto digit = value % radix;
    result = string(1, digits[digit]) + result;
    value /= radix;
  }
  while(value);

  return result;
};


string String::Convert(__int64 value, int radix)
{
  if(value < 0)
    return "-" + Convert(static_cast<unsigned long long>(-value), radix);
  return Convert(static_cast<unsigned long long>(value), radix);
};


string String::Convert(bool value)
{
  return value ? string("true") : "false";
};


string String::Surround(const string& left, const Enumerable<string>& list, const string& right, int maxCount)
{
  switch(list.Count)
  {
  case 0:
    return left + right;
  case 1:
    if(!(list.First | HWLib::Contains('\n')) && list.First.size() < maxCount)
      return left + " " + list.First + " " + right;
  default:
    return left + ("\n" + (list | HWLib::Stringify("\n")) | HWLib::Indent()) + "\n" + right;
  };
};


string HWLib::operator*(const string& left, size_t count)
{
  string result;
  for(auto i = 0; i < count; i++)
    result += left;
  return result;
}

string String::FilePosition(const string& fileName, size_t line, size_t column, string flag)
{
  return fileName
    + "("
    + to_string(line)
    + (column ? "," + to_string(column - 1) : "")
    + "): "
    + flag
    + ": ";
};
