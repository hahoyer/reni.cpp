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


HW_PR_IMPL_GETTER(SourcePosition, bool, IsEnd){
    return _source->IsEnd(_position);
}

HW_PR_IMPL_GETTER(SourcePosition, Optional<size_t>, End){
    if (IsEnd)
        return 0;
    return Optional<size_t>(
    )
    ;
}

HW_PR_IMPL_GETTER(SourcePosition, char, First){
    if (IsEnd)
        return 0;
    return Part(1)[0];
}

SourcePart const SourcePosition::Span(size_t count)const{
    return SourcePart(_source, _position, count);
}

string SourcePosition::Part(size_t count) const
{
    return _source->Part(_position, count);
}

bool SourcePosition::BeginsWith(string const&value) const
{
    return _source->BeginsWith(_position, value);
}

HW_PR_IMPL_GETTER(SourcePosition, string, DumpCurrent){
    return IsEnd ? string() : string(1,First);
}

const size_t DumpWidth = 100;

HW_PR_IMPL_GETTER(SourcePosition, string, DumpAfterCurrent){
    return _source->DumpAfterCurrent(_position, 1, DumpWidth);
}

HW_PR_IMPL_GETTER(SourcePosition, string, DumpBeforeCurrent){
    return _source->DumpBeforeCurrent(_position, DumpWidth);
}

HW_PR_IMPL_GETTER(SourcePosition, Array<string>, DumpData){
    auto result = (_source->DumpAroundCurrent(_position, 1, DumpWidth))|Quote();
    return{ result };
}

//////////////////////////////////////

SourcePart::operator string const()const{
    return _source->Part(_position, _count);
}

HW_PR_IMPL_GETTER(SourcePart, string, DumpCurrent){
    return *this;
}

HW_PR_IMPL_GETTER(SourcePart, bool, IsStart){
    return _position == 0 && _count == 0;
}

HW_PR_IMPL_GETTER(SourcePart, bool, IsEnd){
    return _source->IsEnd(_position);
}

HW_PR_IMPL_GETTER(SourcePart, string, DumpAfterCurrent){
    return _source->DumpAfterCurrent(_position, _count, DumpWidth);
}

HW_PR_IMPL_GETTER(SourcePart, string, DumpBeforeCurrent){
    return _source->DumpBeforeCurrent(_position, DumpWidth);
}


HW_PR_IMPL_GETTER(SourcePart, Array<string>, DumpData){
    auto result = (_source->DumpAroundCurrent(_position, _count, DumpWidth))|Quote();
    return{ result };
}

HW_PR_IMPL_GETTER(SourcePart, string, DumpShort){
    return _source->DumpAroundCurrent(_position, _count, 10);
}