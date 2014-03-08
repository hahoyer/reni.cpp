#include "Import.h"
#include "TokenClass.h"

#include "Context.h"
#include "FeatureClass.h"
#include "Syntax.h"
#include "..\HWLang\SourcePart.h"

static bool Trace = true;

using namespace Reni;
using namespace HWLang;

Ref<Syntax> const TokenClass::CreateSyntax(Ref<Syntax >const left, SourcePart const&part, Ref<Syntax >const right, bool isMatch)const{
    md(left, part, right, isMatch);
    mb;
    return Ref<Syntax,true>();
};

Ref<Syntax> const TokenClass::CreateSyntax(Ref<Syntax >const left, SourcePart const&part, bool isMatch)const{
    md(left, part, isMatch);
    mb;
    return Ref<Syntax, true>();
};

Ref<Syntax> const TokenClass::CreateSyntax(SourcePart const&part, Ref<Syntax >const right, bool isMatch)const{
    md(part, right, isMatch);
    mb;
    return Ref<Syntax, true>();
};

Ref<Syntax> const TokenClass::CreateSyntax(SourcePart const&part, bool isMatch)const{
    md(part, isMatch);
    mb;
    return Ref<Syntax, true>();
};

pure_p_implementation(TokenClass, WeakRef<FeatureClass>, featureClass);
