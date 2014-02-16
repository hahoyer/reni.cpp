#pragma once

namespace HWLang
{
    class SourcePosition;

    class Source final : public DumpableObject
    {
        using thisType = Source;
        String const _text;
        String const _fileName;
        ValueCache<String> const _textCache;

        Source(String const& fileName, String const& text);
    public:
        Source(Source const& other);
        static Source const FromFile(String const& fileName);
        static Source const FromText(String const& text);

        virtual ~Source(){};

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

    private:
        virtual p_function(Array<String>, DumpData)override;
    };
}
