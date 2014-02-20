#include "Import.h"
#include "Result.h"

#include "Code.h"
#include "Context.h"
#include "Syntax.h"

static bool Trace = true;

using namespace Reni;

static int nextObjectId = 0;

Category::Category(bool hasSize, bool hasCode)
: hasSize(hasSize)
, hasCode(hasCode)
{}

override_p_implementation(Category, Array<String>, DumpData){

    auto result = 
        _({ 
        hasSize ? String("Size") : "",
        hasCode ? String("Code") : ""
    })
    .Where([](String element){return element != ""; })
        ->ToArray;
    if (result.Count)
        return result;
    return{ "<none>" };
};


class Result::internal{
    Result const&parent;
public:
    internal(Result const&parent) : parent(parent){}
    void Ensure(Category category)const;

    OptRef<CodeItem> code;
};


    
Result::Result(Syntax const& syntax, Context const&context)
: baseType(nextObjectId++)
, syntax(syntax)
, context(context)
, cache(new internal(*this))
{}


override_p_implementation(Result, Array<String>, DumpData){
    return{
        nd(context),
        nd(syntax)
    };
};

p_implementation(Result, Ref<CodeItem>, Code){
    cache->Ensure(Category::Code);
    return cache->code;
}
