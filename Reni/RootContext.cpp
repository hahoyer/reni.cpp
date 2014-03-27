#include "Import.h"
#include "RootContext.h"

#include "ContextFeature.h"
#include "ContextFeatureProvider.h"
#include "FeatureProvider.h"
#include "Global.h"
#include "Result.h"
#include "MinusFeatureProvider.h"
#include "Syntax.h"
#include "../HWLib/RefCountContainer.instance.h"

using namespace Reni;
static bool Trace = true;

class RootContext::internal final{
public:
    internal()
    : minus(new MinusFeatureProvider)
    , symbol(new SymbolFeatureProvider) {};
    Ref<MinusFeatureProvider> const minus;
    Ref<SymbolFeatureProvider> const symbol;
};


RootContext::RootContext()
: _internal(new internal)
, _global(new Global){
    SetDumpString();
}

RootContext::operator Ref<ContextFeatureProvider<MinusToken>, true>() const{ return _internal->minus->ref; }
RootContext::operator Ref<ContextFeatureProvider<UserDefinedToken>, true>() const { return _internal->symbol->ref;}

p_implementation(RootContext, Array<String>, DumpData){
    return{};
};


