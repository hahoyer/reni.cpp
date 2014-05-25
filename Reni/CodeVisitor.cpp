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

String const CodeVisitor::FiberConnection(Array<Ref<CodeItem>> const& items, Ref<FiberConnectorItem> const&connector) const {
    md(items, connector);
    b_;
    return "";
}

String const CodeVisitor::BinaryOperation(String const& name, Size const&size, int leftDepth, Size const&leftSize, int rightDepth, Size const&rightSize)const{
    md(name, size, leftDepth, leftSize, rightDepth, rightSize);
    b_;
    return "";
};
