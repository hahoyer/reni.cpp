#include "Import.h"

#include "TokenClass.h"
#include "Syntax.h"

using namespace Reni;
static bool Trace = true;


Ref<Syntax const> const TokenClass::CreateSyntax(Ref<Syntax const>const left, SourcePart const&, Ref<Syntax const>const right, bool )const{
    return new InfixSyntax(left, *this, right);
};

Ref<Syntax const> const TokenClass::CreateSyntax(Ref<Syntax const>const left, SourcePart const&, bool )const{
    return new SuffixSyntax(left, *this);
};

Ref<Syntax const> const TokenClass::CreateSyntax(SourcePart const&, Ref<Syntax const>const right, bool)const{
    return new PrefixSyntax(*this, right);
};

Ref<Syntax const> const TokenClass::CreateSyntax(SourcePart const&part, bool )const{
    return new TerminalSyntax(*this, part);
}

