#include "Import.h"
#include "SyntaxContainer.h"

#include "ContainerContext.h"
#include "ContextIncludes.h"
#include "EmptySyntax.h"
#include "ExpressionSyntax.h"
#include "CodeItems.h"
#include "DefineableToken.h"
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

p_implementation(SyntaxContainer, String, SmartDump)
{
    return( _({nd(statements)}) +
        names
        .keys
        .Select<String>([&](DefineableToken const*key)
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
    a_if_(e.left.IsEmpty);
    a_if_(e.right.IsEmpty);
    auto& d = dynamic_cast<DefineableToken const&>(e.tokenClass);
    names.Assign(&d, statements.Count);
    statements += value;
}

ResultData const SyntaxContainer::GetResultData(Context const& context, Category category) const
{
    return ResultData::GetSmartSizeExts
        (
        category,
        l_(GetCode(context)),
        l_(GetType(context))
        );
}
     
WeakRef<Type> const SyntaxContainer::GetType(Context const& context) const
{
    return context.Container(*this, statements.Count)->dataType;
}

void SyntaxContainer::Add(Optional<Ref<Syntax>> const& value)
{
    Ref<Syntax> const effectiveValue = value || emptySyntax; 
    effectiveValue->AddTo(*this);
}


Ref<CodeItem> const SyntaxContainer::GetCode(Context const& context) const
{
    auto result = Numbers(statements.Count)
        ->Select<Ref<CodeItem>>([&](int index){return GetCode(context, index);})
        ->Where([&](Ref<CodeItem> item){return !item->isEmpty; })
        ->ToArray;

    if(result.Count == 1)
        return result[0];

    md(context, result);
    mb;
};

Ref<CodeItem> const SyntaxContainer::GetCode(Context const& context, int index) const
{
    auto container = context.Container(*this, index);
    return statements[index]->Code(*container);
};
