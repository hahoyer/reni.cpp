#pragma once
#include "../HWLib/Ref.h"
#include "../HWLib/RefCountProvider.h"
#include "../HWLib/CtrlRef.h"
#include "../HWLib/DumpableObject.h"

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

    class Type
        : public WithId<DumpableObject, Type>
        , public RefCountProvider
    {
        using baseType = WithId<DumpableObject, Type>;
        using thisType = Type;
        struct internal;
        CtrlRef<internal> _internal;
    protected:
        Type();
    public:
        virtual_p(Size, size) = 0;
        Ref<Type> const array(int count)const;
        ResultData const GetResultData(Category category, BitsConst const&value)const;
        ref_p;
        virtual operator Ptr<FeatureProvider<DumpPrintToken>>()const;
        virtual operator Ptr<FeatureProvider<DumpPrintToken, ArrayType>>()const{ return{}; };
    private:
        override_p_function(Array<String>, DumpData){ return{}; };
    };
}
