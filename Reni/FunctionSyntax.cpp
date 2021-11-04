#include "Import.h"
#include "FunctionSyntax.h"

#include "ContainerContext.h"
#include "SyntaxContainer.h"
#include "Type.h"

using namespace Reni;


p_implementation(FunctionSyntax, string, SmartDump) 
{
    string result;
    if(!setter.IsEmpty)
        result += setter.Value->SmartDumpFrame(priority);
    result += " /\\ ";
    if(!getter.IsEmpty)
        result += getter.Value->SmartDumpFrame(priority);
    return result;
}


ResultData FunctionSyntax::GetResultData(const Context& context, Category const& category) const
{
    return context.FunctionType(*this)->GetResultDataEmpty(category);
}