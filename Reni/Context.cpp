#include "Import.h"
#include "Context.h"

#include "FeatureClass.h"
#include "Result.h"
#include "SearchResult.h"
#include "Syntax.h"
#include "SyntaxContainer.h"
#include "Type.h"
#include "../HWLib/RefCountContainer.instance.h"

using namespace Reni;
static bool Trace = true;


class ContainerContext final : public Context, public RefCountProvider{
    Context const& context;
    SyntaxContainer const&containerData;
    int const index;
public:
    ContainerContext(Context const&context, SyntaxContainer const&containerData, int index)
        : context(context)
        , containerData(containerData)
        , index(index){};
private:
    p_function(Array<String>, DumpData) override{ 
        return{
            nd(context), 
            nd(containerData), 
            nd(index)
        }; 
    };
    p_function(WeakRef<Global>, global) override{ return context.global; };
};


struct Context::internal final{
    FunctionCache<SyntaxContainer const*, CtrlRef<FunctionCache<int, Ref<ContainerContext>>>> container;

    internal(Context const&context)
        : container([&](SyntaxContainer const*containerData)
    {
        return new FunctionCache<int, Ref<ContainerContext>>
            ([&](int index){return new ContainerContext(context, *containerData, index);});
    }){};
};


Context::Context()
:_internal(new internal(*this)){
}

ResultData const Context::GetResultData(Category category, Syntax const&syntax)const{
    return syntax.GetResultData(*this, category);
}

pure_p_implementation(Context, WeakRef<Global>, global);


SearchResult const Context::Search(Ref<Syntax, true> const&left, TokenClass const&tokenClass)const{
    if(left.IsEmpty)
        return tokenClass.featureClass->GetDefinition(*this);
    return tokenClass.featureClass->GetDefinition(*left->Type(*this));
}

Context::operator Ref<ContextFeatureProvider<MinusToken>, true>() const{
    md_;
    mb;
}

Context::operator Ref<ContextFeatureProvider<UserDefinedToken>, true>() const {
    md_;
    mb;
}

WeakRef<Context> const Context::Container(SyntaxContainer const& syntax, int index) const{
    auto s = _internal->container[&syntax];
    Ref<ContainerContext> t = (*s)[index];
    return *t;
}
