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

        p(bool, IsValid){return !feature.isEmpty;};
    private:
        p_function(Array<String>, DumpData) override{return{nd(feature)};};
    };

}