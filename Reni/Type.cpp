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
#include "TemplateInstances.h"
#include "TypeType.h"
#include "UserDefinedToken.h"

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
    return ResultData(size, code, thisRef);
};

ResultData const Type::GetResultData(Category category, function<Ref<CodeItem>()> getCode) const{
    return ResultData(
        !category.hasSize ? Optional<Size>() : size ,
        !category.hasCode ? Ref<CodeItem, true>() : getCode(),
        !category.hasType ? WeakPtr<Type>() : thisRef
        );
}

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

Type::operator Ref<FeatureProvider<InstanceToken>, true>() const{
    md_;
    mb;
}

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

ResultData const Type::ContextAccessResult(Category category, ContextReference const& target, function<Size()> getOffset)const{
    if(!HasData)
        return GetResultData() & category;

    return GetResultData(category, [&]{
        return CodeItem::ReferenceCode(target)
            ->ReferencePlus(getOffset()); 
    });
}

SearchResult const Type::GetDefinition(DefineableToken const&token) const{
    bool Trace = false;
    md(token);
    return_d(SearchResult());
}

Type::operator Ref<FeatureProvider<StarToken>, true>() const{
    md_;
    mb;
}


struct InstanceProvider{
    typedef class TypeType targetType;
    static ResultData const Result(Category category, TypeType const&target, Type const&arg){
        fd(category, target, arg);
        b_;
        return{};
    }
};


inline TypeType::operator Ref<FeatureProvider<InstanceToken>, true>() const{
    return new InfixFunctionProvider<InstanceToken, InstanceProvider>(*this);
};

/*
InfixFunctionProvider<InstanceToken,InstanceProvider>
::InfixFunctionProvider<InstanceToken,InstanceProvider>
(TypeType const &) 

TypeType::operator Ref<FeatureProvider<InstanceToken>,true>()const 

*/