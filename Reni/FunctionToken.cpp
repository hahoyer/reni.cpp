#include "Import.h"
#include "FunctionToken.h"

using namespace Reni;
static bool Trace = true;


Ref<Syntax> const FunctionToken::CreateSyntax(Ref<Syntax, true> const left, SourcePart const& part, Ref<Syntax, true> const right) const{
    md(left, part, right);
    b_;
    return left;
}