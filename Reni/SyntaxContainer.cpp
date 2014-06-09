#include "Import.h"
#include "SyntaxContainer.h"

#include "ExpressionSyntax.h"
#include "CodeItems.h"
#include "DefineableToken.h"
#include "../HWLib/RefCountContainer.instance.h"
#include "../HWLang/Admin/Export.h"

using namespace HWLib;
using namespace Reni;
static bool Trace = true;


class EmptySyntax final : public Syntax
{
    using baseType = Syntax; 
    using thisType = EmptySyntax;
public:
    explicit EmptySyntax(SourcePart const& part)
        : Syntax(part) {}
private:
    p_function(String, SmartDump) override{ return ""; };
    p_function(int, priority) override{ return 0; }
    void AddTo(SyntaxContainer& main) const override{main.statements += thisRef;}
};


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
    if(category == Category::Code)
        return GetCode(context);

    md(context, category);
    b_;
    return{};
}

void SyntaxContainer::Add(Optional<Ref<Syntax>> const& value)
{
    Ref<Syntax> const effectiveValue = value || emptySyntax; 
    effectiveValue->AddTo(*this);
}

Ref<CodeItem> const SyntaxContainer::GetCode(Context const& context) const
{
    Array<Ref<CodeItem>> result
        (
            statements.Count,
            [&]
            (int index)
            {
                return GetCode(context, index);
            }
        );
    md(context, result);
    mb;
};

Ref<CodeItem> const SyntaxContainer::GetCode(Context const& context, int index) const
{
    auto container = context.Container(*this, index);
    return statements[index]->Code(*container);
};