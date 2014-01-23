#pragma once

namespace Reni
{
    class Source
    {
        using thisType = Source;
        String const _fileName;
        ValueCache<String> const _textCache;

    public:
        Source(String const& fileName)
            : _fileName(fileName)
            , _textCache([&]{return HWLib::File(_fileName).Data; })
        {}
        Source(Source const& other) 
            :Source(other._fileName)
        {}

        DefaultAssignmentOperator;

        p(String, Text){ return *_textCache; }
    };
}
