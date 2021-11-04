#include "Import.h"
#include "Result.h"

#include "CodeItem.h"
#include "Context.h"
#include "ContainerContext.h"
#include "Closure.h"
#include "RecursionContext.h"
#include "ReplaceVisitor.h"
#include "Syntax.h"

#include "../HWLib/RefCountContainer.instance.h"
#include "../HWLib/LevelValue.h"

static bool Trace = true;

using namespace HWLib;
using namespace Reni;


ResultCache::ResultCache()
    : Trace(false)
{
}

ResultData ResultCache::Get(const Category& category) const
{
    Ensure(category);
    const auto pendingCategory = category - data.complete;
    if (pendingCategory == Category::None)
        return data;

    const auto recursive = GetResultDataRecursive(pendingCategory);
    a_if(recursive.IsConsistent(data), nd(thisRef) + nd(recursive) + nd(pendingCategory));
    return recursive | data;
}

void ResultCache::Ensure(const Category& category)const
{
    auto todo = category - complete;
    if(todo == Category::None)
        return;
    bool Trace = this->Trace && category.hasClosure;
    md(category);
    auto newTodo = todo - pending;
    if (newTodo != Category::None)
    {
        LevelValue<Category> localPending(pending, pending | newTodo);

        auto oldData = data;
        d(newTodo);
        b_if_(Trace);
        auto newResult = GetResultData(newTodo);
        d(newResult);
        b_if_(Trace);
        a_if(isRecursion || data.IsConsistent(newResult), nd(thisRef) + nd(newResult));
        data = newResult| data;
        a_if(isRecursion || category <= complete, nd(category) + nd(complete) + nd(pending));
    }

    pending -= complete;
    thisRef.SetDumpString();
    dumpreturn(data);
}

p_virtual_header_implementation(ResultCache, bool, isRecursion) ;

p_implementation(ResultCache, Category, complete){ return data.complete; };
p_implementation(ResultCache, bool, hollow){ return Get(Category::Hollow).hollow; };
p_implementation(ResultCache, Size, size){ return Get(Category::Size).size; };
p_implementation(ResultCache, Ref<CodeItem>, code){ return Get(Category::Code).code; };
p_implementation(ResultCache, WeakRef<Type>, type){ return Get(Category::Type).type; };
p_implementation(ResultCache, Closure, closure){ return Get(Category::Closure).closure; };

p_implementation(ResultCache, Optional<WeakRef<Type>>, cachedType){return data.type;}

p_implementation(ResultCache, Array<string>,DumpData)
{
    return{
        nd(pending),
        nd(data)
    };
};

ResultData ResultCache::GetResultDataRecursive(Category const& category) const
{
    bool Trace = true;
    md(category);
    mb;
}


ResultFromSyntaxAndContext::ResultFromSyntaxAndContext(Syntax const& syntax, Context const&context)
    : syntax(syntax)
      , context(context)
{
    Trace = context.ObjectId == -4 && syntax.ObjectId == 27;
    SetDumpString();
}

ResultData ResultFromSyntaxAndContext::GetResultData(Category const&category) const
{
    a_if_(category != Category::None || context.isRecursion);
    bool Trace = false;// this->Trace && category.hasClosure;
    md(category);
    b_if_(Trace);
    auto result = syntax.GetResultData(context,category);
    a_is(category, <= , result.complete);
    return_db(result);
}

ResultData ResultFromSyntaxAndContext::GetResultDataRecursive(Category const& category) const
{
    if (!isRecursion)
        return syntax.GetResultCache(*context.recursionContext)->Get(category);
    
    if (category == Category::Closure)
        return Closure();
    auto Trace = true;
    md(category);
    mb;
}

p_implementation(ResultFromSyntaxAndContext, Array<string>,DumpData)
{
  const auto baseDump = p_base_name(DumpData);
    const auto thisDump = Array<string>({
        nd(context),
        nd(syntax)
    });
    return baseDump + thisDump;
};

p_implementation(ResultFromSyntaxAndContext, bool, isRecursion)
{
    return !!dynamic_cast<RecursionContext const*>(&context);
}

ResultData ResultData::operator|(ResultData const& other) const
{
    return ResultData(
        hollow || other.hollow,
        size || other.size,
        code || other.code,
        type || other.type,
        closure || other.closure
        );
}

bool ResultData::IsConsistent(ResultData const& other) const
{
    return (*this & other.complete) == (other & complete);
}

ResultData ResultData::operator&(Category const& other) const
{
    return ResultData(
        other.hasHollow ? hollow : Optional<bool>(),
        other.hasSize ? size : Optional<Size>(),
        other.hasCode ? code : Optional<Ref<CodeItem>>(),
        other.hasType ? type : Optional<WeakRef<Type>>(),
        other.hasClosure ? closure : Optional<Closure>()
        );
}

ResultData::ResultData(Ref<CodeItem> code)
    : size(code->size)
      , code(code)
{
    SetDumpString();
    AssertValid();
}

bool ResultData::operator==(ResultData const& other) const
{
    return hollow == other.hollow
        && size == other.size
        && type == other.type
        && code == other.code
        && closure == other.closure;
}

Optional<bool> ResultData::ReplenishHollow(Category const& category, function<Ref<CodeItem>()> getCode,
                                         function<WeakRef<Type>()> getType)
{
    if(!category.hasHollow)
        return{};

    if(category.hasCode)
        return getCode()->size == 0;

    if(category.hasType)
        return getType()->size == 0;
    
    a_fail(category.Dump);
    return{};
}

