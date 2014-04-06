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

pure_p_implementation(TokenClass, Array<WeakRef<FeatureClass>>, featureClasses);
pure_p_implementation(TokenClass, bool, AcceptsMatch);
