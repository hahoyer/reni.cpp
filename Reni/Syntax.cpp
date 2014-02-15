#include "Import.h"
#include "Syntax.h"
#include "TokenClass.h"

using namespace Reni;
static bool Trace = true;

static int nextObjectId = 0;

Syntax::Syntax()
: baseType(nextObjectId++)
{}

p_implementation(InfixSyntax, Array<String>, DumpData){
    return List<String>{
        vardump(left),
            vardump(tokenClass),
            vardump(right)
    };
};

p_implementation(PrefixSyntax, Array<String>, DumpData){
    return List<String>{
            vardump(tokenClass),
            vardump(right)
    };
};

p_implementation(SuffixSyntax, Array<String>, DumpData){
    return List<String>{
        vardump(left),
            vardump(tokenClass)
    };
};

p_implementation(TerminalSyntax, Array<String>, DumpData){
    return List<String>{
            vardump(tokenClass),
                vardump(part)
    };
};
