#include "Import.h"
#include "SimpleTokenFactory.h"

using namespace _HWLang;

typedef SimpleTokenFactory TokenFactory;
typedef TokenFactory::TokenClass SimpleTokenClass;

static SimpleTokenClass const theMatchInstance = true;
static SimpleTokenClass const theNormalInstance = false;

SimpleTokenClass const & TokenFactory::End =(theMatchInstance);
SimpleTokenClass const & TokenFactory::Number(theNormalInstance);
SimpleTokenClass const & TokenFactory::Text(theNormalInstance);
SimpleTokenClass const & TokenFactory::Start(theNormalInstance);

SimpleTokenClass const & TokenFactory::GetTokenClass(String const &name){
    if(name.Count == 1 && String(")]}").Contains(name[0]))
        return theMatchInstance;
    return theNormalInstance;
};

SimpleTokenClass const& TokenFactory::GetErrorClass(String const&){
    return theNormalInstance;
};
