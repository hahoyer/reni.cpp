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

        HW_PR_GET(Optional<size_t>, End);
        HW_PR_GET(bool, IsEnd);
        HW_PR_GET(char, First);

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
            HW_ASSERT_IS(&*_source, ==, &*other._source);
            return _position - other._position;
        }

        bool operator==(SourcePosition const& other)const;

        SourcePart const Span(size_t count)const;
        string Part(size_t count) const;

        bool BeginsWith(string const&value) const;
    private:
        HW_PR_DECL_GETTER(HWLib::Array<std::string>,DumpData) override;
        HW_PR_GET(std::string, DumpCurrent);
        HW_PR_GET(std::string, DumpAfterCurrent);
        HW_PR_GET(std::string, DumpBeforeCurrent);
    };
}
