#include "Import.h"
#include "SourcePosition.h"

#include "Source.h"
#include "SourcePart.h"

using namespace Reni;

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


SourcePart const SourcePosition::Span(int count)const
{
    return SourcePart(_source, _position, count);
}

String const SourcePosition::Part(int count)const
{
    return _source->Part(_position, count);
}


SourcePart::operator String const()const{ return _source->Part(_position, _count); }

