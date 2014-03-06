#include "Import.h"
#include "TokenClass.h"

#include "Context.h"
#include "FeatureClass.h"
#include "Syntax.h"
#include "..\HWLang\SourcePart.h"

static bool Trace = true;

using namespace Reni;
using namespace HWLang;

CtrlRef<Syntax> const TokenClass::CreateSyntax(CtrlRef<Syntax >const left, SourcePart const&part, CtrlRef<Syntax >const right, bool isMatch)const{
    md(left, part, right, isMatch);
    b_;
    return CtrlPtr<Syntax >();
};

CtrlRef<Syntax> const TokenClass::CreateSyntax(CtrlRef<Syntax >const left, SourcePart const&part, bool isMatch)const{
    md(left, part, isMatch);
    b_;
    return CtrlPtr<Syntax >();
};

CtrlRef<Syntax> const TokenClass::CreateSyntax(SourcePart const&part, CtrlRef<Syntax >const right, bool isMatch)const{
    md(part, right, isMatch);
    b_;
    return CtrlPtr<Syntax >();
};

CtrlRef<Syntax> const TokenClass::CreateSyntax(SourcePart const&part, bool isMatch)const{
    md(part, isMatch);
    b_;
    return CtrlPtr<Syntax >();
};

pure_p_implementation(TokenClass, WeakRef<FeatureClass>, featureClass);
