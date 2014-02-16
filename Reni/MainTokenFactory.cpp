#include "Import.h"
#include "MainTokenFactory.h"


#if 0
using namespace Reni;

template<class TClass>
static TClass t1;

template<class TClass>
TokenFactory::TokenFactory()
: EndOfText(t1)
, Number(t1)
, Text(t1)
, BeginOfText(t1)
{};


template<class TClass>
TClass const & TokenFactory::GetTokenClass(String const &)const
{
    return t1;
};

template<class TClass>
TClass const& TokenFactory::GetError(String const&)const
{
    return t1;
};


#endif