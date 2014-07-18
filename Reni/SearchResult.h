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
    class SearchResult : public DumpableObject
    {
        using baseType = DumpableObject;
        using thisType = SearchResult;
    
        TFeature const feature;
        Array<WeakRef<Type>> const path;

        SearchResult(TFeature const&feature, Array<WeakRef<Type>> const&path)
            : feature(feature)
            , path(path)
        {
            SetDumpString();
        }
    public:
        SearchResult(TFeature const&feature)
            : feature(feature)
        {
            SetDumpString();
        }

        SearchResult(): feature(TFeature::None()){SetDumpString();}

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

        void AssertValid()const{a_if(IsValid, Dump);}

        p(bool, IsValid){return !feature.isEmpty;};
        thisType const operator+(Type const&fromType)const;

        ResultData const FunctionResult(
            Context const&context,
            Category category,
            Optional<Ref<Syntax>> const& right
            )const;
        ResultData const FunctionResult(
            Context const&context,
            Category category,
            Optional<Ref<Syntax>> const& left,
            Optional<Ref<Syntax>> const& right
            )const;
        ResultData const ConversionResult(Category category, Type const&target, Type const&destination) const;
    private:
        p_function(Array<String>, DumpData) override{return{nd(feature)+nd(path)};};

    };

}


