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
        p(int, Count);

        bool const IsEnd(int position)const;
        String const Part(int start, int count)const;
        String const FilePosition(int position, String flagText, String tag = "")const;
        int const LineNr(int position)const;
        int const ColNr(int position)const;
        friend SourcePosition const operator +(CtrlRef<Source> const& source, int position);
        SourcePosition const operator +(int position)const;
        bool const BeginsWith(int position, String value)const;

    private:
        override_p_function(Array<String>, DumpData);
    };
}
