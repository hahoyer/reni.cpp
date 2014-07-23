#include "Import.h"
#include "Result.h"

#include "CodeItem.h"
#include "Context.h"
#include "ContainerContext.h"
#include "Externals.h"
#include "RecursionContext.h"
#include "ReplaceVisitor.h"
#include "Syntax.h"
#include "SyntaxContainer.h"

#include "../HWLib/RefCountContainer.instance.h"
#include "../HWLib/LevelValue.h"

static bool Trace = true;

using namespace HWLib;
using namespace Reni;


ResultData const ResultCache::Get(Category category) const
{
    Ensure(category);
    if(category.hasExts && !data.complete.hasExts)
        return data + Externals();

    return data;
}

void ResultCache::Ensure(Category category)const
{
    auto todo = category - complete;
    if(todo == Category::None)
        return;
    auto newTodo = todo - pending;
    LevelValue<Category> localPending(pending, pending | newTodo);

    if(newTodo == Category::None)
    {
        a_if(todo.hasType || todo.hasExts, nd(category) + nd(complete) + nd(pending));
        a_if(pending == todo, nd(category) + nd(complete) + nd(pending));
    }

    auto newResult = GetResultData(newTodo);
    data = data + newResult;

    a_if(isRecursion || category <= complete, nd(category) + nd(complete) + nd(pending));
    thisRef.SetDumpString();
}

p_virtual_header_implementation(ResultCache, bool, isRecursion) ;

p_implementation(ResultCache, Category, complete)
{
    return data.complete;
}

bool const ResultCache::get_hllw() const
{
    Ensure(Category::Hllw);
    return data.hllw;
}

p_implementation(ResultCache, Size, size)
{
    Ensure(Category::Size);
    return data.size;
}

p_implementation(ResultCache, Ref<CodeItem>, code)
{
    Ensure(Category::Code);
    return data.code;
}

p_implementation(ResultCache, WeakRef<Type>, type)
{
    Ensure(Category::Type);
    return data.type;
}

p_implementation(ResultCache, Externals, exts)
{
    Ensure(Category::Exts);
    return data.exts;
}

p_implementation(ResultCache, Optional<WeakRef<Type>>, cachedType)
{
    return data.type;
}

p_implementation(ResultCache, Array<String>, DumpData)
{
    return{
        nd(pending),
        nd(data)
    };
};


ResultFromSyntaxAndContext::ResultFromSyntaxAndContext(Syntax const& syntax, Context const&context)
    : syntax(syntax)
      , context(context)
{
    SetDumpString();
}

ResultData const ResultFromSyntaxAndContext::GetResultData(Category category)const
{
    a_if_(category != Category::None || context.isRecursion);
    bool Trace = context.ObjectId == -6 && syntax.ObjectId == 15 && category.hasCode;
    md(category);
    b_if_(Trace);
    auto result = syntax.GetResultData(context,category);
    a_is(category, <= , result.complete);
    return_db(result);
}

p_implementation(ResultFromSyntaxAndContext, Array<String>, DumpData)
{
    auto baseDump = base_p_name(DumpData);
    auto thisDump = Array<String>({
        nd(context),
        nd(syntax)
    });
    return baseDump + thisDump;
};

p_implementation(ResultFromSyntaxAndContext, bool, isRecursion)
{
    return !!dynamic_cast<RecursionContext const*>(&context);
}

ResultData const ResultData::operator+(ResultData const& other) const
{
    a_if((*this & other.complete) == (other & complete), 
        DumpList({nd((complete & other.complete)), nd(*this), nd(other)}));

    return ResultData(
        hllw || other.hllw,
        size || other.size,
        code || other.code, 
        type || other.type,
        exts || other.exts
        );
}

ResultData const ResultData::operator&(Category const& other) const
{
    return ResultData(
        other.hasHllw ? hllw : Optional<bool>(),
        other.hasSize ? size : Optional<Size>(),
        other.hasCode ? code : Optional<Ref<CodeItem>>(),
        other.hasType ? type : Optional<WeakRef<Type>>(),
        other.hasExts ? exts : Optional<Externals>()
        );
}

ResultData::ResultData(Ref<CodeItem> code)
    : size(code->size)
      , code(code)
{
    SetDumpString();
    AssertValid();
}

bool const ResultData::operator==(ResultData const& other) const
{
    return hllw == other.hllw
        && size == other.size
        && type == other.type
        && code == other.code
        && exts == other.exts;
}

Optional<bool> const ResultData::ReplenishHllw(Category const& category, function<Ref<CodeItem>()> getCode, function<WeakRef<Type>()> getType)
{
    if(!category.hasHllw)
        return{};

    if(category.hasCode)
        return getCode()->size == 0;

    if(category.hasType)
        return getType()->size == 0;
    
    a_fail(category.Dump);
    return{};
}

Optional<Size> const ResultData::ReplenishSize(Category const& category, function<Ref<CodeItem>()> getCode, function<WeakRef<Type>()> getType)
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

