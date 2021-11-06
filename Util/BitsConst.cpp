#include "Import.h"
#include "BitsConst.h"

#include "Size.h"

static bool Trace = true;

using namespace Util;

BitsConst BitsConst::Empty()
{
  return BitsConst();
}

BitsConst BitsConst::Convert(const string& text)
{
  return BitsConst(text);
};

BitsConst::operator const size_t() const
{
  return value.toUlong();
}

p_implementation(BitsConst, Size, size)
{
  if(value == BigInt::Rossi(0))
    return 0;
  size_t result = (value.getUnitsSize() - 1) * 8;
  HW_ASSERT_IS(result, <, 10000);
  BigInt::Rossi value = this->value;
  if(result != 0)
    value >>= result;

  if(value < BigInt::Rossi(0))
    value = -value;
  else
    result++;

  while(value != BigInt::Rossi(0))
  {
    value >>= 1;
    result++;
  }
  return result;
}

p_implementation(BitsConst, string, format)
{
  return
    Array<BigInt::Ulong>(value.ToArray())
    .Select<string>([](__int64 element) { return String::Convert(element); })
    ->Stringify(", ");
}


p_implementation(BitsConst, string, DumpShort)
{
  return value.toStrDec() + string(" size=") + size.DumpShort;
};
