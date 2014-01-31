#include "Import.h"
#include "MainTokenFactory.h"

using namespace Reni;
using Class = MainTokenFactory::Class;

MainTokenFactory::MainTokenFactory()
: EndOfText(_)
, Number(_)
, Text(_)
{}

Class const& MainTokenFactory::TokenClass(String const&name)const
{
    return _;
};

Class const& MainTokenFactory::Error(String const&name)const
{
    return _;
};
