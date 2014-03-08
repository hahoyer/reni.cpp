#include "Import.h"
#include "NumberToken.h"

#include "BitsConst.h"
#include "BitType.h"
#include "RootContext.h"

using namespace Reni;
static bool Trace = true;


Ref<Syntax> const NumberToken::CreateSyntax(SourcePart const&part, bool isMatch)const{
    if (isMatch)
        return baseType::CreateSyntax(part, isMatch);
    return new TerminalSyntax(*this, part);
};

ResultData const NumberToken::GetResultData(Context const&context, Category category, SourcePart const&part)const{
    auto value = BitsConst::Convert(part);
    return context
        .rootContext
        ->bitType
        ->array(value.size.value)
        ->GetResultData(category, value);
}
