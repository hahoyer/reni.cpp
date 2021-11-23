#include "Import.h"
#include "SimpleTokenFactory.h"

using namespace TestHWLang;

typedef SimpleTokenFactory TokenFactory;
typedef TokenFactory::TokenClass SimpleTokenClass;

static const SimpleTokenClass theMatchInstance = true;
static const SimpleTokenClass theNormalInstance = false;

const SimpleTokenClass& TokenFactory::End = theMatchInstance;
const SimpleTokenClass& TokenFactory::Number(theNormalInstance);
const SimpleTokenClass& TokenFactory::Text(theNormalInstance);
const SimpleTokenClass& TokenFactory::Start(theNormalInstance);

const SimpleTokenClass& TokenFactory::GetTokenClass(const string& name)
{
  if(name.size() == 1 && string(")]}") | Contains(name[0]))
    return theMatchInstance;
  return theNormalInstance;
};

const SimpleTokenClass& TokenFactory::GetErrorClass(const string&)
{
  return theNormalInstance;
};
