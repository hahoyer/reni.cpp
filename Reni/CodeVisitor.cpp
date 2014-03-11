#include "Import.h"
#include "CodeVisitor.h"

#include "BitsConst.h"
#include "Size.h"
#include "..\HWLib\DumpMacros.h"
#include "..\HWLib\BreakHandling.h"

using namespace Reni;
using namespace HWLib;

static bool Trace = true;

String const CodeVisitor::Const(Size const size, BitsConst const& value) const
{
    md(size, value);
    b_;
    return "";
}

String const CodeVisitor::DumpPrintNumber(Size const size) const{
    md(size);
    b_;
    return "";
}