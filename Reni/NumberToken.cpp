#include "Import.h"
#include "NumberToken.h"

#include "BitsConst.h"
#include "BitType.h"
#include "RootContext.h"

using namespace Reni;
static bool Trace = true;


Ref<Syntax> const NumberToken::CreateSyntax(SourcePart const&part)const{
    return new TerminalSyntax<NumberToken>(*this, part);
};

ResultData const NumberToken::GetResultData(Context const&context, Category category, SourcePart const&part)const{
    auto value = BitsConst::Convert(part);
    return context
        .rootContext
        ->bitType
        ->array(value.size.value)
        ->numberType
        ->GetResultData(category, value);
}
