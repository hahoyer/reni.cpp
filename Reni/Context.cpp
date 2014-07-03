#include "Import.h"
#include "Context.h"

#include "CodeFunction.h"
#include "CodeItems.h"
#include "ContextIncludes.h"
#include "DefineableToken.h"
#include "ExpressionSyntax.h"
#include "FunctionCallContext.h"
#include "FunctionCallResultCache.h"
#include "FunctionSyntax.h"
#include "Global.h"
#include "RecursionContext.h"
#include "ReplaceVisitor.h"
#include "Result.h"
#include "SearchResult.h"
#include "Syntax.h"
#include "SyntaxContainer.h"
#include "Type.h"

#include "../HWLib/RefCountContainer.instance.h"
#include "ResultDataDirect.h"

using namespace Reni;
static bool Trace = true;


ResultData const SimpleFeature::Result(Context const& context, Category category) const
{
    md(context, category);
    b_;
    return{};
}

ResultData const ExtendedFeature::Result(Context const&, Category category, Type const& right) const
{
    auto result = parent.FunctionCallResult(right, tokenIndex);
    return result->Get(category);
}


struct RegularContext::internal final
{
    FunctionCache<WeakRef<ContainerContext>, SyntaxContainer const*, int> container;
    FunctionCache<WeakRef<FunctionBodyType>, FunctionSyntax const*> functionType;
    ValueCache<WeakRef<RecursionContext>> recursionContext;

    explicit internal(RegularContext const&context)
        : container([&](SyntaxContainer const*containerData, int index)
              {
                  return new ContainerContext(context, *containerData, index);
              })
          , functionType([&](FunctionSyntax const*body)
              {
                  return new FunctionBodyType(context, *body);
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

p_implementation(RegularContext, WeakRef<RecursionContext>, recursionContext)
{
    return _internal->recursionContext.Value;
}

RegularContext::operator Optional<Ref<ContextFeatureProvider<MinusToken>>>() const
{
    md_;
    mb;
}

RegularContext::operator Optional<Ref<ContextFeatureProvider<DefineableToken>>>() const
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

SearchResult<ContextFeature> const Context::DeclarationsForType(DefineableToken const&token) const
{
    md(token);
    mb;
}

ResultData const Context::ReferenceResult(Category category, External::Function const& external) const
{
    return functionContext->ReferenceResult(category, external);
}


ContainerContext::ContainerContext(RegularContext const&parent, SyntaxContainer const&containerData, int index)
    : baseType(parent)
      , containerData(containerData.thisRef)
      , accessFeature([&](int tokenIndex)
          {
              return ContextFeature(*new SimpleFeature(*this, tokenIndex), *new ExtendedFeature(*this, tokenIndex));
          })
      , dataTypeCache([&]
          {
              return new ContainerType(*this);
          })
      , functionCallContext([&](Type const* args)
          {
              return new FunctionCallContext(*this, args);
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
    return functionCallContext(&argsType)->functionCallResultCache(tokenIndex);
}

SearchResult<ContextFeature> const ContainerContext::DeclarationsForType(DefineableToken const&token) const 
{
    if(containerData->names.ContainsKey(&token))
    {
        auto tokenIndex = containerData->names[&token];
        return accessFeature(tokenIndex);
    }
    return baseType::DeclarationsForType(token);
}


ResultData const FunctionCallResultCache::GetResultData(Category category) const
{
    if(category == Category::None)
        return *valueTypeInRecursion;

    return ResultData::GetSmartSizeExts(category,l_(codeGet),l_(valueType));
}

p_implementation(FunctionCallResultCache, int, codeIndex){ return context.global->FunctionIndex(*this); };

p_implementation(FunctionCallResultCache, FunctionSyntax const&, body)
{
    return dynamic_cast<FunctionSyntax const&>(*context.container.containerData->statements[bodyIndex]);
};

p_implementation(FunctionCallResultCache, Ref<CodeItem>, codeGet)
{
    a_if(!arg.IsEmpty, "NotImpl: no arg " + Dump);
    a_if(!body.getter.IsEmpty, "NotImpl: no function getter " + Dump);
    function.GetterIsUsed();
    auto result = body
        .getter
        .Value
        ->GetResultCache(context)
        ->Get(Category::Type | Category::Exts)
        .Convert(*valueType);
    if(result.exts.Value == External::Function::Arg::Instance)
        return CodeItem::CallGetter(valueType->size, codeIndex, *arg);
    
    md(result);
    mb;
}

p_implementation(FunctionCallResultCache, CodeFunction, getter)
{
    a_if(!arg.IsEmpty, "NotImpl: no arg " + Dump);
    a_if(!body.getter.IsEmpty, "NotImpl: no function getter " + Dump);
    auto rawResult = body
        .getter
        .Value
        ->GetResultCache(context)
        ->Get(Category::Type | Category::Code | Category::Exts)
        .Convert(*valueType);
    if(rawResult.exts.Value == External::Function::Arg::Instance)
    {
        ReplaceVisitor visitor;
        Ref<ResultCache> functionArg = new ResultDataDirect(CodeItem::FunctionArg(*arg), arg->IndirectType(1)->thisRef);
        visitor.SetResults(External::Function::Arg::Instance, *functionArg);
        auto result = rawResult.Replace(visitor);
        return CodeFunction::Getter(codeIndex, result.code.Value);
    }
    md(rawResult);
    mb;
}

p_implementation(FunctionCallResultCache, CodeFunction, setter)
{
    md_;
    mb;
}

p_implementation(FunctionCallResultCache, WeakRef<Type>, valueType)
{
    return body
        .getter
        .Value
        ->Type(context)
        ->asFunctionResult;
}

p_implementation(FunctionCallResultCache, WeakRef<Type>, valueTypeInRecursion)
{
    return body
        .getter
        .Value
        ->Type(*context.recursionContext)
        ->asFunctionResult;
}


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

SearchResult<ContextFeature> const RecursionContext::DeclarationsForType(DefineableToken const& token) const
{
    return parent.DeclarationsForType(token);
}

