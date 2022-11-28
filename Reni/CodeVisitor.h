#pragma once
#include "..\HWLib\DumpableObject.h"
#include "CodeItem.h"

using namespace HWLib;

namespace Reni
{
    class FiberConnectorItem;

    class CodeVisitor: public DumpableObject
    {
        using thisType = CodeVisitor;
        using baseType = DumpableObject;
    protected:
        CodeVisitor(){};
    public:
        virtual string Const(Size const size, BitsConst const& value) const;
        virtual string DumpPrintNumber(Size const size) const;
        virtual string FiberConnection(Array<Ref<CodeItem>> const& items,
                                       Ref<FiberConnectorItem> const& connector) const;
        virtual string NumberOperation(string const& name, Size const& size, int leftDepth, Size const& leftSize,
                                       int rightDepth, Size const& rightSize) const;
        virtual string NumberConversion(Size const& size, int argDepth, Size const& argSize) const;
        virtual string FunctionArg() const;
        virtual string CallGetter(Size const& result, size_t const index, Size const& args) const;
        
        static string InName(string const&prefix, size_t index);
        static string GetterFunctionDeclaration(size_t const index);
        static string GetterName(size_t const index) { return "get_" + String::Convert(index); };
        static string SetterName(size_t const index) { return "set_" + String::Convert(index); };

    };

    class MainCodeVisitor final : public CodeVisitor{
        typedef CodeVisitor baseType;
        typedef MainCodeVisitor thisType;
        HW_PR_DECL_GETTER(Array<string>, DumpData) override { return{}; };
        string Const(Size const size, BitsConst const& value) const override;
        string DumpPrintNumber(Size const size) const override;
        string FiberConnection(Array<Ref<CodeItem>> const&items, Ref<FiberConnectorItem> const&connector) const override;
        string NumberOperation(string const& name, Size const&size, int leftDepth, Size const&leftSize, int rightDepth, Size const&rightSize)const override;
        string CallGetter(Size const& result, size_t const index, Size const& args) const override;
        string FunctionArg() const override;
        string Visit(Ref<CodeItem> target) const;
    public:
        static string MainVisit(Ref<CodeItem> body);
        static string GetterVisit(size_t index, Ref<CodeItem> body);
    };

}