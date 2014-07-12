#include "Import.h"
#include "Context.h"

#include "AccessResultCache.h"
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
#include "ResultDataDirect.h"
#include "SearchResult.h"
#include "Syntax.h"
#include "SyntaxContainer.h"
#include "Type.h"

#include "../HWLib/RefCountContainer.instance.h"

using namespace Reni;
static bool Trace = true;

namespace Reni
{
    class AccessType final : public Type
    {
        using baseType = Type;
        using thisType = AccessType;

        int const statementIndex;
        ContainerContext const& container;
    public:
        AccessType(AccessType const&) = delete;
        AccessType(ContainerContext const& container, int statementIndex)
            : container(container)
            , statementIndex(statementIndex)
        {
            SetDumpString();
        }
        ThisRef;
    private:
        p_function(bool, hllw) { return false; };
        p_function(Array<String>, DumpData) override{ return{nd(statementIndex), nd(container)}; }
        p_function(Size, size)override { return Size::Address; }
        p_function(WeakRef<Global>, global) override{ return container.global; }
    };
};


SimpleFeature::SimpleFeature(ContainerContext const& parent, int const statementIndex)
    : statementIndex(statementIndex)
    , resultCache(new AccessResultCache(parent, statementIndex))
{}

ResultData const SimpleFeature::Result(Context const& , Category category) const
{
    return resultCache->Get(category);
}

ResultData const ExtendedFeature::Result(Context const&, Category category, Type const& right) const
{
    auto result = parent.AccessResult(right, tokenIndex);
    return result->Get(category);
}


struct RegularContext::internal final
{
    FunctionCache<WeakRef<ContainerContext>, SyntaxContainer const*, int> container;
    FunctionCache<WeakRef<FunctionBodyType>, FunctionSyntax const*> functionType;
    ValueCache<WeakRef<RecursionContext>> recursionContext;

    explicit internal(RegularContext const&context)
        : container([&](SyntaxContainer const*statements, int viewIndex)
              {
                  return new ContainerContext(context, *statements, viewIndex);
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

p_virtual_header_implementation(Context, bool, isRecursion) ;

p_virtual_header_implementation(Context, WeakRef<Global>, global) ;

p_virtual_header_implementation(Context, WeakRef<FunctionCallContext>, functionContext) ;

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

WeakRef<ContainerContext> const RegularContext::Container(SyntaxContainer const& statements, int viewIndex) const
{
    return _internal->container(&statements, viewIndex)->thisRef;
}


WeakRef<Type> const RegularContext::FunctionType(FunctionSyntax const& body) const
{
    return _internal->functionType(&body)->thisRef;
}

SearchResult<AccessFeature> const Context::DeclarationsForType(DefineableToken const&token) const
{
    md(token);
    mb;
}

ResultData const Context::ReferenceResult(Category category, External::Function const& external) const
{
    return functionContext->ReferenceResult(category, external);
}


ContainerContext::ContainerContext(RegularContext const& parent, SyntaxContainer const& containerData, int viewIndex)
    : baseType(parent)
    , containerData(containerData.thisRef)
    , accessFeature([&](int statementIndex)
        {
            return AccessFeature(*new SimpleFeature(*this, statementIndex), *new ExtendedFeature(*this, statementIndex));
        })
    , accessType([&](int statementIndex)
        {
            return new Reni::AccessType(*this, statementIndex);
        })
    , dataTypeCache([&]
        {
            return new ContainerType(*this);
        })
    , functionCallContext([&](Type const* args)
        {
            return new FunctionCallContext(*this, args);
        })
    , viewIndex(viewIndex)
{
    SetDumpString();
};


p_implementation(ContainerContext, Size, dataSize)
{
    return containerData->Size(parent);
}

WeakRef<Type> const ContainerContext::AccessType(int const statementIndex) const
{
    return accessType(statementIndex)->thisRef;
}

Ref<ResultCache> const ContainerContext::AccessResult(Type const& argsType, int const statementIndex) const
{
    return functionCallContext(&argsType)->functionCallResultCache(statementIndex)->thisRef;
}

SearchResult<AccessFeature> const ContainerContext::DeclarationsForType(DefineableToken const&token) const 
{
    if(containerData->names.ContainsKey(&token))
        return accessFeature(containerData->names[&token]);
    return baseType::DeclarationsForType(token);
}



FunctionCallResultCache::FunctionCallResultCache(FunctionCallContext const& context, int bodyIndex) : context(context)
, bodyIndex(bodyIndex)
{
    SetDumpString();
}

ResultData const FunctionCallResultCache::GetResultData(Category category) const
{
    if(category == Category::None)
        return valueInRecursion;

    return ResultData::GetSmartHllwSizeExts(category,l_(codeGet),l_(valueType));
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

p_implementation(FunctionCallResultCache, ResultData, valueInRecursion)
{
    return body
        .getter
        .Value
        ->GetResultCache(*context.recursionContext)
        ->Get(Category::Type|Category::Exts)
        .asFunctionResult;
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

WeakRef<ContainerContext> const RecursionContext::Container(SyntaxContainer const& statements, int viewIndex) const
{
    md(statements, viewIndex);
    mb;
}

SearchResult<AccessFeature> const RecursionContext::DeclarationsForType(DefineableToken const& token) const
{
    return parent.DeclarationsForType(token);
}

