#include "Import.h"
#include "FunctionToken.h"

#include "Context.h"
#include "FunctionSyntax.h"


using namespace Reni;
static bool Trace = true;


Ref<Syntax> const FunctionToken::CreateSyntax(Ref<Syntax, true> const left, SourcePart const& part, Ref<Syntax, true> const right) const{
    return new FunctionSyntax(left, part, right);
}