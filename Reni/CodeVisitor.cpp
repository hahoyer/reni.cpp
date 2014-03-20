#include "Import.h"
#include "CodeVisitor.h"

#include "..\HWLib\DumpMacros.h"
#include "..\HWLib\BreakHandling.h"
#include "Fiber.h"

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

String const CodeVisitor::Pair(Ref<CodeItem> const& left, Ref<CodeItem> const& right, Ref<FiberConnector> const&connector) const {
    md(left, right, connector);
    b_;
    return "";
}

String const CodeVisitor::BinaryOperation(String const& name, Size const&size, Size const&leftSize, Size const&rightSize)const{
    md(name, size, leftSize, rightSize);
    b_;
    return "";
};
