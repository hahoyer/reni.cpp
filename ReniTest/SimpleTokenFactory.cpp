#include "Import.h"
#include "SimpleTokenFactory.h"

using namespace _HWLang;

typedef SimpleTokenFactory TokenFactory;
typedef TokenFactory::TokenClass TokenClass;
//using TokenFactory = SimpleTokenFactory;
//using TokenClass = TokenFactory::TokenClass;

static TokenClass const theOnlyInstance;

TokenClass const & TokenFactory::End =(theOnlyInstance);
TokenClass const & TokenFactory::Number(theOnlyInstance);
TokenClass const & TokenFactory::Text(theOnlyInstance);
TokenClass const & TokenFactory::Start(theOnlyInstance);

TokenClass const & TokenFactory::GetTokenClass(String const &){
    return theOnlyInstance;
};

TokenClass const& TokenFactory::GetErrorClass(String const&){
    return theOnlyInstance;
};