#include "Import.h"
#include "Source.h"

#include "SourcePosition.h"
#include "../HWLib/string.h"
#include "../HWLib/File.h"
#include "../HWLib/Ref.h"
#include "../HWLib/RefCountContainer.instance.h"

using namespace HWLang;
using namespace HWLib;

Source::Source(const string& fileName, const string& text)
  : fileName(fileName)
    , textCache([=] { return fileName == "" ? text : File(fileName).Data; })
{}


Source Source::FromFile(const string& fileName)
{
  return Source(fileName, "");
}

Ref<Source> Source::CreateFromFile(const string& fileName)
{
  return new Source(fileName, "");
}

Ref<Source> Source::CreateFromText(const string& text)
{
  return new Source("", text);
}

Source Source::FromText(const string& text)
{
  return Source("", text);
}


Source::Source(const Source& other)
  : fileName(other.fileName)
    , textCache(other.textCache.getValue)
{}


p_implementation(Source, string, Text)
{
  return textCache.Value;
}


p_implementation(Source, size_t, Count)
{
  return Text.size();
}


bool Source::IsEnd(size_t position) const
{
  return Count <= position;
}

string Source::Part(size_t start, size_t count) const
{
  return Text.substr(start);
}

string Source::FilePosition(size_t position, string flagText, string tag) const
{
  if(fileName == "")
    return "????";
  const auto file = File(fileName);
  return String::FilePosition(
      file.FullName,
      LineNr(position),
      ColNr(position) + 1,
      tag != "" ? tag : "Debug")
    + flagText;
}


size_t Source::LineNr(size_t iEnd) const
{
  return (Text | ToArray())
         .Take(iEnd)
         ->Where([](char c) { return c == '\n'; })
         ->Count;
}


size_t Source::ColNr(size_t position) const
{
  return (Text
           | ToArray())
         .Take(position)
         ->Aggregate<size_t>(0, [](size_t current, const char c)
         {
           return c == '\n' ? 0 : current + 1;
         });
}

SourcePosition Source::operator +(size_t position) const
{
  return SourcePosition(*this, position);
}

SourcePosition HWLang::operator +(const Ref<Source>& source, size_t position)
{
  return SourcePosition(source, position);
}

bool Source::BeginsWith(size_t position, string value) const
{
  return (Text .substr(position)) == value;
}

p_implementation(Source, Array<string>, DumpData)
{
  return Array<string>{FilePosition(0, "", "")};
}

string Source::DumpAfterCurrent(size_t position, size_t count, size_t dumpWidth) const
{
  if(IsEnd(position + count))
    return "";
  const auto length = min(dumpWidth, Count - position - count);
  auto result = Part(position + count, length);
  if(length == dumpWidth)
    result += "...";
  return result;
};

string Source::DumpCurrent(size_t position, size_t count) const
{
  return Part(position, count);
}

string Source::DumpBeforeCurrent(size_t position, size_t dumpWidth) const
{
  const auto start = max<size_t>(0, position - dumpWidth);
  auto result = Part(start, position - start);
  if(position >= dumpWidth)
    result = "..." + result;
  return result;
}

string Source::DumpAroundCurrent(size_t position, size_t count, size_t dumpWidth) const
{
  return DumpBeforeCurrent(position, dumpWidth)
    + "[" + DumpCurrent(position, count) + "]"
    + DumpAfterCurrent(position, count, dumpWidth);
}
