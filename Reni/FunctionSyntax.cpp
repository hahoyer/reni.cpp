#include "Import.h"
#include "FunctionSyntax.h"

#include "Type.h"

using namespace Reni;


p_implementation(FunctionSyntax, String, SmartDump) 
{
    String result;
    if(!setter.IsEmpty)
        result += setter->SmartDumpFrame(priority);
    result += " /\\ ";
    if(!getter.IsEmpty)
        result += getter->SmartDumpFrame(priority);
    return result;
}


ResultData const FunctionSyntax::GetResultData(Context const& context, Category category) const
{
    return context.FunctionType(*this)->GetResultData();
}