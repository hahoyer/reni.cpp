#include "Import.h"
#include "TokenClass.h"

#include "Context.h"
#include "DefineableToken.h"
#include "DumpPrintToken.h"
#include "ExpressionSyntax.h"
#include "NumberType.h"
#include "NumberTypeIncludes.h"
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

SearchResult const DefineableToken::Search(TypeType const&) const{ return{}; }

SearchResult const DefineableToken::Search(NumberType const& target) const
{
    bool Trace = true;
    md(target);
    b_;
    return_d(SearchResult());
}

Ref<Syntax> const DefineableToken::CreateSyntax(Ref<Syntax, true> const left, SourcePart const& part, Ref<Syntax, true> const right) const
{
    return new ExpressionSyntax(*this, left, part, right);
};

pure_p_implementation(DefineableToken, int, priority);

tc_Search_implementation(InstanceToken, TypeType);
tc_Search_implementation(PlusToken, NumberType);
tc_Search_implementation(MinusToken, NumberType);
tc_Search_implementation(StarToken, NumberType);
tc_Search_implementation(EnableCutToken, NumberType);
