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
    : minus(new MinusFeatureProvider){};
    Ref<MinusFeatureProvider> const minus;
};


RootContext::RootContext()
: _internal(new internal)
, _global(new Global){
    SetDumpString();
}

RootContext::operator Ref<ContextFeatureProvider<MinusToken>, true>() const{ return _internal->minus->thisRef; }

RootContext::operator Ref<ContextFeatureProvider<DefineableToken>, true>() const {
    md_;
    b_;
    return{};
}

p_implementation(RootContext, Array<String>, DumpData){
    return{};
};


