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
            p_function(Array<String>, DumpData) override{
                return{};
            }
        };

    public:
        ThisRef;

    private:
        p_function(Ref<ContextFeature>, feature) override{
            return new Feature();
        }

        p_function(Array<String>, DumpData) override{
            return{};
        }
    };


    class ContainerContext;

    class AccessFeature final : public Feature{
        typedef Feature baseType;
        typedef AccessFeature thisType;

    public:
        ContainerContext const& container;
    private:
        int const tokenIndex;
    public:
        AccessFeature(ContainerContext const& containerContext, int tokenIndex);
    private:
        p_function(Array<String>, DumpData) override;
        ResultData const FunctionResult(Context const& context, Category category, ExpressionSyntax const& expressionSyntax) const override;
    };

    class FunctionCallResultCache;

    class ContainerContext final : public Context, public RefCountProvider{
        typedef ContainerContext thisType;
        typedef Context baseType;

        FunctionCache<Ref<FunctionCallResultCache>, Type const*, Syntax const*> functionCallResultCache;
        FunctionCache<Ref<Feature>, int> accessFeature;
        Context const& context;
    public:
        Ref<SyntaxContainer> containerData;
    private:
        int const index;
    public:

        ContainerContext(Context const&context, SyntaxContainer const&containerData, int index);

        ContainerContext(ContainerContext const&) = delete;
        ThisRef;

        Ref<FunctionCallResultCache> const FunctionCallResult(Type const& argsType, int const tokenIndex) const;
    private:
        p_function(Array<String>, DumpData) override{
            return{
                nd(context),
                nd(containerData),
                nd(index)
            };
        };

        p_function(WeakRef<Global>, global) override{
            return context.global;
        }

        Ref<DefinableTokenFeatureProvider> const token;

        operator Ref<ContextFeatureProvider<DefineableToken>, true>() const override{
            return token->thisRef;
        }

        SearchResult const GetDefinition(DefineableToken const&token) const override{
            if(containerData->names.ContainsKey(&token)){
                auto tokenIndex = containerData->names[&token];
                return accessFeature(tokenIndex);
            }
            return baseType::GetDefinition(token);
        }

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


    class FunctionCallContext final : public Context{
        typedef Context baseType; typedef FunctionCallContext thisType;
    public:
        ContainerContext const& container;
        WeakPtr<Type const> const args;
        FunctionCallContext(ContainerContext const& container,WeakPtr<Type const> const args)
        : container(container), args(args){}
    private:
        p_function(WeakRef<Global>, global) override{return container.global;};
        p_function(Array<String>, DumpData) override{return{nd(args), nd(container)};
        }
    };

    class FunctionCallResultCache final : public ResultCache{
        typedef ResultCache baseType;
        typedef FunctionCallResultCache thisType;
        Syntax const&body;
        FunctionCallContext context;
    public:
        FunctionCallResultCache(ContainerContext const& container, Type const*args, Syntax const&body)
            : context(container, args)
              , body(body.thisRef){
            SetDumpString();
        }

    private:
        p(WeakPtr<Type const>, args){ return context.args; }
        p(ContainerContext const&, container){ return context.container; }
        p_function(Array<String>, DumpData) override{return{nd(body), nd(context)};}

        ResultData const GetResultData(Category category)const override;
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
    if(category == Category::None)
        return{};
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
            ? featureClasses.Select<SearchResult>([&](WeakRef<FeatureClass> fc){
                return fc->GetDefinition(*this);
            })
            : featureClasses.Select<SearchResult>([&](WeakRef<FeatureClass> fc){
                return fc->GetDefinition(*type);
            });
    return results
        ->Where([&](SearchResult const& result){
            return result.IsValid;
        })
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


AccessFeature::AccessFeature(ContainerContext const& container, int tokenIndex)
    : container(container)
      , tokenIndex(tokenIndex){
    SetDumpString();
}


p_implementation(AccessFeature, Array<String>, DumpData){
    return{nd(container), nd(tokenIndex)};
}


ResultData const AccessFeature::FunctionResult(Context const& context, Category category, ExpressionSyntax const& expressionSyntax) const{
    auto argsType = expressionSyntax.right->Type(context);
    auto result = container.FunctionCallResult(*argsType, tokenIndex);
    return result->Get(category);
};


ContainerContext::ContainerContext(Context const&context, SyntaxContainer const&containerData, int index)
: context(context)
, token(new DefinableTokenFeatureProvider)
, containerData(containerData.thisRef)
, accessFeature([&](int tokenIndex){return new AccessFeature(*this, tokenIndex);})
, functionCallResultCache([&](Type const*args, Syntax const*body){return new FunctionCallResultCache(*this, args, *body);})
, index(index){
    SetDumpString();
};


Ref<FunctionCallResultCache> const ContainerContext::FunctionCallResult(Type const& argsType, int const tokenIndex) const{
    return functionCallResultCache(&argsType, &*containerData->statements[tokenIndex]);
}

ResultData const FunctionCallResultCache::GetResultData(Category category) const{
    if(category == Category::Type){
        a_if(!args.IsEmpty, "NotImpl: no arg "+Dump);
        auto& fs = dynamic_cast<FunctionSyntax const&>(body);
        a_if(fs.setter.IsEmpty, "NotImpl: function setter " + Dump);
        a_if(!fs.getter.IsEmpty, "NotImpl: no function getter " + Dump);
        return *fs.getter->Type(context)->asFunctionResult;
    }



    md(category);
    b_;
    return{};
}