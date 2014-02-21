#include "Import.h"
#include "TokenClass.h"
#include "Context.h"
#include "Syntax.h"

static bool Trace = true;

using namespace Reni;

static int nextObjectId = 0;

TokenClass::TokenClass() : baseType(nextObjectId++){};

Ref<Syntax> const TokenClass::CreateSyntax(Ref<Syntax >const left, SourcePart const&part, Ref<Syntax >const right, bool isMatch)const{
    d_here;
    md(left, part, right, isMatch);
    b_;
    return OptRef<Syntax >();
};

Ref<Syntax> const TokenClass::CreateSyntax(Ref<Syntax >const left, SourcePart const&part, bool isMatch)const{
    d_here;
    md(left, part, isMatch);
    b_;
    return OptRef<Syntax >();
};

Ref<Syntax> const TokenClass::CreateSyntax(SourcePart const&part, Ref<Syntax >const right, bool isMatch)const{
    d_here;
    md(part, right, isMatch);
    b_;
    return OptRef<Syntax >();
};

Ref<Syntax> const TokenClass::CreateSyntax(SourcePart const&part, bool isMatch)const{
    md(part, isMatch);
    b_;
    return OptRef<Syntax >();
};
