#include "Import.h"
#include "NumberType.h"

#include "ArgVisitor.h"
#include "ArrayType.h"
#include "CodeItems.h"
#include "ExpressionSyntax.h"
#include "Feature.h"
#include "FeatureProvider.h"
#include "RootContext.h"
#include "Result.h"
#include "VoidType.h"
#include "TemplateInstances.h"
#include "../HWLib/RefCountContainer.instance.h"


using namespace Reni;
static bool Trace = true;

NumberType::NumberType(WeakRef<ArrayType> const parent) 
: parent(parent)
, dumpPrintFeature(new dumpPrintProviderType(*this)){
    SetDumpString();
}


NumberType::operator Ref<FeatureProvider<DumpPrintToken>, true>() const {
    return dumpPrintFeature->ref;
};

p_implementation(NumberType, Size, size){ return parent->size; };

p_implementation(NumberType, Array<String>, DumpData){
    return{nd(parent)};
};

ResultData const NumberType::DumpPrintProvider::Result(
    NumberType const&type,
    Context const&context,
    Category category,
    Ref<Syntax> target
    ){

    Ref<CodeItem, true> code;
    if(category.hasCode){
        auto result = target->GetResultCache(context);
        ArgVisitor visitor = *result;
        code = CodeItem::DumpPrint(type)
            ->Replace(visitor);
    }

    return ResultData(
        Size(0),
        code,
        context.rootContext->voidType->ref
        );
};

