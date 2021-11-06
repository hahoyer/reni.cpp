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

SimpleFeature::SimpleFeature(const AccessData& data)
  : data(data.thisRef)
{}

const ResultData SimpleFeature::Result(const Context&, Category category) const
{
  return data->resultCache->Get(category);
}

const ResultData ExtendedFeature::Result(const Context&, Category category, const Type& right) const
{
  auto result = parent.AccessResult(right, tokenIndex);
  return result->Get(category);
}


struct RegularContext::internal final
{
  FunctionCache<WeakRef<ContainerContext>, const SyntaxContainer*, size_t> container;
  FunctionCache<WeakRef<FunctionBodyType>, const FunctionSyntax*> functionType;
  ValueCache<WeakRef<RecursionContext>> recursionContext;

  explicit internal(const RegularContext& context)
    : container([&](const SyntaxContainer* statements, size_t viewIndex)
      {
        return new ContainerContext(context, *statements, viewIndex);
      })
      , functionType([&](const FunctionSyntax* body)
      {
        return new FunctionBodyType(context, *body);
      })
      , recursionContext(l_(new RecursionContext(context)))
  { };
};


RegularContext::RegularContext()
  : _internal(new internal(*this))
{}

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
  HW_D_METHOD_;
  HW_BREAK_AND_THROW;
}

RegularContext::operator Optional<Ref<ContextFeatureProvider<DefineableToken>>>() const
{
  HW_D_METHOD_;
  HW_BREAK_AND_THROW;
}

const WeakRef<ContainerContext> RegularContext::Container(const SyntaxContainer& statements, size_t viewIndex) const
{
  return _internal->container(&statements, viewIndex)->thisRef;
}


const WeakRef<Type> RegularContext::FunctionType(const FunctionSyntax& body) const
{
  return _internal->functionType(&body)->thisRef;
}

const SearchResult<AccessFeature> Context::DeclarationsForType(const DefineableToken& token) const
{
  HW_D_METHOD(token);
  HW_BREAK_AND_THROW;
}

const ResultData Context::ReferenceResult(Category category, const External::Function& external) const
{
  return functionContext->ReferenceResult(category, external);
}


ContainerContext::ContainerContext(const RegularContext& parent, const SyntaxContainer& containerData, size_t viewIndex)
  : baseType(parent)
    , containerData(containerData.thisRef)
    , accessData([&](size_t statementIndex)
    {
      return new AccessData(*this, statementIndex);
    })
    , dataTypeCache([&]
    {
      return new ContainerType(*this);
    })
    , functionCallContext([&](const Type* args)
    {
      return new FunctionCallContext(*this, args);
    })
    , viewIndex(viewIndex)
    , asExternal(*new External::Context(thisRef))
{
  SetDumpString();
};


p_implementation(ContainerContext, Size, dataSize) { return containerData->GetSize(parent); }

const Ref<ResultCache> ContainerContext::AccessResult(const Type& argsType, size_t statementIndex) const
{
  return functionCallContext(&argsType)->functionCallResultCache(statementIndex)->thisRef;
}

const SearchResult<AccessFeature> ContainerContext::DeclarationsForType(const DefineableToken& token) const
{
  if(containerData->names.ContainsKey(&token))
  {
    auto data = accessData(containerData->names[&token]);
    return data->feature->thisRef;
  }
  return baseType::DeclarationsForType(token);
}

const Size ContainerContext::AlignedPartSize(size_t position) const
{
  return PartSize(position).Align(alignBits);
}

const Size ContainerContext::PartSize(size_t position) const
{
  return accessData(position)->dataResultCache->size;
}

p_implementation(ContainerContext, Array<string>, DumpData) 
{
  return p_base_name(DumpData) +
    _({
      HW_D_VALUE(containerData),
      HW_D_VALUE(viewIndex)
    });
};

FunctionCallResultCache::FunctionCallResultCache(const FunctionCallContext& context, size_t bodyIndex) :
  context(context)
  , bodyIndex(bodyIndex)
{
  SetDumpString();
}

ResultData FunctionCallResultCache::GetResultDataRecursive(const Category& category) const
{
  return GetResultData(category);
}

ResultData FunctionCallResultCache::GetResultData(Category const&category) const
{
  HW_ASSERT_IS(category, !=, Category::None);
  return ResultData::GetSmartHollowSize(category, l_(codeGet), l_(valueType), l_(extsGet));
}

p_implementation(FunctionCallResultCache, size_t, codeIndex) { return context.global->FunctionIndex(*this); };

