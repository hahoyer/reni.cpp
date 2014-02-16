#pragma once

#include "SourcePosition.h"

namespace HWLang{

    class Source;

    class SourcePart final : public DumpableObject
    {
        using baseType = DumpableObject;
        using thisType = SourcePart;

        Ref<Source const> const _source;
        int const _position;
        int const _count;
    public:
        SourcePart(Ref<Source const> source, int position, int count)
            : _source(source)
            , _position(position)
            , _count(count)
        {
            SetDumpString();
        };

        p(bool, IsStart);
        p(bool, IsEnd);
        operator String const()const;
    private:
        override_p_function(Array<String>, DumpData);
        p(String, DumpCurrent);
        p(String, DumpAfterCurrent);
        p(String, DumpBeforeCurrent);
    };
}