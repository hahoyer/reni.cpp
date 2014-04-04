#include "Import.h"
#include "Context.h"

#include "FeatureClass.h"
#include "FunctionSyntax.h"
#include "Result.h"
#include "SearchResult.h"
#include "Syntax.h"
#include "SyntaxContainer.h"
#include "Type.h"
#include "../HWLib/RefCountContainer.instance.h"

using namespace Reni;
static bool Trace = true;

namespace Reni{
    class ContainerContext final : public Context, public RefCountProvider{
        typedef ContainerContext thisType;
        Context const& context;
        Ref<SyntaxContainer> containerData;
        int const index;
    public:

        ContainerContext(Context const&context, SyntaxContainer const&containerData, int index)
            : context(context)
              , containerData(containerData.thisRef)
              , index(index){
            SetDumpString();
        };

        ContainerContext(ContainerContext const&) = delete;
        ThisRef;

    private:
        p_function(Array<String>, DumpData) override{
            return{
                nd(context),
                nd(containerData),
                nd(index)
            };
        };

        p_function(WeakRef<Global>, global) override{return context.global;};
    };


    class FunctionType final : public Type{
        typedef Type baseType;
        typedef FunctionType thisType;
        Context const& context;
        Ref<FunctionSyntax> body;
    public:

        FunctionType(Context const& context, FunctionSyntax const& body) 
            : context(context)
            , body(body.thisRef){
        }

        ThisRef;

    private:
        p_function(Array<String>, DumpData) override{
            return{
                nd(context),
                nd(body)
            };
        };
        p_function(Size, size) override{ return 0; }
        p_function(WeakRef<Global>, global) override{ return context.global; };
    };
};


struct Context::internal final{
    FunctionCache<Ref<ContainerContext>, SyntaxContainer const*, int> container;
    FunctionCache<WeakRef<Reni::FunctionType>, FunctionSyntax const*> functionType;

    internal(Context const&context)
        : container([&](SyntaxContainer const*containerData, int index){
              return new ContainerContext(context, *containerData, index);
          })
          , functionType([&](FunctionSyntax const*body){
              return new Reni::FunctionType(context, *body);
          }){
    };
};


Context::Context()
    :_internal(new internal(*this)){
}

ResultData const Context::GetResultData(Category category, Syntax const&syntax)const{
    return syntax.GetResultData(*this, category);
}

pure_p_implementation(Context, WeakRef<Global>, global) ;


SearchResult const Context::Search(Ref<Syntax, true> const&left, TokenClass const&tokenClass)const{
    if(left.IsEmpty)
        return tokenClass.featureClass->GetDefinition(*this);
    return tokenClass.featureClass->GetDefinition(*left->Type(*this));
}

Context::operator Ref<ContextFeatureProvider<MinusToken>, true>() const{
    md_;
    mb;
}

Context::operator Ref<ContextFeatureProvider<UserDefinedToken>, true>() const{
    md_;
    mb;
}

WeakRef<Context> const Context::Container(SyntaxContainer const& syntax, int index) const{
    return _internal->container(&syntax, index)->thisRef;
}


WeakRef<Type> const Context::FunctionType(FunctionSyntax const& body) const{
    return _internal->functionType(&body)->thisRef;
}