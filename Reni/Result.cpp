#include "Import.h"
#include "Result.h"

#include "ArgVisitor.h"
#include "Code.h"
#include "Context.h"
#include "RecursionContext.h"
#include "Syntax.h"
#include "..\HWLib\LevelValue.h"
#include "..\HWLib\DumpMacros.h"
#include "..\HWLib\BreakHandling.h"

static bool Trace = true;

using namespace HWLib;
using namespace Reni;


class ResultDataDirect final : public ResultCache
{
    typedef ResultCache baseType;
    typedef ResultFromSyntaxAndContext thisType;
    WeakPtr<Type> const type;
    Ref<CodeItem, true> const code;
public:

    ResultDataDirect(Ref<CodeItem, true> code, WeakPtr<Type> type) : type(type), code(code)
    {
        SetDumpString();
        a_is(type->size, == , code->size);
    }

private:
    p_function(Array<String>, DumpData) override
    {
        return{
            nd(code),
            nd(type)
        };
    };

    ResultData const GetResultData(Category category)const override
    {
        return ResultData::Get(category, *code, *type);
    }
};


ResultData const ResultCache::Get(Category category) const
{
    Ensure(category);
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
        a_if(todo == Category::Type, nd(category) + nd(complete) + nd(pending));
        a_if(pending == Category::Type, nd(category) + nd(complete) + nd(pending));
    }

    data = data + GetResultData(newTodo);

    a_if(isRecursion || category <= complete, nd(category) + nd(complete) + nd(pending));
    thisRef.SetDumpString();
}

pure_p_implementation(ResultCache, bool, isRecursion) ;

p_implementation(ResultCache, Category, complete)
{
    return data.complete;
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

p_implementation(ResultCache, Externals, externals)
{
    Ensure(Category::Externals);
    return data.externals;
}

p_implementation(ResultCache, WeakPtr<Type>, cachedType)
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
    bool Trace = syntax.ObjectId == -11;
    md(category)  ;
    b_if_(Trace);
    auto result = syntax.GetResultData(context,category);
    return_d(result);
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
    return ResultData(
        size || other.size, 
        code || other.code, 
        type || other.type,
        externals || other.externals
        );
}

ResultData const ResultData::operator&(Category const& other) const
{
    return ResultData(
        other.hasSize ? size : Optional<Size>(),
        other.hasCode ? code : Ref<CodeItem, true>(),
        other.hasType ? type : WeakPtr<Type>(),
        other.hasExternals ? externals : Optional<Externals>()
        );
}

ResultData::ResultData(Ref<CodeItem> code)
    : size(code->size)
      , code(code)
{
    SetDumpString();
    AssertValid();
}

ResultData ResultData::Get(Category category, function<Ref<CodeItem>()> getCode, function<WeakRef<Type>()> getType)
{
    auto code = category.hasCode ? Ref<CodeItem, true>(getCode()) : Ref<CodeItem, true>();
    auto type = category.hasType ? WeakPtr<Type>(getType()) : WeakPtr<Type>();
    Optional<Size> size;
    if(category.hasSize)
    {
        if(category.hasCode)
            size = code->size;
        else if(category.hasType)
            size = type->size;
        else
            a_fail(category.Dump);
    }
    Optional<Externals> externals;
    if(category.hasExternals)
    {
        if(category.hasCode)
            externals = code->externals;
        else
            a_fail(category.Dump);
    }

    return Get(category, size, code, type, externals);
}

ResultData ResultData::Get(Category category, CodeItem const& code, Type const& type)
{
    Optional<Size> size;
    if(category.hasSize)
        size = code.size;
    Optional<Externals> externals;
    if(category.hasExternals)
        externals = code.externals;

    return Get(category, size, &code.thisRef, &type.thisRef, externals) ;
}

ResultData ResultData::Get(Category category, function<Ref<CodeItem>()> getCode, Type const& type)
{
    auto code = category.hasCode ? Ref<CodeItem, true>(getCode()) : Ref<CodeItem, true>();
    Optional<Size> size;
    if(category.hasSize)
            size = type.size;

    Optional<Externals> externals;

    if(category.hasExternals)
    {
        if(category.hasCode)
            externals = code->externals;
        else
            a_fail(category.Dump);
    }

    return Get(category, size, code, &type.thisRef, externals) ;
}

ResultData ResultData::Get(Category category, CodeItem const& code, function<WeakRef<Type>()> getType)
{
    auto type = category.hasType ? WeakPtr<Type>(getType()) : WeakPtr<Type>();
    Optional<Size> size;
    if(category.hasSize)
        size = code.size;
    Optional<Externals> externals;
    if(category.hasExternals)
        externals = code.externals;

    return Get(category, size, &code.thisRef, type, externals) ;
}

ResultData const ResultData::With(Type const& type) const
{return ResultData(size, code, type.thisRef, {});}

ResultData::ResultData(Type const& type)
    : size(type.size)
      , type(type.thisRef)
{
    SetDumpString();
    AssertValid();
}

ResultData const ResultData::Replace(ReplaceVisitor const& arg) const
{
    if(!complete.hasCode)
        return *this;
    bool Trace = arg.Trace;
    md(arg)  ;
    auto newCode = code->Replace(arg);
    if(newCode.IsEmpty)
        return_d(*this);
    return_d(Get(complete, *newCode, *type));
}

ResultData const ResultData::With(CodeItem const& code) const
{
    return Get(complete | Category::Code, code, *type);
}

p_implementation(ResultData, Array<String>, DumpData)
{
    return{
        nd(size),
        nd(type),
        nd(code),
        nd(externals)
    };
}

void ResultData::AssertValid()
{
    if(complete.hasSize)
    {
        if(complete.hasCode)
        a_is(code->size, == , size);
        if(complete.hasType)
        a_is(type->size, == , size);
    }
    else if(complete.hasCode && complete.hasType)
        a_is(code->size, == , type->size);
    if(complete.hasCode && complete.hasExternals)
        a_is(code->externals, == , externals);
};