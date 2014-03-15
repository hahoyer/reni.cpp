#include "Import.h"
#include "SimpleTokenFactory.h"

using namespace _HWLang;

typedef SimpleTokenFactory TokenFactory;
typedef TokenFactory::TokenClass TokenClass;
//using TokenFactory = SimpleTokenFactory;
//using TokenClass = TokenFactory::TokenClass;

static TokenClass const theMatchInstance = true;
static TokenClass const theNormalInstance = false;

TokenClass const & TokenFactory::End =(theMatchInstance);
TokenClass const & TokenFactory::Number(theNormalInstance);
TokenClass const & TokenFactory::Text(theNormalInstance);
TokenClass const & TokenFactory::Start(theNormalInstance);

TokenClass const & TokenFactory::GetTokenClass(String const &name){
    if(name.Count == 1 && String(")]}").Contains(name[0]))
        return theMatchInstance;
    return theNormalInstance;
};

TokenClass const& TokenFactory::GetErrorClass(String const&){
    return theNormalInstance;
};

pure_p_implementation(TokenClass, Optional<bool>, HasLeft);
pure_p_implementation(TokenClass, Optional<bool>, HasRight);
