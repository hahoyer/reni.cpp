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

        HW_PR_GET(bool, IsStart);
        HW_PR_GET(bool, IsEnd);
        operator string const()const;
    private:
        HW_PR_DECL_GETTER(Array<string>,DumpData) override;
        HW_PR_DECL_GETTER(string, DumpShort) override;
        HW_PR_GET(string, DumpCurrent);
        HW_PR_GET(string, DumpAfterCurrent);
        HW_PR_GET(string, DumpBeforeCurrent);
    };
}