Optional<Size> ResultData::ReplenishSize(Category const& category, function<Ref<CodeItem>()> getCode,
                                         function<WeakRef<Type>()> getType)
{
    if(category.hasSize)
    {
        if(category.hasCode)
            return getCode()->size;
        if(category.hasType)
            return getType()->size;
        a_fail(category.Dump);
    }

    return {};
}

ResultData ResultData::Convert(Type const& destination) const
{
    if(complete == Category::None)
        return *this;
    if(destination == *type.Value)
        return *this;
    md(destination);
    mb;
}

Optional<Closure> ResultData::ReplenishExternals(Category const& category, function<Ref<CodeItem>()> getCode)
{
    if(category.hasClosure)
        return getCode()->closure;
    return {};
}

ResultData ResultData::Get(
  const Category& category,
  function<bool()> const& getHollow,
  function<Size()> getSize,
  function<Ref<CodeItem>()> getCode,
  function<WeakRef<Type>()> getType,
  function<Closure()> getClosure
)
{
  const auto hollow = category.hasHollow? Optional<bool>(getHollow()) : Optional<bool>();
  const auto size = category.hasSize ? Optional<Size>(getSize()) : Optional<Size>();
  const auto code = category.hasCode ? Optional<Ref<CodeItem>>(getCode()) : Optional<Ref<CodeItem>>();
  const auto type = category.hasType ? Optional<WeakRef<Type>>(getType()) : Optional<WeakRef<Type>>();
  const auto closure = category.hasClosure ? Optional<Closure>(getClosure()) : Optional<Closure>();
    return FullGet(category, hollow, size, code, type, closure);
}

ResultData ResultData::GetSmartHollowSize(
  const Category& category,
  function<Ref<CodeItem>()> getCode,
  function<WeakRef<Type>()> getType,
  function<Closure()> getClosure
)
{
    auto code = category.hasCode ? Optional<Ref<CodeItem>>(getCode()) : Optional<Ref<CodeItem>>();
    auto type = category.hasType ? Optional<WeakRef<Type>>(getType()) : Optional<WeakRef<Type>>();
    const auto closure = category.hasClosure ? Optional<Closure>(getClosure()) : Optional<Closure>();
    const auto size = ReplenishSize(category, l_(code), l_(type));
    const auto hollow = ReplenishHollow(category, l_(code), l_(type));
    return FullGet(category, hollow, size, code, type, closure);
}

ResultData ResultData::GetSmartHollowClosure(
  const Category& category,
  function<Size()> getSize,
  function<Ref<CodeItem>()> getCode,
  function<WeakRef<Type>()> getType
)
{
    return Get(category, l_(ReplenishHollow(category, getCode, getType)), getSize, getCode, getType, l_(getCode()->closure));
}

ResultData ResultData::GetSmartHollowSizeClosure(
  const Category& category,
  function<Ref<CodeItem>()> getCode,
  function<WeakRef<Type>()> getType
)
{
    return GetSmartHollowSize(category, getCode, getType, l_(getCode()->closure));
}

ResultData::ResultData(Type const& type)
    : size(type.size)
      , type(type.thisRef)
{
    SetDumpString();
    AssertValid();
}

ResultData::ResultData(const Closure &closure)
    : closure(closure)
{
    SetDumpString();
    AssertValid();
}

ResultData ResultData::Replace(ReplaceVisitor const& arg) const
{
    return Get(
        complete, 
        l_(hollow),
        l_(size),
        l_(code.Value->Replace(arg) || code), 
        l_(type), 
        l_(closure.Value.Replace(arg) || closure));
}

ResultData ResultData::Replace(External const& tag, ResultCache const& result) const
{
    auto visitor = ReplaceVisitor(tag, result);
    visitor.Trace = result.Trace;
    return Replace(visitor);
}

p_implementation(ResultData, Array<string>,DumpData)
{
    return{
        nd(hollow),
        nd(size),
        nd(type),
        nd(code),
        nd(closure)
    };
}

p_implementation(ResultData, ResultData, asFunctionResult)
{
    return GetSmartHollowSize
        (
        complete, 
        l_(code.Value),
        l_(type.Value->asFunctionResult),
        l_(closure.Value)
        );
}

void ResultData::AssertValid() const
{
    if (complete.hasHollow)
    {
        if (complete.hasSize)
            a_if(hollow.Value == (size.Value == 0), nd(hollow) + nd(size));
        if (complete.hasCode)
            a_if(hollow.Value == (code.Value->size == 0), nd(hollow) + nd(code));
        if (complete.hasType)
            a_if(hollow.Value == (type.Value->size == 0), nd(hollow) + nd(type));
    }

    if(complete.hasSize)
    {
        if(complete.hasCode)
            a_is(code.Value->size, == , size.Value);
        if(complete.hasType)
            a_is(type.Value->size, == , size.Value);
    }
    else if(complete.hasCode && complete.hasType)
        a_is(code.Value->size, == , type.Value->size);
    if(complete.hasCode && complete.hasClosure)
        a_is(code.Value->closure, == , closure.Value);
}

void ResultData::AssertValid(const Category& category, Optional<bool> const& hollow, Optional<Size> const size, Optional<Ref<CodeItem>> code, Optional<WeakRef<Type>> type, Optional<Closure> const& closure)
{
    if(category.hasHollow)
        a_if(hollow.IsValid, nd(category) + nd(hollow));
    if(category.hasSize)
        a_if(size.IsValid, nd(category) + nd(size));
    if(category.hasCode)
        a_if(code.IsValid, nd(category) + nd(code));
    if(category.hasType)
        a_if(type.IsValid, nd(category) + nd(type));
    if(category.hasClosure)
        a_if(closure.IsValid, nd(category) + nd(closure));
}

