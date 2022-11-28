#pragma once
#include "../HWLib/DumpableObject.h"
#include "../HWLib/ValueCache.h"
#include "../HWLib/RefCountProvider.h"
#include "../HWLib/Ref.h"

using namespace HWLib;

namespace HWLang
{
    class SourcePosition;

    class Source final 
        : public DumpableObject
        , public RefCountProvider
    {
        using thisType = Source;
        string const fileName;
        ValueCache<string> const textCache;

        Source(string const& fileName, string const& text);
    public:
        Source(Source const& other);
        static Source FromFile(string const& fileName);
        static Source FromText(string const& text);
        static Ref<Source> CreateFromFile(string const& fileName);
        static Ref<Source> CreateFromText(string const& text);

        virtual ~Source(){};

        HW_DO_PLACEMENT_ASSIGN;
        operator Source& ()const{ return const_cast<Source&>(*this); }

        HW_PR_GET(string, Text);
        HW_PR_GET(size_t, Count);

        bool IsEnd(size_t position) const;
        string Part(size_t start, size_t count) const;
        string FilePosition(size_t position, string flagText, string tag = "") const;
        size_t LineNr(size_t position) const;
        size_t ColNr(size_t position) const;
        friend SourcePosition operator +(Ref<Source> const& source, size_t position);
        SourcePosition operator +(size_t position) const;
        bool BeginsWith(size_t position, string value) const;
        string DumpAfterCurrent(size_t position, size_t count, size_t dumpWidth) const;
        string DumpCurrent(size_t position, size_t count) const;
        string DumpBeforeCurrent(size_t position, size_t dumpWidth) const;
        string DumpAroundCurrent(size_t position, size_t count, size_t dumpWidth) const;
    private:
        HW_PR_DECL_GETTER(Array<string>,DumpData) override;
    };
}
