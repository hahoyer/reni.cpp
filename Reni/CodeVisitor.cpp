#include "Import.h"
#include "CodeVisitor.h"

#include "..\HWLib\DumpMacros.h"
#include "..\HWLib\BreakHandling.h"
#include "Fiber.h"

using namespace Reni;
using namespace HWLib;

static bool Trace = true;


string CodeVisitor::InName(string const& prefix, size_t index) 
{
    return
        "$("
        + prefix
        + "."
        + String::Convert(index)
        + ")";
}

string CodeVisitor::Const(Size const size, BitsConst const& value) const
{
    HW_D_METHOD(size, value)    ;
    HW_BREAKPOINT;
    return "";
}

string CodeVisitor::CallGetter(Size const& result, size_t const index, Size const& args) const
{
    HW_D_METHOD(result, index,args)      ;
    HW_BREAKPOINT;
    return "";
}

string CodeVisitor::GetterFunctionDeclaration(size_t const index)
{
    return "size_t " + GetterName(index) + "(size_t arg)";
}

string CodeVisitor::DumpPrintNumber(Size const size) const
{
    HW_D_METHOD(size)  ;
    HW_BREAKPOINT;
    return "";
}

string CodeVisitor::FiberConnection(Array<Ref<CodeItem>> const& items, Ref<FiberConnectorItem> const& connector) const
{
    HW_D_METHOD(items, connector)    ;
    HW_BREAKPOINT;
    return "";
}

string CodeVisitor::NumberOperation(string const& name, Size const& size, int leftDepth, Size const& leftSize,
                                    int rightDepth, Size const& rightSize) const
{
    HW_D_METHOD(name, size, leftDepth, leftSize, rightDepth, rightSize)            ;
    HW_BREAKPOINT;
    return "";
}

string CodeVisitor::NumberConversion(Size const& size, int argDepth, Size const& argSize) const
{
    HW_D_METHOD(size, argDepth, argSize);
    HW_BREAKPOINT;
    return "";
}

string CodeVisitor::FunctionArg() const
{
    HW_D_METHOD_;
    HW_BREAKPOINT;
    return "";
};


string MainCodeVisitor::MainVisit(Ref<CodeItem> target)
{
    return MainCodeVisitor().Visit(target);
}

string MainCodeVisitor::Visit(Ref<CodeItem> target) const
{
    HW_ASSERT(target->closure.isEmpty, HW_D_VALUE(target));
    const auto result= target->ToCpp(*this);
    if(target->size == 0)
        return result + ";\nreturn 0;";
    return "return " + result + ";";
}

string MainCodeVisitor::GetterVisit(size_t index, Ref<CodeItem> target)
{
  const MainCodeVisitor visitor;
  const auto body = visitor.Visit(target);
  const auto result = GetterFunctionDeclaration(index);
    return result
        + "{\n\t" + body +"\n}\n";
}

string MainCodeVisitor::Const(Size const size, BitsConst const& value) const
{
    return value.format;
}

string MainCodeVisitor::CallGetter(Size const&, size_t const index, Size const&) const
{
    return GetterName(index) + "($(arg))";
}

string MainCodeVisitor::DumpPrintNumber(Size const size) const
{
    return "DumpPrint($(arg))";
}

string MainCodeVisitor::FunctionArg() const
{
    return "arg";
}

string MainCodeVisitor::FiberConnection(Array<Ref<CodeItem>> const& items, Ref<FiberConnectorItem> const&connector) const
{
    auto connectorCodeRaw = connector->ToCpp(*this);
    auto index = 0;
    return items
        .Aggregate<string>
        (
            connectorCodeRaw,
            [&]
            (string const &current, Ref<CodeItem> const &item)
            {
                return current|Replace(InName(connector->prefix, index++), item->ToCpp(*this));
            }
        );
}

string UnrefCode(int depth, string const&target)
{
    return "(" + target + ")";
    HW_D_FUNCTION(depth, target);
    HW_BREAKPOINT;
    return target;
}


string MainCodeVisitor::NumberOperation(string const& name, Size const&, int leftDepth, Size const&, int rightDepth, Size const&)const
{
    return UnrefCode(leftDepth, InName(name, 0)) + " " + name + " " + UnrefCode(rightDepth, InName(name, 1));
};


