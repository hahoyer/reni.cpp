#include "Import.h"
#include "SourcePosition.h"

#include "../Ref.h"
#include "Source.h"
#include "SourcePart.h"

using namespace HWLib::Match;

p_implementation(SourcePosition, bool, IsEnd)
{
    return _source->IsEnd(_position);
}

p_implementation(SourcePosition, Optional<int>, End)
{
    if (IsEnd)
        return 0;
    return Optional<int>();
}

p_implementation(SourcePosition, char, First)
{
    if (IsEnd)
        return 0;
    return Part(1)[0];
}

SourcePart const SourcePosition::Span(int count)const
{
    return SourcePart(_source, _position, count);
}

String const SourcePosition::Part(int count)const
{
    return _source->Part(_position, count);
}

bool const SourcePosition::BeginsWith(String value)const
{
    return _source->BeginsWith(_position, value);
}

p_implementation(SourcePosition, String, DumpCurrent)
{
    return IsEnd ? String() : String(First);
}

const int DumpWidth = 100;

p_implementation(SourcePosition, String, DumpAfterCurrent)
{
    if (IsEnd)
        return "";
    auto length = min(DumpWidth, _source->Count - _position - 1);
    auto result = _source->Part(_position + 1, length);
    if (length == DumpWidth)
        result += "...";
    return result;
}

p_implementation(SourcePosition, String, DumpBeforeCurrent)
{
    auto start = max(0, _position - DumpWidth);
    auto result = _source->Part(start, _position - start);
    if (_position >= DumpWidth)
        result = "..." + result;
    return result;
}

p_implementation(SourcePosition, String, Dump)
{
    return (DumpBeforeCurrent + "[" + DumpCurrent + "]" + DumpAfterCurrent)
        .Quote;
}

//////////////////////////////////////

SourcePart::operator String const()const{ return _source->Part(_position, _count); }

p_implementation(SourcePart, String, DumpCurrent)
{
    return *this;
}

p_implementation(SourcePart, String, DumpAfterCurrent)
{
    if (_source->IsEnd(_position+_count))
        return "";
    auto length = min(DumpWidth, _source->Count - _position - _count);
    auto result = _source->Part(_position + _count, length);
    if (length == DumpWidth)
        result += "...";
    return result;
}

p_implementation(SourcePart, String, DumpBeforeCurrent)
{
    auto start = max(0, _position - DumpWidth);
    auto result = _source->Part(start, _position - start);
    if (_position >= DumpWidth)
        result = "..." + result;
    return result;
}

p_implementation(SourcePart, String, Dump)
{
    return (DumpBeforeCurrent + "[" + DumpCurrent + "]" + DumpAfterCurrent)
        .Quote;
}

