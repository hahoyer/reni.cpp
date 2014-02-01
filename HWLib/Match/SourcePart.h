#pragma once

#include "SourcePosition.h"

namespace HWLib
{
    namespace Match
    {
        class Source;


        class SourcePart
        {
            Ref<Source const> const _source;
            int const _position;
            int const _count;
        public:
            SourcePart(Ref<Source const> source, int position, int count)
                : _source(source)
                , _position(position)
                , _count(count)
            {};

            operator String const()const;
        };

    }
}