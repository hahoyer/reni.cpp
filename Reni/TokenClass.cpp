#include "Import.h"
#include "TokenClass.h"

#include "Context.h"
#include "FeatureClass.h"
#include "Syntax.h"
#include "..\HWLang\SourcePart.h"

static bool Trace = true;

using namespace Reni;
using namespace HWLang;

Ref<Syntax> const TokenClass::Mismatch(Ref<Syntax, true> const left, SourcePart const& part, Ref<Syntax, true> const right) const{
    md(left, part, right);
    mb;
    return Ref<Syntax, true>();
}

Ref<Syntax> const TokenClass::Infix(Ref<Syntax> const left, SourcePart const& part, Ref<Syntax> const right) const{
    md(left, part, right);
    mb;
    return Ref<Syntax, true>();
}

Ref<Syntax> const TokenClass::Suffix(Ref<Syntax> const left, SourcePart const& part) const{
    md(left, part);
    mb;
    return Ref<Syntax, true>();
}

Ref<Syntax> const TokenClass::Sufanyfix(Ref<Syntax> const left, SourcePart const& part, Ref<Syntax, true> const right) const{
    md(left, part, right);
    mb;
    return Ref<Syntax, true>();
}

Ref<Syntax> const TokenClass::Prefix(SourcePart const& part, Ref<Syntax> const right) const{
    md(part, right);
    mb;
    return Ref<Syntax, true>();
}

Ref<Syntax> const TokenClass::Terminal(SourcePart const&part) const{
    md(part);
    mb;
    return Ref<Syntax, true>();
}

Ref<Syntax> const TokenClass::NoSuffix(SourcePart const& part, Ref<Syntax, true> const right) const{
    md(part, right);
    b_;
    auto r = HasRight.IsValid;
    return Ref<Syntax, true>();
}

Ref<Syntax> const TokenClass::Preanyfix(Ref<Syntax, true> const left, SourcePart const& part, Ref<Syntax> const right) const{
    md(left, part, right);
    mb;
    return Ref<Syntax, true>();
}

Ref<Syntax> const TokenClass::NoPrefix(Ref<Syntax, true> const left, SourcePart const& part) const{
    md(left, part);
    mb;
    return Ref<Syntax, true>();
}

Ref<Syntax> const TokenClass::Anyfix(Ref<Syntax, true> const left, SourcePart const& part, Ref<Syntax, true> const right) const{
    md(left, part, right);
    mb;
    return Ref<Syntax, true>();
}

pure_p_implementation(TokenClass, WeakRef<FeatureClass>, featureClass);
pure_p_implementation(TokenClass, Optional<bool>, HasLeft);
pure_p_implementation(TokenClass, Optional<bool>, HasRight);
