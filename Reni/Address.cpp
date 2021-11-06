#include "Import.h"
#include "Address.h"

#include "Type.h"

using namespace Reni;
static bool Trace = true;


Address::Address(const Size& data, int depth) : data(data), depth(depth)
{
  SetDumpString();
}

Address Address::operator+(int delta) const
{
  HW_ASSERT_IS(delta, >, 0);
  return Address(data, depth + delta);
}
