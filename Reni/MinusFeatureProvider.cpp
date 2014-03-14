#include "Import.h"
#include "MinusFeatureProvider.h"
#include "Compiler.h"

using namespace Reni;
static bool Trace = true;


ResultData const MinusFeatureProvider::Feature::FunctionResult(
    Context const&context,
    Category category,
    Ref<Syntax, true> const& right
    )const {
    if(right.IsEmpty)
        return baseType::FunctionResult(context, category, right);
    auto macro = Compiler::GetSyntaxFromText("(0-1)*arg");
    auto syntax = macro->ReplaceArg(right);
    return syntax->GetResultData(context, category);
}

