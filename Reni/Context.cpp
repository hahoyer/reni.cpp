#include "Import.h"
#include "Context.h"


#include "AccessData.h"
#include "Result.h"
#include "AccessFeature.h"
#include "AccessType.h"
#include "CodeFunction.h"
#include "ContainerContext.h"
#include "ContextIncludes.h"
#include "DefineableToken.h"
#include "FunctionCallContext.h"
#include "FunctionCallResultCache.h"
#include "RecursionContext.h"
#include "ReplaceVisitor.h"
#include "ResultDataDirect.h"
#include "SyntaxContainer.h"

#include "../HWLib/RefCountContainer.instance.h"

using namespace Reni;
static bool Trace = true;

SimpleFeature::SimpleFeature(AccessData const&data)
    : data(data.thisRef)
{}

ResultData const SimpleFeature::Result(Context const& , Category category) const
{
    return data->resultCache->Get(category);
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
p_virtual_header_implementation(Context, WeakRef<FunctionCallContext>, functionContext);
p_virtual_header_implementation(Context, WeakRef<RecursionContext>, recursionContext);

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
    , accessData([&](int statementIndex)
        {
            return new AccessData(*this, statementIndex);
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
    , asExternal(*new External::Context(thisRef))
{
    SetDumpString();
};


p_implementation(ContainerContext, Size, dataSize){return containerData->Size(parent);}

Ref<ResultCache> const ContainerContext::AccessResult(Type const& argsType, int statementIndex) const
{
    return functionCallContext(&argsType)->functionCallResultCache(statementIndex)->thisRef;
}

SearchResult<AccessFeature> const ContainerContext::DeclarationsForType(DefineableToken const&token) const 
{
    if(containerData->names.ContainsKey(&token))
    {
        auto data = accessData(containerData->names[&token]);
        return data->feature->thisRef;
    }
    return baseType::DeclarationsForType(token);
}

Size const ContainerContext::AlignedPartSize(int position) const
{
    return PartSize(position).Align(alignBits);
}

Size const ContainerContext::PartSize(int position) const
{
    return accessData(position)->dataResultCache->size;
}


FunctionCallResultCache::FunctionCallResultCache(FunctionCallContext const& context, int bodyIndex) : context(context)
, bodyIndex(bodyIndex)
{
    SetDumpString();
}

ResultData const FunctionCallResultCache::GetResultDataRecursive(Category category) const
{
    return GetResultData(category);
}

ResultData const FunctionCallResultCache::GetResultData(Category category) const
{
    a_is(category, != , Category::None);
    return ResultData::GetSmartHllwSize(category, l_(codeGet), l_(valueType), l_(extsGet));
}

p_implementation(FunctionCallResultCache, int, codeIndex){ return context.global->FunctionIndex(*this); };

p_implementation(FunctionCallResultCache, FunctionSyntax const&, body)
{
    return dynamic_cast<FunctionSyntax const&>(*context.container.containerData->statements[bodyIndex]);
};

p_implementation(FunctionCallResultCache, Ref<CodeItem>, codeGet)
{
    auto Trace = context.ObjectId == 4 && bodyIndex == 1; 
    md_;
    a_if(!arg.IsEmpty, "NotImpl: no arg " + Dump);
    a_if(!body.getter.IsEmpty, "NotImpl: no function getter " + Dump);
    function.GetterIsUsed();
    auto rawResult
        = body
        .getter
        .Value
        ->GetResultCache(context)
        ->Get(Category::Type | Category::Exts);
    d(rawResult);
    auto result = rawResult
        .Convert(*valueType);
    if(result.exts.Value == External::Function::Arg::Instance)
        return CodeItem::CallGetter(valueType->size, codeIndex, *arg.Value);

    md(result);
    mb;
}

p_implementation(FunctionCallResultCache, Externals, extsGet)
{
    return body
        .getter
        .Value
        ->GetResultCache(context)
        ->exts 
        - External::Function::Arg::Instance;
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
        Ref<ResultCache> functionArg = new ResultDataDirect(CodeItem::FunctionArg(*arg.Value), arg.Value->IndirectType(1));
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


p_implementation(RecursionContext, WeakRef<RecursionContext>, recursionContext)
{
    md_;
    mb;
}

WeakRef<Type> const RecursionContext::FunctionType(FunctionSyntax const& body) const
{
    md(body);
    mb;
}

Optional<WeakRef<Type>> const RecursionContext::CachedType(Syntax const& target) const
{
    return target.CachedType(parent);
}

WeakRef<ContainerContext> const RecursionContext::Container(SyntaxContainer const& statements, int viewIndex) const
{
    return parent.Container(statements, viewIndex);
}

SearchResult<AccessFeature> const RecursionContext::DeclarationsForType(DefineableToken const& token) const
{
    return parent.DeclarationsForType(token);
}

SearchResult<AccessFeature> const ChildContext::DeclarationsForType(DefineableToken const& token) const
{
    return parent.DeclarationsForType(token);
}


FunctionBodyType::FunctionBodyType(Context const& context, FunctionSyntax const& body) : context(context)
, body(body.thisRef)
{
}


p_implementation(FunctionBodyType, WeakRef<Global>, global){return context.global;}

p_implementation(ContainerType, Size ,size){return parent.dataSize;}
p_implementation(ContainerType, WeakRef<Global>, global){return parent.global;}

String const External::Context::internalDump() const
{
    return target.DumpShort;
}

Externals const External::Context::Replace(ReplaceVisitor const& arg) const
{
    return arg.GetExts(*this);
}