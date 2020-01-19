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
        virtual String const Const(Size const size, BitsConst const& value) const;
        virtual String const DumpPrintNumber(Size const size) const;
        virtual String const FiberConnection(Array<Ref<CodeItem>> const& items, Ref<FiberConnectorItem> const&connector) const;
        virtual String const NumberOperation(String const& name, Size const&size, int leftDepth, Size const&leftSize, int rightDepth, Size const&rightSize)const;
        virtual String const NumberConversion(Size const& size, int argDepth, Size const& argSize) const;
        virtual String const FunctionArg() const;
        virtual String const CallGetter(Size const&result, int const index, Size const&args) const;
        
        static String const InName(String const&prefix, int index);
        static String const GetterFunctionDeclaration(int const index);
        static String const GetterName(int const index){ return "get_" + String::Convert(index); };
        static String const SetterName(int const index){ return "set_" + String::Convert(index); };

    };

    class MainCodeVisitor final : public CodeVisitor{
        typedef CodeVisitor baseType;
        typedef MainCodeVisitor thisType;
        p_function(Array<String>, DumpData) override { return{}; };
        String const Const(Size const size, BitsConst const& value) const override;
        String const DumpPrintNumber(Size const size) const override;
        String const FiberConnection(Array<Ref<CodeItem>> const&items, Ref<FiberConnectorItem> const&connector) const override;
        String const NumberOperation(String const& name, Size const&size, int leftDepth, Size const&leftSize, int rightDepth, Size const&rightSize)const override;
        String const CallGetter(Size const& result, int const index, Size const& args) const override;
        String const FunctionArg() const override;
        String const Visit(Ref<CodeItem> target)const;
    public:
        static String const MainVisit(Ref<CodeItem> body);
        static String const GetterVisit(int index, Ref<CodeItem> body);
    };

}