#include "Import.h"
#include "ConditionalSyntax.h"

#include "Feature.h"

using namespace Reni;
static bool Trace = true;


ConditionalSyntax::ConditionalSyntax(Ref<Syntax> const left, SourcePart const part, Ref<Syntax> const right)
: baseType(part)
, left(left)
, right(right)
{
    SetDumpString();
};

p_implementation(ConditionalSyntax, Array<String>, DumpData)
{
    return{
        nd(left),
        nd(right)
    };
};

ResultData const ConditionalSyntax::GetResultData(Context const&context, Category category)const
{
    md(context, category);
    b_;
    return{};
}

Ref<Syntax, true> ConditionalSyntax::Replace(SyntaxArgVisitor const&visitor) const{
    auto newLeft = left->Replace(visitor);
    auto newRight = right->Replace(visitor);
    if(newLeft.IsEmpty && newRight.IsEmpty)
        return{};
    return new ConditionalSyntax(newLeft || left, part, newRight || right);
}