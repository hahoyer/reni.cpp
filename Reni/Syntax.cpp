#include "Import.h"
#include "Syntax.h"
#include "TokenClass.h"

using namespace Reni;
static bool Trace = true;

static int nextObjectId = 0;

Syntax::Syntax(SourcePart const&part)
: baseType(nextObjectId++)
, part(part)
{}


Ref<CodeItem> Syntax::Code(Ref<Context> const&context)const{
    return Result(Category::Code, context).Code;
}


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
