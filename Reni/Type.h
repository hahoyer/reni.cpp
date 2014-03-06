#pragma once
#include "../HWLib/OptWeakRef.h"
#include "../HWLib/Ref.h"

using namespace HWLib;

namespace Reni{
    class ArrayType;
    class BitsConst;
    class DefinitionPoint;
    class DumpPrintToken;
    class ResultData;
    class Category;
    class Size;

    template<typename T, typename ...> class FeatureProvider;

    class Type: public WithId<DumpableObject, Type>{
        using baseType = WithId<DumpableObject, Type>;
        using thisType = Type;
        struct internal;
        CtrlRef<internal> _internal;
    protected:
        Type();
    public:
        virtual_p(Size, size) = 0;
        Type const& array(int count)const;
        ResultData const GetResultData(Category category, BitsConst const&value)const;
        ref_p;
        virtual operator CtrlPtr<FeatureProvider<DumpPrintToken>>()const;
        virtual operator CtrlPtr<FeatureProvider<DumpPrintToken, ArrayType>>()const{ return{}; };
    private:
        override_p_function(Array<String>, DumpData){ return{}; };
    };
}
