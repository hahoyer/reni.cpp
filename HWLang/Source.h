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
        String const fileName;
        ValueCache<String const> const textCache;

        Source(String const& fileName, String const& text);
    public:
        Source(Source const& other);
        static Source const FromFile(String const& fileName);
        static Source const FromText(String const& text);
        static Ref<Source>const CreateFromFile(String const& fileName);
        static Ref<Source>const CreateFromText(String const& text);

        virtual ~Source(){};

        DefaultAssignmentOperator;
        operator Source& ()const{ return const_cast<Source&>(*this); }

        p(String, Text);
        p(size_t, Count);

        bool const IsEnd(size_t position)const;
        String const Part(size_t start, size_t count)const;
        String const FilePosition(size_t position, String flagText, String tag = "")const;
        size_t const LineNr(size_t position)const;
        size_t const ColNr(size_t position)const;
        friend SourcePosition const operator +(Ref<Source> const& source, size_t position);
        SourcePosition const operator +(size_t position)const;
        bool const BeginsWith(size_t position, String value)const;
        String const DumpAfterCurrent(size_t position, size_t count, size_t dumpWidth)const;
        String const DumpCurrent(size_t position, size_t count)const;
        String const DumpBeforeCurrent(size_t position, size_t dumpWidth)const;
        String const DumpAroundCurrent(size_t position, size_t count, size_t dumpWidth)const;
    private:
        p_function(Array<String>,DumpData) override;
    };
}
