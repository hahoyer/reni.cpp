#include "Import.h"
#include "Result.h"

#include "ArgVisitor.h"
#include "Code.h"
#include "Context.h"
#include "Syntax.h"
#include "..\HWLib\LevelValue.h"
#include "..\HWLib\DumpMacros.h"
#include "..\HWLib\BreakHandling.h"

static bool Trace = true;

using namespace HWLib;
using namespace Reni;


class ResultDataDirect final : public ResultCache{
    typedef ResultCache baseType;
    typedef ResultFromSyntaxAndContext thisType;
    WeakPtr<Type> const type;
    Ref<CodeItem, true> const code;
public:

    ResultDataDirect(Ref<CodeItem, true> code, WeakPtr<Type> type) : type(type), code(code){
        SetDumpString();
        a_is(type->size, == , code->size);
    }

private:
    p_function(Array<String>, DumpData) override{
        return{
            nd(code),
            nd(type)
        };
    };

    ResultData const GetResultData(Category)const override{
        return ResultData(code->size, code, type);
    }
};


void ResultCache::Ensure(Category category)const{
    auto todo = category - complete;
    if(todo == Category::None)
        return;
    auto newTodo = todo - pending;
    a_if(newTodo != Category::None, nd(category) + nd(complete) + nd(pending));
    LevelValue<Category>(pending, pending + newTodo);
    data = GetResultData(newTodo);
}

p_implementation(ResultCache, Category, complete){
    if(!data.code.IsEmpty)
        return Category::Code;
    return Category::None;
}

p_implementation(ResultCache, Size, size){
    Ensure(Category::Size);
    return data.size;
}

p_implementation(ResultCache, Ref<CodeItem>, code){
    Ensure(Category::Code);
    return data.code;
}

p_implementation(ResultCache, WeakRef<Type>, type){
    Ensure(Category::Type);
    return data.type;
}

p_implementation(ResultCache, Array<String>, DumpData){
    return{
        nd(pending),
        nd(data)
    };
};


ResultFromSyntaxAndContext::ResultFromSyntaxAndContext(Syntax const& syntax, Context const&context)
    : syntax(syntax)
      , context(context){
    SetDumpString();
}

ResultData const ResultFromSyntaxAndContext::GetResultData(Category category)const{
    return context.GetResultData(category, syntax);
}

p_implementation(ResultFromSyntaxAndContext, Array<String>, DumpData){
    auto baseDump = base_p_name(DumpData);
    auto thisDump = Array<String>({
        nd(context),
        nd(syntax)
    });
    return baseDump + thisDump;
};

ResultData const ResultData::Replace(ReplaceVisitor const& arg) const{
    if(!complete.hasCode)
        return *this;
    auto newCode = code->Replace(arg);
    if(newCode.IsEmpty)
        return *this;
    return ResultData(size, newCode, type);
};

p_implementation(ResultData, Array<String>, DumpData){
    return{
        nd(size),
        nd(type),
        nd(code)
    };
};