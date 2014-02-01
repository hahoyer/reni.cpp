#pragma once

#include "../String.h"
#include "../ValueCache.h"
#include "../DumpToString.h"

namespace HWLib
{
    namespace Match
    {
        class SourcePosition;

        class Source
        {
            using thisType = Source;
            String const _fileName;
            ValueCache<String> const _textCache;

        public:
            Source(String const& fileName);
            Source(Source const& other);
            DefaultAssignmentOperator;
            p(String, Text);
            p(int, Count);
            bool const IsEnd(int position)const;
            String const Part(int start, int count)const;
            String const FilePosition(int position, String flagText, String tag = "")const;
            int const LineNr(int position)const;
            int const ColNr(int position)const;
            friend SourcePosition const operator +(Ref<Source const> const source, int position);
            bool const BeginsWith(int position, String value)const;
        };

    }
}

template<>
String const HWLib::DumpToString<HWLib::Match::Source>(HWLib::Match::Source const&target);