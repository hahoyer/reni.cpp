#include "Import.h"
#include "SimpleTokenFactory.h"

using namespace _HWLang;

static TokenClass const theOnlyInstance;

TokenClass const & TokenFactory::End =(theOnlyInstance);
TokenClass const & TokenFactory::Number(theOnlyInstance);
TokenClass const & TokenFactory::Text(theOnlyInstance);
TokenClass const & TokenFactory::Start(theOnlyInstance);

TokenClass const & TokenFactory::GetTokenClass(String const &){
    return theOnlyInstance;
};

TokenClass const& TokenFactory::GetError(String const&){
    return theOnlyInstance;
};