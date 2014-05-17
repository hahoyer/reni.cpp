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
        int const _position;
        int const _count;
    public:
        SourcePart(Ref<Source> source, int position, int count)
            : _source(source)
            , _position(position)
            , _count(count)
        {
        };

        p(bool, IsStart);
        p(bool, IsEnd);
        operator String const()const;
    private:
        p_function(Array<String>,DumpData) override;
        p_function(String, DumpShort) override;
        p(String, DumpCurrent);
        p(String, DumpAfterCurrent);
        p(String, DumpBeforeCurrent);
    };
}