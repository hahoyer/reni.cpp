#include "Import.h"
#include "Syntax.h"

#include "Category.h"
#include "Code.h"
#include "Context.h"
#include "Result.h"
#include "TokenClass.h"

using namespace Reni;
static bool Trace = true;

static int nextObjectId = 0;


Syntax::Syntax(SourcePart const&part)
: baseType(nextObjectId++)
, part(part)
, resultCache([&](Context const*context){return Result(*this, *context); }){};


Ref<CodeItem> Syntax::Code(Ref<Context> const&context)const{
    return GetResult(Category::Code, context).Code;
}


Result Syntax::GetResult(Category category, Ref<Context> const&context)const{
    return resultCache[&*context];


};


override_p_implementation(InfixSyntax, Array<String>, DumpData){
    return {
        nd(left),
            nd(tokenClass),
            nd(right)
    };
};

override_p_implementation(PrefixSyntax, Array<String>, DumpData){
    return {
            nd(tokenClass),
            nd(right)
    };
};

override_p_implementation(SuffixSyntax, Array<String>, DumpData){
    return {
        nd(left),
            nd(tokenClass)
    };
};

override_p_implementation(TerminalSyntax, Array<String>, DumpData){
    return {
            nd(tokenClass)
    };
};
