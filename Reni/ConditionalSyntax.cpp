#include "Import.h"
#include "ConditionalSyntax.h"

#include "CodeItem.h"
#include "ContainerContext.h"
#include "Feature.h"
#include "Global.h"
#include "RecursionContext.h"
#include "SyntaxContainer.h"
#include "Type.h"

using namespace Reni;
static bool Trace = true;


IfThenSyntax::IfThenSyntax(Ref<Syntax> const condition, SourcePart const part, Ref<Syntax> const thenClause)
: baseType(part)
, condition(condition)
, thenClause(thenClause)
{
    SetDumpString();
};

p_implementation(IfThenSyntax, string, SmartDump)
{
    return
        condition->SmartDumpFrame(priority)
        + " then "
        + thenClause->SmartDumpFrame(priority);
};

ResultData IfThenSyntax::GetResultData(const Context& context, Category const& category)const
{
    WeakRef<Reni::Type> elseType = context.global->voidType.thisRef;

    auto conditionCategory =
        (category.hasCode || category.hasClosure)
        ? category.typed
        : Category::None;

    auto conditionResult =
        condition
        ->GetResultCache(context)
        ->Get(conditionCategory)
        .Convert(context.global->boolType);

    auto thenResult =
        thenClause
        ->GetResultCache(context)
        ->Get(category);

    return ResultData::GetSmartHollowSize
        (
        category,
        l_(CodeItem::IfThenElse(conditionResult.code, thenResult.code, CodeItem::Empty())),
        l_(elseType),
        l_(conditionResult.closure.Value + thenResult.closure.Value)
        );
}

Optional<Ref<Syntax>> IfThenSyntax::Replace(SyntaxArgVisitor const&visitor) const{
    Optional<Ref<Syntax>> newLeft = condition->Replace(visitor);
    auto newRight = thenClause->Replace(visitor);
    if(newLeft.IsEmpty && newRight.IsEmpty)
        return{};
    return Ref<Syntax>(new IfThenSyntax(newLeft || condition, part, newRight || thenClause));
}


IfThenElseSyntax::IfThenElseSyntax(Ref<Syntax> const condition, Ref<Syntax> const thenClause, SourcePart const& part, Ref<Syntax> const elseClause)
: baseType(part)
, condition(condition)
, thenClause(thenClause)
, elseClause(elseClause)
{
    SetDumpString();
};

p_implementation(IfThenElseSyntax, string, SmartDump)
{
    return
        condition->SmartDumpFrame(priority)
        + " then "
        + thenClause->SmartDumpFrame(priority)
        + " else "
        + elseClause->SmartDumpFrame(priority);
};

ResultData IfThenElseSyntax::GetResultData(const Context& context, Category const& category)const
{
  if(auto recursionContext = dynamic_cast<RecursionContext const*>(&context))
    {
        if(category == Category::Type)
        {
            auto thenType = recursionContext->CachedType(*thenClause);
            auto elseType = recursionContext->CachedType(*elseClause);
            if(thenType.IsEmpty != elseType.IsEmpty)
                return *(thenType || elseType).Value;

            HW_D_METHOD(context, category);
            HW_BREAKPOINT;
            return{};

        }
        
        if(category == Category::Closure)
            return Closure();

        HW_D_METHOD(context, category);
        HW_BREAKPOINT;
        return{};
    }

    auto conditionCategory =
        (category.hasCode || category.hasClosure)
            ? category.typed
            : Category::None;

    auto conditionResult =
        condition
        ->GetResultCache(context)
        ->Get(conditionCategory)
        .Convert(context.global->boolType);

    auto thenResult = 
        thenClause
        ->GetResultCache(context)
        ->Get(category);
    
    auto elseResult = 
        elseClause
        ->GetResultCache(context)
        ->Get(category);

    return ResultData::GetSmartHollowSize
        (
            category,
            l_(CodeItem::IfThenElse(conditionResult.code, thenResult.code, elseResult.code)),
            l_(thenResult.type.Value->Common(*elseResult.type.Value)),
            l_(conditionResult.closure.Value + thenResult.closure.Value + elseResult.closure.Value)
        );
}

Optional<Ref<Syntax>> IfThenElseSyntax::Replace(SyntaxArgVisitor const&visitor) const
{
    auto newLeft = condition->Replace(visitor);
    auto newRight = thenClause->Replace(visitor);
    auto newElse= elseClause->Replace(visitor);
    if(newLeft.IsEmpty && newRight.IsEmpty&& newElse.IsEmpty)
        return{};
    return Ref<Syntax>(new IfThenElseSyntax(newLeft || condition, newRight || thenClause, part, newElse || elseClause));
}

