#include "Import.h"
#include "Source.h"

#include "SourcePosition.h"
#include "../File.h"

using namespace HWLib::Match;

Source::Source(String const& fileName)
: _fileName(fileName)
, _textCache([&]{return HWLib::File(_fileName).Data; })
{}


Source::Source(Source const& other)
: Source(other._fileName)
{}


p_implementation(Source, String, Text)
{
    return *_textCache.Value;
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
    if (_fileName == "")
        return "????";
    auto file = HWLib::File(_fileName);
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
        ->Count([](char c){return c == '\n'; });
}


int const Source::ColNr(int position)const
{
    return Text
        .ToArray
        .Take(position)
        ->Aggregate<int>(0, [](int current, char const c)
    {return c == '\n' ? 0 : current + 1; });
}


SourcePosition const HWLib::Match::operator +(Ref<Source const> const source, int position)
{
    return SourcePosition(source, position);
}


bool const Source::BeginsWith(int position, String value)const
{
    return Text.BeginsWith(value, position);
}


#include "../TemplateInstances.h"