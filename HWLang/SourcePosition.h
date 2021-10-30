#pragma once


namespace HWLang
{
    class Source;
    class SourcePart;

    class SourcePosition final : public DumpableObject
    {
        Ref<Source> const _source;
        size_t _position;
    public:
        SourcePosition(Ref<Source> source, size_t position = 0);
        SourcePosition(Source const& source, size_t position = 0);
        virtual ~SourcePosition(){};

        p(Optional<size_t>, End);
        p(bool, IsEnd);
        p(char, First);

        void operator+=(size_t index)
        {
            _position += index;
            SetDumpString();
        }

        SourcePosition const operator+(size_t other)const
        {
            return SourcePosition(_source, _position + other);
        }

        size_t const operator-(SourcePosition const&other)const
        {
            a_is(&*_source, ==, &*other._source);
            return _position - other._position;
        }

        bool const operator==(SourcePosition constother)const;

        SourcePart const Span(size_t count)const;
        String const Part(size_t count)const;

        bool const BeginsWith(String value)const;
    private:
        p_function(HWLib::Array<HWLib::String>,DumpData) override;
        p(String, DumpCurrent);
        p(String, DumpAfterCurrent);
        p(String, DumpBeforeCurrent);
    };
}
