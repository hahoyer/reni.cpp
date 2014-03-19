#include "Import.h"
#include "Type.h"

#include "ArrayType.h"
#include "BitsConst.h"
#include "BitType.h"
#include "Code.h"
#include "DumpPrintToken.h"
#include "Feature.h"
#include "FeatureProvider.h"
#include "NumberType.h"
#include "Size.h"
#include "Result.h"

#include "../HWLib/_EditorTemplates.h"
#include "../HWLib/FunctionCache.h"
#include "../HWLib/RefCountContainer.instance.h"
#include "../HWLib/ValueCache.h"


using namespace Reni;
static bool Trace = true;

struct Type::internal{
    FunctionCache<int, WeakRef<ArrayType>> array;
    ValueCache<WeakRef<NumberType>> number;

    explicit internal(Type const&parent)
        : array([&](int count){return new ArrayType(parent, count);})
          , number([&]{return parent.CreateNumberType();}){
    };
};


Type::Type() : _internal(new internal(*this)){
}

pure_p_implementation(Type, Size, size) ;
pure_p_implementation(Type, WeakRef<Global>, global);

ResultData const Type::GetResultData(Category category, Ref<CodeItem> code)const{
    return ResultData(size, code, &this->ref);
};

WeakRef<Type> const Type::array(int count)const{
    return _internal->array[count]->ref;
};

p_implementation(Type, WeakRef<NumberType>, numberType){
    return &_internal->number.Value->ref;
};

Type::operator Ref<FeatureProvider<DumpPrintToken>, true>()const{
    md_;
    mb;
};


WeakRef<NumberType> const Type::CreateNumberType() const{
    auto a = dynamic_cast<ArrayType const*>(this);
    a_if(a && dynamic_cast<BitType const*>(&a->elementType), nd(*this) + " cannot be flagged as number type");
    return new NumberType(a->ref);
};

Type::operator Ref<FeatureProvider<MinusToken>, true>()const{
    md_;
    mb;
};


Type::operator Ref<FeatureProvider<PlusToken>, true>() const{
    md_;
    mb;
}

Type::operator Ref<FeatureProvider<StarToken>, true>() const{
    md_;
    mb;
}

