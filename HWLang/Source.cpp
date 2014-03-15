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


p_implementation(Source, int, Count)
{
    return Text.Count;
}


bool const Source::IsEnd(int position)const
{
    return Count <= position;
}

String const Source::Part(int start, int count)const
{
    return Text.Part(start, count);
}

String const Source::FilePosition(int position, String flagText, String tag)const
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


int const Source::LineNr(int iEnd)const
{
    return Text
        .ToArray
        .Take(iEnd)
        ->Where([](char c){return c == '\n'; })
        ->Count;
}


int const Source::ColNr(int position)const
{
    return Text
        .ToArray
        .Take(position)
        ->Aggregate<int>(0, [](int current, char const c)
    {return c == '\n' ? 0 : current + 1; });
}

SourcePosition const Source::operator +(int position)const{
    return SourcePosition(*this, position);
}

SourcePosition const HWLang::operator +(CtrlRef<Source> const& source, int position){
    return SourcePosition(source, position);
}

bool const Source::BeginsWith(int position, String value)const
{
    return Text.BeginsWith(value, position);
}

p_implementation(Source, Array<String>, DumpData){
    return Array<String>{ FilePosition(0, "", "")};
}

