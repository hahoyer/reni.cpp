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

ResultData const NumberToken::GetResultData(Context const&context, Category category, SourcePart const&part)const {
    return context
        .rootContext
        .bitType
        .GetResultData(category, BitsConst::Convert(part));
}
