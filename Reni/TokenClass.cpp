#include "Import.h"
#include "TokenClass.h"

#include "CodeItems.h"
#include "Context.h"
#include "ContextIncludes.h"
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

Ref<Syntax> const TokenClass::Mismatch(Optional<Ref<Syntax>> const left, SourcePart const& part, Optional<Ref<Syntax>> const right) const{
    md(left, part, right);
    mb;
    return Optional<Ref<Syntax>>();
}

p_virtual_header_implementation(TokenClass, bool, AcceptsMatch);

SearchResult<Feature>  const DefineableToken::Declarations(TypeType const&) const{ return{}; }

SearchResult <Feature> const DefineableToken::Declarations(NumberType const& target) const
{
    bool Trace = true;
    md(target);
    b_;
    return_d(SearchResult<Feature>());
}

SearchResult <Feature> const DefineableToken::Declarations(EnableCutType const& target) const
{
    bool Trace = true;
    md(target);
    b_;
    return_d(SearchResult<Feature>());
}

Ref<Syntax> const DefineableToken::CreateSyntax(Optional<Ref<Syntax>> const left, SourcePart const& part, Optional<Ref<Syntax>> const right) const
{
    return new ExpressionSyntax(*this, left, part, right);
};

p_virtual_header_implementation(DefineableToken, int, priority);

tc_Search_implementation(InstanceToken, TypeType);
tc_Search_implementation(PlusToken, NumberType);
tc_Search_implementation(MinusToken, NumberType);
tc_Search_implementation(StarToken, NumberType);
tc_Search_implementation(EnableCutToken, NumberType);
tc_Search_implementation(EqualToken, NumberType);
tc_Search_implementation(GreaterToken, NumberType);
tc_Search_implementation(DumpPrintToken, NumberType);

