#include "Import.h"
#include "Source.h"

#include "SourcePosition.h"
#include "../HWLib/String.h"
#include "../HWLib/File.h"
#include "../HWLib/Ref.h"
#include "../HWLib/RefCountContainer.instance.h"

using namespace HWLang;
using namespace HWLib;

Source::Source(String const& fileName, String const&text)
: fileName(fileName)
, textCache([=]{return fileName == "" ? text : File(fileName).Data; })
{
}


Source const Source::FromFile(String const& fileName)
{
    return Source(fileName,"");
}

Ref<Source>const Source::CreateFromFile(String const& fileName)
{
    return new Source(fileName, "");
}

Ref<Source>const Source::CreateFromText(String const& text)
{
    return new Source("", text);
}

Source const Source::FromText(String const& text)
{
    return Source("", text);
}


Source::Source(Source const& other)
: fileName(other.fileName)
, textCache(other.textCache.getValue)
{}


p_implementation(Source, String, Text)
{
    return textCache.Value;
}


p_implementation(Source, size_t, Count)
{
    return Text.Count;
}


bool const Source::IsEnd(size_t position)const
{
    return Count <= position;
}

String const Source::Part(size_t start, size_t count)const
{
    return Text.Part(start);
}

String const Source::FilePosition(size_t position, String flagText, String tag)const
{
    if (fileName == "")
        return "????";
    auto file = HWLib::File(fileName);
    return String::FilePosition(
        file.FullName,
        LineNr(position),
        ColNr(position) + 1,
        tag != "" ? tag : "Debug")
        + flagText;
}


size_t const Source::LineNr(size_t iEnd)const
{
    return Text
        .ToArray
        .Take(iEnd)
        ->Where([](char c){return c == '\n'; })
        ->Count;
}


size_t const Source::ColNr(size_t position)const
{
    return Text
        .ToArray
        .Take(position)
        ->Aggregate<size_t>(0, [](size_t current, char const c)
    {return c == '\n' ? 0 : current + 1; });
}

SourcePosition const Source::operator +(size_t position)const{
    return SourcePosition(*this, position);
}

SourcePosition const HWLang::operator +(Ref<Source> const& source, size_t position){
    return SourcePosition(source, position);
}

bool const Source::BeginsWith(size_t position, String value)const
{
    return Text.BeginsWith(value, position);
}

p_implementation(Source, Array<String>, DumpData){
    return Array<String>{ FilePosition(0, "", "")};
}

String const Source::DumpAfterCurrent(size_t position, size_t count, size_t dumpWidth)const{
    if(IsEnd(position + count))
        return "";
    auto length = min(dumpWidth, Count - position - count);
    auto result = Part(position + count, length);
    if(length == dumpWidth)
        result += "...";
    return result;
};

String const Source::DumpCurrent(size_t position, size_t count)const{
    return Part(position,count);
}

String const Source::DumpBeforeCurrent(size_t position, size_t dumpWidth)const{
    auto start = max<size_t>(0, position - dumpWidth);
    auto result = Part(start, position - start);
    if(position >= dumpWidth)
        result = "..." + result;
    return result;
}

String const Source::DumpAroundCurrent(size_t position, size_t count, size_t dumpWidth)const{
    return DumpBeforeCurrent(position,dumpWidth) 
        + "[" + DumpCurrent(position, count) + "]" 
        + DumpAfterCurrent(position,count,dumpWidth);
}
