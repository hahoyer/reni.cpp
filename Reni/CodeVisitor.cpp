#include "Import.h"
#include "CodeVisitor.h"

#include "BitsConst.h"
#include "Size.h"
#include "..\HWLib\Ref.h"
#include "..\HWLib\DumpMacros.h"
#include "..\HWLib\BreakHandling.h"

using namespace Reni;
using namespace HWLib;

static bool Trace = true;

String CodeVisitor::Const(Size const size, Ref<BitsConst> const value) const
{
    md(size, value);
    b_;
    return "";
}
