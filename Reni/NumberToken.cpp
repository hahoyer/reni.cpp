#include "Import.h"
#include "NumberToken.h"

#include "BitType.h"
#include "CodeItems.h"
#include "Context.h"
#include "ContextIncludes.h"
#include "Global.h"
#include "NumberType.h"
#include "../Util/BitsConst.h"

using namespace Reni;
static bool Trace = true;


Ref<Syntax> const NumberToken::Create(SourcePart const&part)const{
    return new TerminalSyntax<NumberToken>(*this, part);
};

ResultData const NumberToken::GetResultData(Context const&context, Category category, SourcePart const&part)const{
    auto value = BitsConst::Convert(part);
    auto size = value.size.value;
    return context
        .global
        ->bitType
        .array(value.size.value)
        ->numberType
        ->GetResultData(category,l_(CodeItem::Const(value)),l_(Externals()));
}
