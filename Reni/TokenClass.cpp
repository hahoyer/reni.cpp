#include "Import.h"
#include "TokenClass.h"
#include "Syntax.h"

using namespace Reni;
static bool Trace = true;


Ref<Syntax const> const TokenClass::Create(Ref<Syntax const>const left, SourcePart part, Ref<Syntax const>const right)const{
    return new InfixSyntax(left, *this, right);
};

Ref<Syntax const> const TokenClass::Create(Ref<Syntax const>const left, SourcePart part)const{
    return new SuffixSyntax(left, *this);
};

Ref<Syntax const> const TokenClass::Create(SourcePart part, Ref<Syntax const>const right)const{
    return new PrefixSyntax(*this, right);
};

Ref<Syntax const> const TokenClass::Create(SourcePart part)const{
    return new TerminalSyntax(*this, part);
}

p_implementation(TokenClass, Array<String>, DumpData){
    return List<String>{};
};

