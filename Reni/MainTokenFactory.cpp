#include "Import.h"
#include "MainTokenFactory.h"
#include "TokenClass.h"

using namespace Reni;

static TokenClass t1;

MainTokenFactory::MainTokenFactory()
: EndOfText(t1)
, Number(t1)
, Text(t1)
, BeginOfText(t1)
{};


TokenClass const & MainTokenFactory::GetTokenClass(String const &)const
{
    return t1;
};

TokenClass const& MainTokenFactory::GetError(String const&)const
{
    return t1;
};


