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

        static int const WhiteSpace(SourcePosition const&position);
        static Optional<int> const Number(SourcePosition const&position);
        static Optional<int> const Text(SourcePosition const&position);
        static Optional<int> const Any(SourcePosition const&position);

        class Error final
        {
        public:
            Error(int const count, String const& id)
                : Count(count)
                , Id(id)
            {}
            int const Count;
            String const Id;
        };
    };

}