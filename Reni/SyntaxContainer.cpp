#include "Import.h"
#include "SyntaxContainer.h"

#include "ExpressionSyntax.h"
#include "DefineableToken .h"
#include "../HWLib/RefCountContainer.instance.h"

using namespace Reni;
static bool Trace = true;


SyntaxContainer::SyntaxContainer(SourcePart const&part) : baseType(part){
}

p_implementation(SyntaxContainer, Array<String>, DumpData){
    return{};
};

void SyntaxContainer::Add(Ref<Syntax> const& definitionTarget, Ref<Syntax> const& value){
    auto& e = dynamic_cast<ExpressionSyntax const&>(*definitionTarget);
    a_if_(e.left.IsEmpty);
    a_if_(e.right.IsEmpty);
    auto& d = dynamic_cast<DefineableToken const&>(e.tokenClass);
    names.Assign(d.name, statements.Count);
    statements += value;
}