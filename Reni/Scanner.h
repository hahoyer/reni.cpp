#pragma once

namespace Reni
{
    class Scanner final
    {
        class internal;
        Ref<internal const> const _internal;
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