p_implementation(FunctionCallResultCache, FunctionSyntax const&, body)
{
  return dynamic_cast<const FunctionSyntax&>(*context.container.containerData->statements[bodyIndex]);
};

p_implementation(FunctionCallResultCache, Ref<CodeItem>, codeGet)
{
  auto Trace = false; //context.ObjectId == 7 && bodyIndex == 0; 
  HW_D_METHOD_;
  HW_ASSERT(!arg.IsEmpty, "NotImpl: no arg " + Dump);
  HW_ASSERT(!body.getter.IsEmpty, "NotImpl: no function getter " + Dump);
  function.GetterIsUsed();
  auto rawResult
    = body
      .getter
      .Value
      ->GetResultCache(context)
      ->Get(Category::Type | Category::Closure);
  HW_D_LOG_VALUE(rawResult);
  auto result = rawResult
    .Convert(*valueType);
  if(result.closure.Value == Closure())
    return_d(CodeItem::CallGetter(valueType->size, codeIndex));

  if(result.closure.Value == External::Function::Arg::Instance)
    return_d(CodeItem::CallGetter(valueType->size, codeIndex, *arg.Value));

  HW_D_METHOD(result);
  HW_BREAK_AND_THROW;
}

p_implementation(FunctionCallResultCache, Closure, extsGet)
{
  return body
         .getter
         .Value
         ->GetResultCache(context)
         ->closure
    - External::Function::Arg::Instance;
}

p_implementation(FunctionCallResultCache, CodeFunction, getter)
{
  HW_ASSERT(!arg.IsEmpty, "NotImpl: no arg " + Dump);
  HW_ASSERT(!body.getter.IsEmpty, "NotImpl: no function getter " + Dump);
  auto rawResult = body
                   .getter
                   .Value
                   ->GetResultCache(context)
                   ->Get(Category::Type | Category::Code | Category::Closure)
                   .Convert(*valueType);

  if(rawResult.closure.Value == Closure())
    return CodeFunction::Getter(codeIndex, rawResult.code.Value);

  if(rawResult.closure.Value == External::Function::Arg::Instance)
  {
    ReplaceVisitor visitor;
    Ref<ResultCache> functionArg = new ResultDataDirect(CodeItem::FunctionArg(*arg.Value), arg.Value->IndirectType(1));
    visitor.SetResults(External::Function::Arg::Instance, *functionArg);
    auto result = rawResult.Replace(visitor);
    return CodeFunction::Getter(codeIndex, result.code.Value);
  }
  HW_D_METHOD(rawResult);
  HW_BREAK_AND_THROW;
}

p_implementation(FunctionCallResultCache, CodeFunction, setter)
{
  HW_D_METHOD_;
  HW_BREAK_AND_THROW;
}

p_implementation(FunctionCallResultCache, WeakRef<Type>, valueType)
{
  return body
         .getter
         .Value
         ->GetType(context)
         ->asFunctionResult;
}


p_implementation(RecursionContext, WeakRef<RecursionContext>, recursionContext)
{
  HW_D_METHOD_;
  HW_BREAK_AND_THROW;
}

const WeakRef<Type> RecursionContext::FunctionType(const FunctionSyntax& body) const
{
  HW_D_METHOD(body);
  HW_BREAK_AND_THROW;
}

const Optional<WeakRef<Type>> RecursionContext::CachedType(const Syntax& target) const
{
  return target.CachedType(parent);
}

const WeakRef<ContainerContext> RecursionContext::Container(const SyntaxContainer& statements, size_t viewIndex) const
{
  return parent.Container(statements, viewIndex);
}

const SearchResult<AccessFeature> RecursionContext::DeclarationsForType(const DefineableToken& token) const
{
  return parent.DeclarationsForType(token);
}

const SearchResult<AccessFeature> ChildContext::DeclarationsForType(const DefineableToken& token) const
{
  return parent.DeclarationsForType(token);
}


FunctionBodyType::FunctionBodyType(const Context& context, const FunctionSyntax& body) : context(context)
  , body(body.thisRef)
{}


p_implementation(FunctionBodyType, WeakRef<Global>, global) { return context.global; }

p_implementation(ContainerType, Size, size) { return parent.dataSize; }
p_implementation(ContainerType, WeakRef<Global>, global) { return parent.global; }

string External::Context::internalDump() const
{
  return target.DumpShort;
}

const Closure External::Context::Replace(const ReplaceVisitor& arg) const
{
  return arg.GetClosure(*this);
}
