#include "Import.h"
#include "CodeVisitor.h"

#include "..\HWLib\DumpMacros.h"
#include "..\HWLib\BreakHandling.h"
#include "Fiber.h"

using namespace Reni;
using namespace HWLib;

static bool Trace = true;


String const CodeVisitor::ParameterName()
{
    return "arg";
}

String const CodeVisitor::InName(String const& prefix, int index) 
{
    return
        "$("
        + prefix
        + "."
        + String::Convert(index)
        + ")";
}

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

String const CodeVisitor::GetterFunction(int const index) 
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
}

String const CodeVisitor::FunctionArg() const
{
    md_;
    b_;
    return "";
};


String const MainCodeVisitor::MainVisit(Ref<CodeItem> target)
{
    return MainCodeVisitor().Visit(target);
}

String const MainCodeVisitor::Visit(Ref<CodeItem> target)const
{
    a_if(target->exts.isEmpty, nd(target));
    return "return " + target->ToCpp(*this);
}

String const MainCodeVisitor::GetterVisit(int index, Ref<CodeItem> target)
{
    MainCodeVisitor visitor;
    auto body = visitor.Visit(target);
    auto result = GetterFunction(index);
    return result
        .Replace("$(arg)", ParameterName())
        .Replace("$(body)", body);
}

String const MainCodeVisitor::Const(Size const size, BitsConst const& value) const
{
    return value.format;
}

String const MainCodeVisitor::CallGetter(Size const&, int const index, Size const&) const
{
    return GetterName(index) + "($(arg))";
}

String const MainCodeVisitor::DumpPrintNumber(Size const size) const
{
    return "DumpPrint($(arg))";
}

String const MainCodeVisitor::FunctionArg() const
{
    return ParameterName();
}

String const MainCodeVisitor::FiberConnection(Array<Ref<CodeItem>> const& items, Ref<FiberConnectorItem> const&connector) const
{
    auto connectorCodeRaw = connector->ToCpp(*this);
    auto index = 0;
    return items
        .Aggregate<String>
        (
        connectorCodeRaw,
        [&]
    (String const &current, Ref<CodeItem> const &item)
    {
        return current.Replace(InName(connector->prefix, index++), item->ToCpp(*this));
    }
    );
}

String UnrefCode(int depth, String const&target)
{
    if(depth == 0)
        return "(" + target + ")";
    if(depth == 1)
        return "(*reinterpret_cast<int const*>" + target + ")";
    fd(depth, target);
    b_;
    return target;
}


String const MainCodeVisitor::BinaryOperation(String const& name, Size const&, int leftDepth, Size const&, int rightDepth, Size const&)const
{
    return UnrefCode(leftDepth, InName(name, 0)) + " " + name + " " + UnrefCode(rightDepth, InName(name, 1));
};