ResultData const ResultData::Convert(Type const& destination) const
{
    if(complete == Category::None)
        return *this;
    if(destination == *type.Value)
        return *this;
    md(destination);
    mb;
}

Optional<Externals> const ResultData::ReplenishExternals(Category const& category, function<Ref<CodeItem>()> getCode)
{
    if(category.hasExts)
        return getCode()->exts;
    return {};
}

ResultData const ResultData::Get(
    Category category, 
    function<bool()> const&getHllw,
    function<Size()> getSize,
    function<Ref<CodeItem>()> getCode,
    function<WeakRef<Type>()> getType, 
    function<Externals()> getExts
    )
{
    auto hllw = category.hasHllw? Optional<bool>(getHllw()) : Optional<bool>();
    auto size = category.hasSize ? Optional<Size>(getSize()) : Optional<Size>();
    auto code = category.hasCode ? Optional<Ref<CodeItem>>(getCode()) : Optional<Ref<CodeItem>>();
    auto type = category.hasType ? Optional<WeakRef<Type>>(getType()) : Optional<WeakRef<Type>>();
    auto exts = category.hasExts ? Optional<Externals>(getExts()) : Optional<Externals>();
    return FullGet(category, hllw, size, code, type, exts);
}

ResultData const ResultData::GetSmartHllwSize(
    Category category,
    function<Ref<CodeItem>()> getCode,
    function<WeakRef<Type>()> getType,
    function<Externals()> getExts
    )
{
    auto code = category.hasCode ? Optional<Ref<CodeItem>>(getCode()) : Optional<Ref<CodeItem>>();
    auto type = category.hasType ? Optional<WeakRef<Type>>(getType()) : Optional<WeakRef<Type>>();
    auto exts = category.hasExts ? Optional<Externals>(getExts()) : Optional<Externals>();
    auto size = ReplenishSize(category, l_(code), l_(type));
    auto hllw = ReplenishHllw(category, l_(code), l_(type));
    return FullGet(category, hllw, size, code, type, exts);
}

ResultData const ResultData::GetSmartHllwExts(
    Category category,
    function<Size()> getSize,
    function<Ref<CodeItem>()> getCode,
    function<WeakRef<Type>()> getType
    )
{
    return Get(category, l_(ReplenishHllw(category, getCode, getType)), getSize, getCode, getType, l_(getCode()->exts));
}

ResultData const ResultData::GetSmartHllwSizeExts(
    Category category,
    function<Ref<CodeItem>()> getCode,
    function<WeakRef<Type>()> getType
    )
{
    return GetSmartHllwSize(category, getCode, getType, l_(getCode()->exts));
}

ResultData::ResultData(Type const& type)
    : size(type.size)
      , type(type.thisRef)
{
    SetDumpString();
    AssertValid();
}

ResultData::ResultData(Externals exts)
    : exts(exts)
{
    SetDumpString();
    AssertValid();
}

ResultData const ResultData::Replace(ReplaceVisitor const& arg) const
{
    return Get(
        complete, 
        l_(hllw),
        l_(size),
        l_(code.Value->Replace(arg) || code), 
        l_(type), 
        l_(exts.Value.Replace(arg) || exts));
}

ResultData const ResultData::Replace(External const& tag, ResultCache const& result) const
{
    return Replace(ReplaceVisitor(tag, result));
}

p_implementation(ResultData, Array<String>, DumpData)
{
    return{
        nd(hllw),
        nd(size),
        nd(type),
        nd(code),
        nd(exts)
    };
}

p_implementation(ResultData, ResultData, asFunctionResult)
{
    return GetSmartHllwSize
        (
        complete, 
        l_(code.Value),
        l_(type.Value->asFunctionResult),
        l_(exts.Value)
        );
}

void ResultData::AssertValid()
{
    if (complete.hasHllw)
    {
        if (complete.hasSize)
            a_if(hllw.Value == (size.Value == 0), nd(hllw) + nd(size));
        if (complete.hasCode)
            a_if(hllw.Value == (code.Value->size == 0), nd(hllw) + nd(code));
        if (complete.hasType)
            a_if(hllw.Value == (type.Value->size == 0), nd(hllw) + nd(type));
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
    if(complete.hasCode && complete.hasExts)
        a_is(code.Value->exts, == , exts.Value);
}

void ResultData::AssertValid(Category category, Optional<bool> const& hllw, Optional<Size> const size, Optional<Ref<CodeItem>> code, Optional<WeakRef<Type>> type, Optional<Externals> const& exts)
{
    if(category.hasHllw)
        a_if(hllw.IsValid, nd(category) + nd(hllw));
    if(category.hasSize)
        a_if(size.IsValid, nd(category) + nd(size));
    if(category.hasCode)
        a_if(code.IsValid, nd(category) + nd(code));
    if(category.hasType)
        a_if(type.IsValid, nd(category) + nd(type));
    if(category.hasExts)
        a_if(exts.IsValid, nd(category) + nd(exts));
}

