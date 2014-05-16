#include "Import.h"
#include "Size.h"

using namespace Util;
using namespace HWLib;

Size const Size::Address = 64;

Size const Constants<Size>::NotValid = Size(Constants<int>::NotValid);
Size const Constants<Size const>::NotValid = Size(Constants<int>::NotValid);
