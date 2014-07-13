#include "Import.h"
#include "FunctionToken.h"

#include "ContainerContext.h"
#include "FunctionSyntax.h"
#include "SyntaxContainer.h"


using namespace Reni;
static bool Trace = true;


Ref<Syntax> const FunctionToken::CreateSyntax(Optional<Ref<Syntax>> const left, SourcePart const& part, Optional<Ref<Syntax>> const right) const{
    return new FunctionSyntax(left, part, right);
}