#include "Import.h"
#include "Context.h"

#include "ContextIncludes.h"
#include "DefineableToken.h"
#include "ExpressionSyntax.h"
#include "FunctionSyntax.h"
#include "RecursionContext.h"
#include "Result.h"
#include "SearchResult.h"
#include "Syntax.h"
#include "SyntaxContainer.h"
#include "Type.h"

#include "../HWLib/RefCountContainer.instance.h"

using namespace Reni;
static bool Trace = true;


ResultData const DefinableTokenFeatureProvider::Feature::FunctionResult(Context const& context, Category category, Ref<Syntax, true> const& right) const
{
    md(context, category, right);
    b_;
    return{};
}


struct RegularContext::internal final
{
    FunctionCache<WeakRef<ContainerContext>, SyntaxContainer const*, int> container;
    FunctionCache<WeakRef<Reni::FunctionType>, FunctionSyntax const*> functionType;
    ValueCache<WeakRef<RecursionContext>> recursionContext;

    explicit internal(RegularContext const&context)
        : container([&](SyntaxContainer const*containerData, int index)
              {
                  return new ContainerContext(context, *containerData, index);
              })
          , functionType([&](FunctionSyntax const*body)
              {
                  return new Reni::FunctionType(context, *body);
              })
          , recursionContext(l_(new RecursionContext(context)))
    {
    };
};


RegularContext::RegularContext()
    :_internal(new internal(*this))
{
}

pure_p_implementation(Context, bool, isRecursion) ;
pure_p_implementation(Context, WeakRef<Global>, global) ;
pure_p_implementation(Context, WeakRef<FunctionCallContext>, functionContext) ;

SearchResult const Context::Search(Ref<Syntax, true> const&left, DefineableToken const&tokenClass)const
{
    if(left.IsEmpty)
        return Search(tokenClass);

    return left
        ->Type(*this)
        ->thisRef
        .Search(tokenClass);
}

p_implementation(RegularContext, WeakRef<RecursionContext>, recursionContext)
{
    return _internal->recursionContext.Value;
}

RegularContext::operator Ref<ContextFeatureProvider<MinusToken>, true>() const
{
    md_;
    mb;
}

RegularContext::operator Ref<ContextFeatureProvider<DefineableToken>, true>() const
{
    md_;
    mb;
}

WeakRef<Context> const RegularContext::Container(SyntaxContainer const& syntax, int index) const
{
    return _internal->container(&syntax, index)->thisRef;
}


WeakRef<Type> const RegularContext::FunctionType(FunctionSyntax const& body) const
{
    return _internal->functionType(&body)->thisRef;
}

SearchResult const Context::Search(DefineableToken const&token) const
{
    md(token);
    mb;
}

ResultData Context::ArgReferenceResult(Category category) const
{
    WeakRef<FunctionCallContext> x = functionContext;
    return x
        ->CreateArgReferenceResult(category);
}

AccessFeature::AccessFeature(ContainerContext const& container, int tokenIndex)
    : container(container)
      , tokenIndex(tokenIndex)
{
    SetDumpString();
}


p_implementation(AccessFeature, Array<String>, DumpData)
{
    return{nd(container), nd(tokenIndex)};
}


ResultData const AccessFeature::FunctionResult(Context const& context, Category category, ExpressionSyntax const& expressionSyntax) const
{
    auto argsType = expressionSyntax.right->Type(context);
    auto result = container.FunctionCallResult(*argsType, tokenIndex);
    return result->Get(category);
};


ContainerContext::ContainerContext(RegularContext const&parent, SyntaxContainer const&containerData, int index)
    : baseType(parent)
      , token(new DefinableTokenFeatureProvider)
      , containerData(containerData.thisRef)
      , accessFeature([&](int tokenIndex)
          {
              return new AccessFeature(*this, tokenIndex);
          })
      , functionCallResultCache([&](Type const*args, Syntax const*body)
          {
              return new FunctionCallResultCache(*this, args, *body);
          })
      , dataTypeCache([&]
          {
              return new ContainerType(*this);
          })
      , index(index)
{
    SetDumpString();
};


p_implementation(ContainerContext, Size, dataSize)
{
    return containerData->Size(parent);
}

Ref<FunctionCallResultCache> const ContainerContext::FunctionCallResult(Type const& argsType, int const tokenIndex) const
{
    return functionCallResultCache(&argsType, &*containerData->statements[tokenIndex]);
}

ResultData const FunctionCallResultCache::GetResultData(Category category) const
{
    if(category == Category::None)
    {
        a_if(pending == Category::Type, Dump);
        a_if(!args.IsEmpty, "NotImpl: no arg " + Dump);
        auto& fs = dynamic_cast<FunctionSyntax const&>(body);
        a_if(fs.setter.IsEmpty, "NotImpl: function setter " + Dump);
        a_if(!fs.getter.IsEmpty, "NotImpl: no function getter " + Dump);
        return *fs.getter->Type(*context.recursionContext)->asFunctionResult;
    }

    if(category == Category::Type)
    {
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

FunctionCallContext::FunctionCallContext(ContainerContext const& parent, WeakRef<Type const> const args)
    : baseType(static_cast<RegularContext const&>(parent))
      , container(parent)
      , args(args)
{
    SetDumpString();
}


p_implementation(FunctionCallContext, WeakRef<Type>, objectType)
{
    return container.dataType;
}

ResultData const FunctionCallContext::CreateArgReferenceResult(Category category)const
{
    return args
        ->ContextAccessResult
        (
            category.typed,
            *objectType, [&]
            {
                return args->size * -1;
            }
        )
        & category;
};


WeakRef<Type> const RecursionContext::FunctionType(FunctionSyntax const& body) const
{
    md(body);
    mb;
}

WeakPtr<Type> const RecursionContext::CachedType(Syntax const& target) const
{
    return target.CachedType(parent);
}

WeakRef<Context> const RecursionContext::Container(SyntaxContainer const& syntax, int index) const
{
    md(syntax, index);
    mb;
}

SearchResult const RecursionContext::Search(DefineableToken const& token) const
{
    return parent.Search(token);
}