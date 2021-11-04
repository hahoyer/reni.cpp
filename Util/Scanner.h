#pragma once
#include "../HWLib/CtrlRef.h"
#include "../HWLang/SourcePosition.h"
#include "../HWLib/Optional.h"
#include "../HWLib/string.h"

using namespace HWLib;
using namespace HWLang;

namespace Util
{
    class Scanner final
    {
        struct internal;
        CtrlRef<internal const> const _internal;
    public:
        Scanner();

        static size_t WhiteSpace(SourcePosition const& position);
        static Optional<size_t> Number(SourcePosition const& position);
        static Optional<size_t> Text(SourcePosition const& position);
        static Optional<size_t> Any(SourcePosition const& position);

        class Error final: public exception
        {
        public:
            Error(size_t const count, string const& id)
                : Count(count)
                , Id(id)
            {}
            size_t const Count;
            string const Id;
        };
    };

}