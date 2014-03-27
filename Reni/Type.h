#pragma once
#include "SearchResult.h"

#include "../HWLib/Ref.h"
#include "../HWLib/RefCountProvider.h"
#include "../HWLib/CtrlRef.h"
#include "../HWLib/DumpableObject.h"
#include "../HWLib/WeakRef.h"
#include "../Util/Category.h"

using namespace HWLib;
using namespace Util;

namespace Reni{
    class ArrayType;
    class CodeItem;
    class DefinitionPoint;
    class DumpPrintToken;
    class Global;
    class MinusToken;
    class NumberType;
    class PlusToken;
    class ResultData;
    class StarToken;
    class UserDefinedToken;

    template<typename T, typename ...> class FeatureProvider;

    class Type
        : public WithId<DumpableObject, Type>
    {
        using baseType = WithId<DumpableObject, Type>;
        using thisType = Type;
        struct internal;
        CtrlRef<internal> _internal;
    protected:
        Type();
    public:
        AssumeConstObject;
        bool operator==(Type const&other)const{ return this == &other; }

        virtual_p(Size, size) = 0;
        virtual_p(WeakRef<Global>, global) = 0;
        
        WeakRef<Type> const array(int count)const;
        p(WeakRef<NumberType>, numberType);
        
        ResultData const GetResultData(Category category, Ref<CodeItem> code)const;

        template<class T>
        SearchResult const GetGenericDefinition()const{
            Ref<FeatureProvider<T>> f = *this;
            return f->feature;
        }

        virtual operator Ref<FeatureProvider<UserDefinedToken>, true>()const;
        virtual operator Ref<FeatureProvider<DumpPrintToken>, true>()const;
        virtual operator Ref<FeatureProvider<MinusToken>,true>()const;
        virtual operator Ref<FeatureProvider<PlusToken>, true>()const;
        virtual operator Ref<FeatureProvider<StarToken>, true>()const;

        WeakRef<NumberType> const CreateNumberType()const;

    private:
        p_function(Array<String>,DumpData) override{ return{}; };
    };
}
