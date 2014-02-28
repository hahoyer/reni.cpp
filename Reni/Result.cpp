#include "Import.h"
#include "Result.h"

#include "Category.h"
#include "Code.h"
#include "Context.h"
#include "Syntax.h"
#include "..\HWLib\LevelValue.h"
#include "..\HWLib\DumpMacros.h"
#include "..\HWLib\BreakHandling.h"

static bool Trace = true;

using namespace HWLib;
using namespace Reni;

Result::Result(Syntax const& syntax, Context const&context)
    : baseType()
      , syntax(syntax)
      , context(context){
}

void Result::Ensure(Category category)const{
    auto todo = category - complete;
    if (todo == Category::None)
        return;
    auto newTodo = todo - pending;
    a_if(newTodo != Category::None, nd(category) + nd(complete) + nd(pending));
    LevelValue<Category>(pending, pending + newTodo);
    data = GetResultData(newTodo);
}

ResultData const Result::GetResultData(Category category)const{
    return context.GetResultData(category, syntax);
}

p_implementation(Result, Category, complete){
    if (data.code.IsValid)
        return Category::Code;
    return Category::None;
}


p_implementation(Result, Ref<CodeItem>, code){
    Ensure(Category::Code);
    return data.code;
}

p_implementation(Result, Ref<Type>, type){
    Ensure(Category::Type);
    return data.type;
}

override_p_implementation(Result, Array<String>, DumpData){
    return{
        nd(context),
        nd(syntax),
        nd(pending),
        nd(data)
    };
};

override_p_implementation(ResultData, Array<String>, DumpData){
    return{
        nd(code)
    };
};