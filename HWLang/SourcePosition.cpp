#include "Import.h"
#include "SourcePosition.h"

#include "Source.h"
#include "SourcePart.h"
#include "../HWLib/Optional.h"
#include "../HWLib/Common.h"

using namespace HWLang;
using namespace HWLib;

SourcePosition::SourcePosition(Ref<Source> source, size_t position)
    : _source(source)
      , _position(position){
    SetDumpString();
}

SourcePosition::SourcePosition(Source const& source, size_t position)
    : _source(new Source(source))
      , _position(position){
    SetDumpString();
}


p_implementation(SourcePosition, bool, IsEnd){
    return _source->IsEnd(_position);
}

p_implementation(SourcePosition, Optional<size_t>, End){
    if (IsEnd)
        return 0;
    return Optional<size_t>(
    )
    ;
}

p_implementation(SourcePosition, char, First){
    if (IsEnd)
        return 0;
    return Part(1)[0];
}

SourcePart const SourcePosition::Span(size_t count)const{
    return SourcePart(_source, _position, count);
}

String const SourcePosition::Part(size_t count)const{
    return _source->Part(_position, count);
}

bool const SourcePosition::BeginsWith(String value)const{
    return _source->BeginsWith(_position, value);
}

p_implementation(SourcePosition, String, DumpCurrent){
    return IsEnd ? String() : String(First);
}

const size_t DumpWidth = 100;

p_implementation(SourcePosition, String, DumpAfterCurrent){
    return _source->DumpAfterCurrent(_position, 1, DumpWidth);
}

p_implementation(SourcePosition, String, DumpBeforeCurrent){
    return _source->DumpBeforeCurrent(_position, DumpWidth);
}

p_implementation(SourcePosition, Array<String>, DumpData){
    auto result = (_source->DumpAroundCurrent(_position, 1, DumpWidth)).Quote;
    return{ result };
}

//////////////////////////////////////

SourcePart::operator String const()const{
    return _source->Part(_position, _count);
}

p_implementation(SourcePart, String, DumpCurrent){
    return *this;
}

p_implementation(SourcePart, bool, IsStart){
    return _position == 0 && _count == 0;
}

p_implementation(SourcePart, bool, IsEnd){
    return _source->IsEnd(_position);
}

p_implementation(SourcePart, String, DumpAfterCurrent){
    return _source->DumpAfterCurrent(_position, _count, DumpWidth);
}

p_implementation(SourcePart, String, DumpBeforeCurrent){
    return _source->DumpBeforeCurrent(_position, DumpWidth);
}


p_implementation(SourcePart, Array<String>, DumpData){
    auto result = (_source->DumpAroundCurrent(_position, _count, DumpWidth)).Quote;
    return{ result };
}

p_implementation(SourcePart, String, DumpShort){
    return _source->DumpAroundCurrent(_position, _count, 10);
}