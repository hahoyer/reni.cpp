#include "Import.h"
#include "SyntaxContainer.h"

#include "CodeItems.h"
#include "ContainerContext.h"
#include "DefineableToken.h"
#include "EmptySyntax.h"
#include "ExpressionSyntax.h"
#include "FunctionSyntax.h"

#include "../HWLib/RefCountContainer.instance.h"
#include "../HWLang/Admin/Export.h"

using namespace HWLib;
using namespace Reni;
static bool Trace = true;


void EmptySyntax::AddTo(SyntaxContainer& main) const
{
    main.statements += thisRef;
}

SyntaxContainer::SyntaxContainer(SourcePart const&part) 
    : baseType(part)
    , emptySyntax(new EmptySyntax(part))
{
    SetDumpString();
}

p_implementation(SyntaxContainer, string, SmartDump)
{
    return( _({HW_D_VALUE(statements)}) +
        names
        .keys
        .Select<string>([&](DefineableToken const*key)
            {
                return key->name + ": " + HWLib::Dump(names[key]);
            })
        ->ToArray).Stringify(";");
}

void SyntaxContainer::AddTo(SyntaxContainer&main) const
{
    for(auto key: names.keys)
        main.names.Assign(key, names[key] + main.statements.Count);
    main.statements += statements;
}

void SyntaxContainer::Add(Ref<Syntax> const& definitionTarget, Ref<Syntax> const& value)
{
    auto& e = dynamic_cast<ExpressionSyntax const&>(*definitionTarget);
    HW_ASSERT_(e.left.IsEmpty);
    HW_ASSERT_(e.right.IsEmpty);
    auto& d = dynamic_cast<DefineableToken const&>(e.tokenClass);
    names.Assign(&d, statements.Count);
    statements += value;
}

ResultData SyntaxContainer::GetResultData(const Context& context, Category const& category) const
{
    return ResultData::GetSmartHollowSize
        (
        category,
        l_(GetCode(context)),
        l_(GetType(context)),
        l_(GetClosure(context))
        );
}
     
WeakRef<Type> SyntaxContainer::GetType(Context const& context) const
{
    return context.Container(*this, statements.Count)->dataType;
}

void SyntaxContainer::Add(Optional<Ref<Syntax>> const& value)
{
    Ref<Syntax> const effectiveValue = value || emptySyntax; 
    effectiveValue->AddTo(*this);
}


Ref<CodeItem> SyntaxContainer::GetCode(Context const& context) const
{
    auto result = Numbers(statements.Count)
        ->Select<Ref<CodeItem>>([&](size_t index){return GetCode(context, index);})
        ->Where([&](Ref<CodeItem> item){return !item->isEmpty; })
        ->ToArray;

    if(result.Count == 1)
        return result[0];

    HW_D_METHOD(context, result);
    HW_BREAK_AND_THROW;
};

Ref<CodeItem> SyntaxContainer::GetCode(Context const& context, size_t index) const
{
    auto container = context.Container(*this, index);
    return statements[index]->GetCode(*container);
}

Closure SyntaxContainer::GetClosure(Context const& context) const
{
    return Numbers(statements.Count)
        ->Select<Closure>([&](size_t index){return GetClosure(context, index); })
        ->Aggregate<Closure>();
}

Closure SyntaxContainer::GetClosure(Context const& context, size_t index) const
{
    auto container = context.Container(*this, index);
    return statements[index]->GetClosure(*container);
};
