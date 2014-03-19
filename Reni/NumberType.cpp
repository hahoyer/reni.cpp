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

ResultData const NumberType::DumpPrintProvider::Result(Category category, NumberType const&type){
    return type.global->voidType
        .GetResultData(category, CodeItem::DumpPrint(type));
};

ResultData const NumberType::MinusProvider::Result(Category category, NumberType const&_thisType, Type const&argType){
    fd(category, _thisType, argType);
    b_;
    return{};
};
