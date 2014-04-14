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
#include "ContextIncludes.h"

using namespace Reni;
static bool Trace = true;

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
pure_p_implementation(Context, WeakRef<FunctionCallContext>, functionContext);


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

ResultData Context::ArgReferenceResult(Category category) const{
    WeakRef<FunctionCallContext> x = functionContext;
    return x
        ->CreateArgReferenceResult(category);
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


ContainerContext::ContainerContext(Context const&parent, SyntaxContainer const&containerData, int index)
: parent(parent)
, token(new DefinableTokenFeatureProvider)
, containerData(containerData.thisRef)
, accessFeature([&](int tokenIndex){return new AccessFeature(*this, tokenIndex);})
, functionCallResultCache([&](Type const*args, Syntax const*body){return new FunctionCallResultCache(*this, args, *body);})
, dataTypeCache([&]{return new ContainerType(*this); })
, index(index){
    SetDumpString();
};


p_implementation(ContainerContext, Size, dataSize){
    return containerData->Size(parent);
}

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


p_implementation(FunctionCallContext, WeakRef<Global>, global) { return container.global; }
p_implementation(FunctionCallContext, WeakRef<Type>, objectType) { return container.dataType; }

SearchResult const FunctionCallContext::GetDefinition(DefineableToken const&token) const{
    return container.GetDefinition(token);
}

ResultData const FunctionCallContext::CreateArgReferenceResult(Category category)const{
    return args
        ->ContextAccessResult(category.typed, *objectType, [&]{return args->size * -1; })
        & category;
};
