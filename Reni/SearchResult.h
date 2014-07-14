#pragma once
#include "../HWLib/Ref.h"
#include "../HWLib/DumpableObject.h"

using namespace HWLib;

namespace Reni
{
    template<class TFeature>
    class SearchResult : public DumpableObject
    {
        using baseType = DumpableObject;
        using thisType = SearchResult;
    public:
        TFeature const feature;

        SearchResult(TFeature const&feature) 
            : feature(feature)
        {
            SetDumpString();
        }

        SearchResult(){SetDumpString();}

        thisType const& CheckAssertValid()const
        {
            AssertValid();
            return *this;            
        }
        thisType & CheckAssertValid()
        {
            AssertValid();
            return *this;
        }

        void AssertValid()const
        {
            a_if(IsValid, Dump);
        }

        p(bool, IsValid){return !feature.isEmpty;};
    private:
        p_function(Array<String>, DumpData) override{return{nd(feature)};};
    };

}