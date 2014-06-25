#include "Import.h"
#include "CodeVisitor.h"

#include "..\HWLib\DumpMacros.h"
#include "..\HWLib\BreakHandling.h"
#include "Fiber.h"

using namespace Reni;
using namespace HWLib;

static bool Trace = true;


virtual_p_implementation(CodeVisitor, String, ParameterName){ return "arg"; }

String const CodeVisitor::Const(Size const size, BitsConst const& value) const
{
    md(size, value)    ;
    b_;
    return "";
}

String const CodeVisitor::CallGetter(Size const& result, int const index, Size const& args) const
{
    md(result, index,args)      ;
    b_;
    return "";
}

String const CodeVisitor::GetterFunction(Size const& , int const index, Size const& ) const
{
    return "int " + GetterName(index) + "(int $(arg)) {\n$(body)\n}\n" ;
}

String const CodeVisitor::DumpPrintNumber(Size const size) const
{
    md(size)  ;
    b_;
    return "";
}

String const CodeVisitor::FiberConnection(Array<Ref<CodeItem>> const& items, Ref<FiberConnectorItem> const&connector) const
{
    md(items, connector)    ;
    b_;
    return "";
}

String const CodeVisitor::BinaryOperation(String const& name, Size const&size, int leftDepth, Size const&leftSize, int rightDepth, Size const&rightSize)const
{
    md(name, size, leftDepth, leftSize, rightDepth, rightSize)            ;
    b_;
    return "";
};


String const TopCodeVisitor::Visit(Ref<CodeItem> target)
{
    TopCodeVisitor visitor;
    a_if(target->exts.isEmpty, nd(target));
    return target->ToCpp(visitor);
}

String const TopCodeVisitor::Const(Size const size, BitsConst const& value) const
{
    a_if_(size <= BitCount<int>());
    return "return " + String::Convert(int(value)) + ";";
}

