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

        SourcePosition operator+(size_t other) const
        {
            return SourcePosition(_source, _position + other);
        }

        size_t operator-(SourcePosition const& other) const
        {
            a_is(&*_source, ==, &*other._source);
            return _position - other._position;
        }

        bool operator==(SourcePosition const& other)const;

        SourcePart const Span(size_t count)const;
        string Part(size_t count) const;

        bool BeginsWith(string const&value) const;
    private:
        p_function(HWLib::Array<std::string>,DumpData) override;
        p(std::string, DumpCurrent);
        p(std::string, DumpAfterCurrent);
        p(std::string, DumpBeforeCurrent);
    };
}
