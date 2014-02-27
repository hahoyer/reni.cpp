#include "Import.h"
#include "TokenClass.h"
#include "Context.h"
#include "Syntax.h"
#include "..\HWLang\SourcePart.h"

static bool Trace = true;

using namespace Reni;
using namespace HWLang;

Ref<Syntax> const TokenClass::CreateSyntax(Ref<Syntax >const left, SourcePart const&part, Ref<Syntax >const right, bool isMatch)const{
    md(left, part, right, isMatch);
    b_;
    return OptRef<Syntax >();
};

Ref<Syntax> const TokenClass::CreateSyntax(Ref<Syntax >const left, SourcePart const&part, bool isMatch)const{
    md(left, part, isMatch);
    b_;
    return OptRef<Syntax >();
};

Ref<Syntax> const TokenClass::CreateSyntax(SourcePart const&part, Ref<Syntax >const right, bool isMatch)const{
    md(part, right, isMatch);
    b_;
    return OptRef<Syntax >();
};

Ref<Syntax> const TokenClass::CreateSyntax(SourcePart const&part, bool isMatch)const{
    md(part, isMatch);
    b_;
    return OptRef<Syntax >();
};
