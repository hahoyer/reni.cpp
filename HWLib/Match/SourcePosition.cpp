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


SourcePart::operator String const()const{ return _source->Part(_position, _count); }


