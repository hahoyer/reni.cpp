#include "Import.h"
#include "Address.h"

#include "Type.h"

using namespace Reni;
static bool Trace = true;


Address::Address(Size const& data, int depth) : data(data), depth(depth)
{
    SetDumpString();
}

Address const Address::operator+(int delta) const
{
    a_is(delta, >, 0);
    return Address(data, depth + delta);
}