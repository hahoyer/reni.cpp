#include "Import.h"
#include "TokenClass.h"

#include "AccessType.h"
#include "CodeItems.h"
#include "Context.h"
#include "ContainerContext.h"
#include "DefineableToken.h"
#include "DumpPrintToken.h"
#include "ExpressionSyntax.h"
#include "Feature.h"
#include "NumberType.h"
#include "NumberTypeIncludes.h"
#include "Syntax.h"
#include "SyntaxContainer.h"
#include "TypeType.h"

#include "../HWLib/RefCountContainer.instance.h"

using namespace Reni;
using namespace HWLang;

SearchResult<Feature>  const DefineableToken::Declarations(TypeType const&) const{ return{}; };
SearchResult <Feature> const DefineableToken::Declarations(NumberType const&) const{ return{}; };

SearchResult <Feature> const DefineableToken::Declarations(EnableCutType const& provider) const
{
    bool Trace = true;
    HW_D_METHOD(provider);
    HW_BREAKPOINT;
    return_d(SearchResult<Feature>());
};

SearchResult<Feature> const DefineableToken::Declarations(AccessType const&) const{ return{}; };

Ref<Syntax> const DefineableToken::CreateSyntax(Optional<Ref<Syntax>> const left, SourcePart const& part, Optional<Ref<Syntax>> const right) const
{
    return new ExpressionSyntax(*this, left, part, right);
};

HW_PR_VIRTUAL_GETTER_WRAPPER(DefineableToken, int, priority);

tc_Search_implementation(InstanceToken, TypeType);
tc_Search_implementation(PlusToken, NumberType);
tc_Search_implementation(MinusToken, NumberType);
tc_Search_implementation(StarToken, NumberType);
tc_Search_implementation(EnableCutToken, NumberType);
tc_Search_implementation(EqualToken, NumberType);
tc_Search_implementation(GreaterToken, NumberType);
tc_Search_implementation(DumpPrintToken, NumberType);
tc_Search_implementation(ColonEqual, AccessType);

