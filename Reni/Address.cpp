#include "Import.h"
#include "Address.h"

#include "Type.h"

using namespace Reni;
static bool Trace = true;


Address::Address(Address const& other) : data(other.data), depth(other.depth)
{
    SetDumpString();
    a_if_(data.isData);
}

Address::Address(Type const& data, int depth): data(data), depth(depth)
{
    SetDumpString();
    a_if_(data.isData);
}

Address const Address::operator+(int delta) const
{
    a_is(delta, >, 0);
    return Address(data, depth + delta);
}