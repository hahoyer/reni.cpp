#pragma once

namespace Reni
{
    class Source;
    class SourcePart;

    class SourcePosition
    {
        Ref<Source const> const _source;
        int _position;
    public:
        SourcePosition(Ref<Source const> source, int position = 0)
            : _source(source)
            , _position(position)
        {}

        p(Optional<int>, End);
        p(bool, IsEnd);
        void operator+=(int index){ _position += index; }
        bool const operator==(SourcePosition constother)const;

        SourcePart const Span(int count)const;
        String const Part(int count)const;
    };

}