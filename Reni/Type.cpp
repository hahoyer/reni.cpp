#include "Import.h"
#include "Type.h"

#include "ArrayType.h"
#include "BitType.h"
#include "Code.h"
#include "DumpPrintToken.h"
#include "Feature.h"
#include "FeatureProvider.h"
#include "NumberType.h"
#include "Result.h"
#include "UserDefinedToken.h"
#include "TypeType.h"

#include "../HWLib/_EditorTemplates.h"
#include "../HWLib/FunctionCache.h"
#include "../HWLib/RefCountContainer.instance.h"
#include "../HWLib/ValueCache.h"


using namespace Reni;
static bool Trace = true;



struct Type::internal{
    FunctionCache<WeakRef<ArrayType>, int> array;
    ValueCache<WeakRef<NumberType>> number;
    ValueCache<WeakRef<TypeType>> type;

    explicit internal(Type const&parent)
        : array([&](int count) {return new ArrayType(parent, count); })
        , number([&] {return parent.CreateNumberType(); })
        , type([&] {return new TypeType(parent.thisRef); }) {
    };
};


Type::Type() : _internal(new internal(*this)){
}

pure_p_implementation(Type, Size, size) ;
pure_p_implementation(Type, WeakRef<Global>, global);
pure_p_implementation(Type, WeakRef<Type>, asFunctionResult);

ResultData const Type::GetResultData(Ref<CodeItem> code)const{
    return ResultData(size, code, &this->thisRef);
};

ResultData const Type::GetResultData() const{
    a_if(!HasData, Dump);
    return ResultData(size, CodeItem::Const(BitsConst::Empty()), &this->thisRef);
}

WeakRef<Type> const Type::array(int count)const{
    return _internal->array(count)->thisRef;
};

p_implementation(Type, WeakRef<NumberType>, numberType){
    return &_internal->number.Value->thisRef;
};

p_implementation(Type, WeakRef<TypeType>, typeType) {
    return &_internal->type.Value->thisRef;
};

p_implementation(Type, WeakRef<Type>, asFunctionResult) {
    md_;
    b_;
    return_d(thisRef);
};

WeakRef<NumberType> const Type::CreateNumberType() const{
    auto a = dynamic_cast<ArrayType const*>(this);
    a_if(a && dynamic_cast<BitType const*>(&a->elementType), nd(*this) + " cannot be flagged as number type");
    return new NumberType(a->thisRef);
};

Type::operator Ref<FeatureProvider<DefineableToken>, true>()const{
    md_;
    mb;
};

Type::operator Ref<FeatureProvider<DumpPrintToken>, true>()const {
    md_;
    mb;
};

Type::operator Ref<FeatureProvider<MinusToken>, true>()const {
    md_;
    mb;
};

Type::operator Ref<FeatureProvider<PlusToken>, true>() const{
    md_;
    mb;
}

SearchResult const Type::GetDefinition(DefineableToken const&token) const{
    md(token);
    mb;
}

Type::operator Ref<FeatureProvider<StarToken>, true>() const{
    md_;
    mb;
}
