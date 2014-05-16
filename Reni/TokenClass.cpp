#include "Import.h"
#include "TokenClass.h"

#include "Context.h"
#include "DefineableToken.h"
#include "DumpPrintToken.h"
#include "NumberType.h"
#include "Syntax.h"
#include "TypeType.h"
#include "..\HWLang\SourcePart.h"

static bool Trace = true;

using namespace Reni;
using namespace HWLang;

Ref<Syntax> const TokenClass::Mismatch(Ref<Syntax, true> const left, SourcePart const& part, Ref<Syntax, true> const right) const{
    md(left, part, right);
    mb;
    return Ref<Syntax, true>();
}

pure_p_implementation(TokenClass, bool, AcceptsMatch);


SearchResult const DefineableToken::Search(NumberType const& target) const
{
    bool Trace = true;
    md(target);
    b_;
    return_d(SearchResult());
};


#define RegisterSearch(tokenClass,targetType)SearchResult const tokenClass::Search(targetType const& target) const{return target.Search<tokenClass>();}

RegisterSearch(InstanceToken, TypeType);
RegisterSearch(PlusToken, NumberType);
RegisterSearch(MinusToken, NumberType);
RegisterSearch(StarToken, NumberType);
