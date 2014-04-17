#include "Import.h"
#include "NumberType.h"

#include "ArgVisitor.h"
#include "ArrayType.h"
#include "CodeItems.h"
#include "ExpressionSyntax.h"
#include "Feature.h"
#include "FeatureProvider.h"
#include "Context.h"
#include "Global.h"
#include "Result.h"
#include "VoidType.h"
#include "TemplateInstances.h"
#include "../HWLib/RefCountContainer.instance.h"


using namespace Reni;
static bool Trace = true;

NumberType::NumberType(WeakRef<ArrayType> const parent) 
: parent(*parent){
    SetDumpString();
}

p_implementation(NumberType, Size, size){ return parent.size; };
p_implementation(NumberType, WeakRef<Global>, global) { return parent.global; };

p_implementation(NumberType, Array<String>, DumpData){
    return{nd(parent)};
};

p_implementation(NumberType, String, DumpShort){
    return base_p_name(DumpShort) + " size=" + size.DumpShort;
};

ResultData const NumberType::DumpPrintProvider::Result(Category category, Type const&type){
    auto numberType = dynamic_cast<thisType const*>(&type);
    if(numberType)
    return type
        .global
        ->voidType
        .GetResultData(CodeItem::DumpPrint(*numberType));

    fd(category, type);
    b_;
    return{};
};

ResultData const NumberType::MinusProvider::Result(Category category, Type const&_thisType, Type const&argType){
    auto thisTypeAsNumber = dynamic_cast<thisType const*>(&_thisType);
    auto argTypeAsNumber = dynamic_cast<thisType const*>(&argType);
    if(thisTypeAsNumber && argTypeAsNumber){
        auto thisSize = thisTypeAsNumber->size.value;
        auto argSize = argTypeAsNumber->size.value;
        thisType const& resultType = thisTypeAsNumber->Resize(BitsConst::MinusSize(thisSize, argSize));
        return resultType.GetResultData(CodeItem::BinaryOperation("-", resultType, *thisTypeAsNumber, *argTypeAsNumber));
    }

    fd(category, _thisType, argType);
    b_;
    return{};
};

ResultData const NumberType::PlusProvider::Result(Category category, Type const&_thisType, Type const&argType){
    auto thisTypeAsNumber = dynamic_cast<thisType const*>(&_thisType);
    auto argTypeAsNumber = dynamic_cast<thisType const*>(&argType);
    if(thisTypeAsNumber && argTypeAsNumber){
        Type const& resultType = thisTypeAsNumber->Resize(BitsConst::PlusSize(_thisType.size.value, argTypeAsNumber->size.value));
        if(category == Category::Type)
            return resultType;
        fd(category, _thisType, argType, resultType);
        b_;
        return{};
    }

    fd(category, _thisType, argType);
    b_;
    return{};
};

ResultData const NumberType::TimesProvider::Result(Category category, Type const&_thisType, Type const&argType){
    auto thisTypeAsNumber = dynamic_cast<thisType const*>(&_thisType);
    auto argTypeAsNumber = dynamic_cast<thisType const*>(&argType);
    if(thisTypeAsNumber && argTypeAsNumber){
        auto thisSize = thisTypeAsNumber->size.value;
        auto argSize = argTypeAsNumber->size.value;
        thisType const& resultType = thisTypeAsNumber->Resize(BitsConst::TimesSize(thisSize, argSize));
        return resultType.GetResultData(CodeItem::BinaryOperation("*", resultType, *thisTypeAsNumber, *argTypeAsNumber));
    }

    fd(category, _thisType, argType);
    b_;
    return{};
};

NumberType const& NumberType::Resize(int newSize)const{
    return parent
        .elementType
        .array(newSize)
        ->numberType
        ->thisRef;
}
