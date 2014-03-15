#pragma once

#include "SourcePosition.h"

using namespace HWLib;

namespace HWLang{

    class Source;

    class SourcePart final : public DumpableObject
    {
        using baseType = DumpableObject;
        using thisType = SourcePart;

        CtrlRef<Source > const _source;
        int const _position;
        int const _count;
    public:
        SourcePart(CtrlRef<Source> source, int position, int count)
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
        p_function(Array<String>,DumpData) override;
        p(String, DumpCurrent);
        p(String, DumpAfterCurrent);
        p(String, DumpBeforeCurrent);
    };
}