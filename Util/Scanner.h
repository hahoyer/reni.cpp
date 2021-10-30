#pragma once
#include "../HWLib/CtrlRef.h"
#include "../HWLang/SourcePosition.h"
#include "../HWLib/Optional.h"
#include "../HWLib/String.h"

using namespace HWLib;
using namespace HWLang;

namespace Util
{
    class Scanner final
    {
        class internal;
        CtrlRef<internal const> const _internal;
    public:
        Scanner();

        static size_t const WhiteSpace(SourcePosition const&position);
        static Optional<size_t> const Number(SourcePosition const&position);
        static Optional<size_t> const Text(SourcePosition const&position);
        static Optional<size_t> const Any(SourcePosition const&position);

        class Error final: public exception
        {
        public:
            Error(size_t const count, String const& id)
                : Count(count)
                , Id(id)
            {}
            size_t const Count;
            String const Id;
        };
    };

}