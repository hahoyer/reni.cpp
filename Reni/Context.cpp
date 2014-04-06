#include "Import.h"
#include "Context.h"

#include "DefineableToken .h"
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
    class DefinableTokenFeatureProvider final : public ContextFeatureProvider<DefineableToken>{
        typedef ContextFeatureProvider<DefineableToken> baseType;
        typedef DefinableTokenFeatureProvider thisType;

        class Feature final : public ContextFeature{
            typedef ContextFeature baseType;
            typedef Feature thisType;
        private:
            virtual ResultData const FunctionResult(
                Context const&context,
                Category category,
                Ref<Syntax, true> const& right
            )const override;
            p_function(Array<String>, DumpData) override{return{};}
        };

    public:
        ThisRef;

    private:
        p_function(Ref<ContextFeature>, feature) override{return new Feature();}
        p_function(Array<String>, DumpData) override{return{};}
    };

    class ContainerContext final : public Context, public RefCountProvider{
        typedef ContainerContext thisType;
        Context const& context;
        Ref<SyntaxContainer> containerData;
        int const index;
    public:

        ContainerContext(Context const&context, SyntaxContainer const&containerData, int index)
            : context(context)
              , token(new DefinableTokenFeatureProvider)
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

        p_function(WeakRef<Global>, global) override{return context.global;}
        Ref<DefinableTokenFeatureProvider> const token;
        operator Ref<ContextFeatureProvider<DefineableToken>, true>() const override{return token->thisRef;}
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

        p_function(Size, size) override{
            return 0;
        }

        p_function(WeakRef<Global>, global) override{
            return context.global;
        };
    };
};


ResultData const DefinableTokenFeatureProvider::Feature::FunctionResult(Context const& context, Category category, Ref<Syntax, true> const& right) const{
    md(context, category, right);
    b_;
    return{};
}


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
    auto featureClasses = tokenClass.featureClasses;
    WeakPtr<Type> type;
    if(!left.IsEmpty)
        type = left->Type(*this)->thisRef;
    auto results =
        left.IsEmpty
        ? featureClasses.Select<SearchResult>([&](WeakRef<FeatureClass> fc){return fc->GetDefinition(*this); })
        : featureClasses.Select<SearchResult>([&](WeakRef<FeatureClass> fc){return fc->GetDefinition(*type); });
    return results
        ->Where([&](SearchResult const& result){return result.IsValid; })
        ->FirstOrEmpty;
}

Context::operator Ref<ContextFeatureProvider<MinusToken>, true>() const{
    md_;
    mb;
}

Context::operator Ref<ContextFeatureProvider<DefineableToken>, true>() const{
    md_;
    mb;
}

WeakRef<Context> const Context::Container(SyntaxContainer const& syntax, int index) const{
    return _internal->container(&syntax, index)->thisRef;
}


WeakRef<Type> const Context::FunctionType(FunctionSyntax const& body) const{
    return _internal->functionType(&body)->thisRef;
}

SearchResult const Context::GetDefinition(DefineableToken const&token) const{
    md(token);
    mb;
}