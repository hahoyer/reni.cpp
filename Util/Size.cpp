#include "Import.h"
#include "Size.h"

using namespace Util;
using namespace HWLib;

Size const Size::Address = 64;

Size const Constants<Size>::NotValid;
Size const Constants<Size const>::NotValid;

Size const Size::Align(int alignBits) const
{
    auto result = value - 1;
    auto bit = 1;
    while (alignBits)
    {
        result |= bit;
        bit *= 2;
        alignBits--;
    }
    return result + 1;
}