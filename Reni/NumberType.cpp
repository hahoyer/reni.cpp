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
#include "../HWLib/RefCountContainer.instance.h"


using namespace Reni;
static bool Trace = true;


class NumberType::DumpPrintProvider final
    : public FeatureProvider<DumpPrintToken> 
{
    typedef FeatureProvider<DumpPrintToken> baseType;
    typedef DumpPrintProvider thisType;

    class Feature final : public Reni::Feature{
        typedef Reni::Feature baseType; 
        typedef Feature thisType;
        NumberType const& value;
    public:
        Feature(NumberType const&value) : value(value) {}
        AssumeConstObject;
    private:
        ResultData const FunctionResult(
            Context const&context,
            Category category,
            ExpressionSyntax const& expressionSyntax
            )const override;

        p_function(Array<String>, DumpData) override { return{nd(value)}; }
    };

    Ref<Feature> feature;
public:    
    DumpPrintProvider(NumberType const&value) : feature(new Feature(value)) {}
    AssumeConstObject;
private:
    p_function(Array<String>, DumpData) override { return{nd(feature)}; }
    p_function(Ref<Reni::Feature>, feature) override{ return feature->ref; }
};


class InvalidArgumentList final : public DumpableObject{
    WeakRef<ExpressionSyntax const> const expressionSyntax;
public:
    InvalidArgumentList(ExpressionSyntax const& expressionSyntax) : expressionSyntax(&expressionSyntax){}
private:
    p_function(Array<String>, DumpData) override{ return{nd(expressionSyntax)}; };
};


ResultData const NumberType::DumpPrintProvider::Feature::FunctionResult(
    Context const&context,
    Category category,
    ExpressionSyntax const& expressionSyntax
    )const{
    if(!expressionSyntax.right.IsEmpty)
        throw InvalidArgumentList(expressionSyntax);

    return value.DumpPrintResult(context, category, expressionSyntax.left);
};


NumberType::NumberType(WeakRef<ArrayType> const parent) 
: parent(parent)
, dumpPrintFeature(new DumpPrintProvider(*this)){
    SetDumpString();
}


NumberType::operator Ref<FeatureProvider<DumpPrintToken>, true>() const {
    return dumpPrintFeature->ref;
};

p_implementation(NumberType, Size, size){ return parent->size; };

p_implementation(NumberType, Array<String>, DumpData){
    return{nd(parent)};
};

ResultData const NumberType::DumpPrintResult(
    Context const&context,
    Category category,
    Ref<Syntax> target
    )const{

    Ref<CodeItem, true> code;
    if(category.hasCode){
        auto result = target->GetResultCache(context);
        ArgVisitor visitor = *result;
        code = CodeItem::DumpPrint(*this)
            ->Replace(visitor);
    }

    return ResultData(
        Size(0),
        code,
        context.rootContext->voidType->ref
        );
};

