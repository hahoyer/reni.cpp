#pragma once

#include "../Ref.h"
#include "../Object.h"

namespace HWLib
{
    namespace Match
    {
        class Source;
        class SourcePart;

        class SourcePosition: Object
        {
            Ref<Source const> const _source;
            int _position;
        public:
            SourcePosition(Ref<Source const> source, int position = 0)
                : _source(source)
                , _position(position)
            {
                SetDumpString();
            }

            p(Optional<int>, End);
            p(bool, IsEnd);
            p(char, First);

            void operator+=(int index)
            {
                _position += index;
                SetDumpString();
            }

            SourcePosition const operator+(int other)const
            {
                return SourcePosition(_source, _position +other);
            }

            int const operator-(SourcePosition const&other)const
            {
                assert(&*_source == &*other._source);
                return _position - other._position;
            }

            bool const operator==(SourcePosition constother)const;

            SourcePart const Span(int count)const;
            String const Part(int count)const;

            bool const BeginsWith(String value)const;
        private:
            virtual p_function(std::string, Dump)override;
            p(String, DumpCurrent);
            p(String, DumpAfterCurrent);
            p(String, DumpBeforeCurrent);

        };
    }
}
