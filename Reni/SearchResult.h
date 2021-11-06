#pragma once

#include "../HWLib/Ref.h"
#include "../HWLib/DumpableObject.h"
#include "../HWLib/Array.h"

using namespace HWLib;
using namespace Util;

namespace Reni
{
    class Context;
    class ResultData;
    class Syntax;
    class Type;

    template<class TFeature>
    class FoundFeatureBase 
        : public DumpableObject
        , public RefCountProvider
    {
        using baseType = DumpableObject;
        using thisType = FoundFeatureBase;
    protected:
        FoundFeatureBase() {};
        FoundFeatureBase(FoundFeatureBase const&) {};
    };

    template<class TFeature>
    class FoundFeature
        : public FoundFeatureBase<TFeature>
    {
        using baseType = FoundFeatureBase<TFeature>;
        using thisType = FoundFeature;
    };


    template<class TFeature>
    class SearchResult : public DumpableObject
    {
        using baseType = DumpableObject;
        using thisType = SearchResult;

        Optional<FoundFeature<TFeature>> data;
    public:
        SearchResult() { SetDumpString(); }
        
        SearchResult(FoundFeature<TFeature> const&data)
            : data(data)
        {
            SetDumpString();
        }

        p(FoundFeature<TFeature>, found)
        {
            AssertValid();
            return data.Value;
        }

        p(bool, IsValid){ return data.IsValid; };

    private:
        p_function(Array<string>, DumpData) override{ return{HW_D_VALUE(data)}; };
        void AssertValid()const{ HW_ASSERT(IsValid, Dump); }
    };
}


