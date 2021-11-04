#pragma once

#include "SourcePosition.h"
#include "../HWLib/DumpableObject.h"
#include "../HWLib/Ref.h"

using namespace HWLib;

namespace HWLang{

    class Source;

    class SourcePart final : public DumpableObject
    {
        using baseType = DumpableObject;
        using thisType = SourcePart;

        Ref<Source> const _source;
        size_t const _position;
        size_t const _count;
    public:
        SourcePart(SourcePart const&other)
            : _source(other._source)
            , _position(other._position)
            , _count(other._count)
        {
            SetDumpString();
        };

        SourcePart(Ref<Source> source, size_t position, size_t count)
            : _source(source)
            , _position(position)
            , _count(count)
        {
            SetDumpString();
        };

        p(bool, IsStart);
        p(bool, IsEnd);
        operator string const()const;
    private:
        p_function(Array<string>,DumpData) override;
        p_function(string, DumpShort) override;
        p(string, DumpCurrent);
        p(string, DumpAfterCurrent);
        p(string, DumpBeforeCurrent);
    };
}