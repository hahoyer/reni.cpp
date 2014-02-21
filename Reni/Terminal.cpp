#include "Import.h"
#include "Terminal.h"

#include "Context.h"

using namespace Reni;
static bool Trace = true;


ResultData const TerminalTokenClass::GetResultData(Context const&context, Category category, SourcePart const&part)const {
    md(context, category, part);
    b_;
    return_d(ResultData());
}


ResultData const TerminalSyntax::GetResultData(Context const&context, Category category)const{
    return tokenClass.GetResultData(context, category, part);
};

override_p_implementation(TerminalSyntax, Array<String>, DumpData){
    return {
            nd(tokenClass)
    };
};

