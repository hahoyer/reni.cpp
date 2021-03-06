#pragma once


namespace HWLang
{
    class Source;
    class SourcePart;

    class SourcePosition final : public DumpableObject
    {
        Ref<Source> const _source;
        int _position;
    public:
        SourcePosition(Ref<Source> source, int position = 0);
        SourcePosition(Source const& source, int position = 0);
        virtual ~SourcePosition(){};

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
            return SourcePosition(_source, _position + other);
        }

        int const operator-(SourcePosition const&other)const
        {
            a_is(&*_source, ==, &*other._source);
            return _position - other._position;
        }

        bool const operator==(SourcePosition constother)const;

        SourcePart const Span(int count)const;
        String const Part(int count)const;

        bool const BeginsWith(String value)const;
    private:
        p_function(HWLib::Array<HWLib::String>,DumpData) override;
        p(String, DumpCurrent);
        p(String, DumpAfterCurrent);
        p(String, DumpBeforeCurrent);
    };
}
