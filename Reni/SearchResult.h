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

        Optional<Ref<FoundFeature<TFeature>>> data;
    public:
        SearchResult() { SetDumpString(); }
        
        SearchResult(FoundFeature<TFeature> const&data)
            : data(new FoundFeature<TFeature>(data))
        {
            SetDumpString();
        }

        p(FoundFeature<TFeature>, found)
        {
            AssertValid();
            return *data.Value;
        }

        p(bool, IsValid){ return data.IsValid; };

    private:
        p_function(Array<String>, DumpData) override{ return{nd(data)}; };
        void AssertValid()const{ a_if(IsValid, Dump); }
    };

    template<class TFeature>
    class ObsoleteSearchResult : public DumpableObject
    {
        using baseType = DumpableObject;
        using thisType = ObsoleteSearchResult ;

        TFeature const feature;
        Array<WeakRef<Type>> const path;

        ObsoleteSearchResult(TFeature const&feature, Array<WeakRef<Type>> const&path)
            : feature(feature)
            , path(path)
        {
            SetDumpString();
        }
    public:
        ObsoleteSearchResult(TFeature const&feature)
            : feature(feature)
        {
            SetDumpString();
        }

        ObsoleteSearchResult () : feature(TFeature::None()){ SetDumpString(); }

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

        void AssertValid()const{ a_if(IsValid, Dump); }

        p(bool, IsValid){ return !feature.isEmpty; };
    private:
        p_function(Array<String>, DumpData) override{ return{nd(feature) + nd(path)}; };

    };

}


