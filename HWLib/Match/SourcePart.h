#pragma once

#include "SourcePosition.h"

namespace HWLib
{
    namespace Match
    {
        class Source;

        class SourcePart: DumpableObject
        {
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

            operator String const()const;
        private:
            virtual p_function(String, Dump)override;
            p(String, DumpCurrent);
            p(String, DumpAfterCurrent);
            p(String, DumpBeforeCurrent);
        };
    }
}