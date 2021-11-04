#pragma once

#include "CodeItem.h"
#include "Fiber.h"
#include "External.h"

#include "../HWLib/DumpToString.h"

namespace Reni{
    class FunctionCallContext;

    class ConstCode final : public CodeItem{
        typedef CodeItem baseType;
        typedef ConstCode thisType;
    public:
        Size const size;
        BitsConst const value;
        ConstCode(Size const&size, BitsConst const&value)
            : size(size)
            , value(value) {
            SetDumpString();
        }
    private:
        p_function(Array<std::string>,DumpData) override{ return{ nd(size), nd(value) }; };
        p_function(Size,size) override{ return size; };
        p_function(Closure, closure)override{ return{}; };
        p_function(bool, isEmpty)override;
        string ToCpp(CodeVisitor const& visitor) const override;
        Optional<Ref<CodeItem>> ReplaceImpl(ReplaceVisitor const&) const override{ return{}; }
    };


    class CallGetterFiber final : public FiberItem {
        typedef FiberItem baseType;
        typedef CallGetterFiber thisType;

        Size const result;
        size_t const index;
        Size const arg;
    public:

        CallGetterFiber(Size const&result, size_t index, Size const&arg)
            : result(result)
            , index(index)
            , arg(arg)
        {
            SetDumpString();
        }
    private:
        p_function(Array<std::string>,DumpData) override{ return{nd(result),nd(index), nd(arg)}; };
        p_function(Size,argSize) override{ return arg; };
        p_function(Size,size) override{ return result; };
        string ToCpp(CodeVisitor const& visitor) const override;
        Optional<Ref<FiberItem>> Replace(ReplaceVisitor const&) const override{ return{}; }
    };


    class DumpPrintNumberCode final : public FiberItem {
        typedef FiberItem baseType;
        typedef DumpPrintNumberCode thisType;
    public:
        Size const _size;

        explicit DumpPrintNumberCode(Size const&size)
            : _size(size) {
            SetDumpString();
        }
    private:
        p_function(Array<std::string>, DumpData) override{ return{nd(_size)}; };
        p_function(Size, argSize) override{ return _size; };
        string ToCpp(CodeVisitor const& visitor) const override;
        Optional<Ref<FiberItem>> Replace(ReplaceVisitor const&) const override{ return{}; }
    };


    class ReferencePlusCode final : public FiberItem {
        typedef FiberItem baseType;
        typedef ReferencePlusCode  thisType;
        Size const referenceSize;
        Size const offset;
    public:
        ReferencePlusCode(Size const&referenceSize, Size const&offset)
            : referenceSize(referenceSize), offset(offset) {
            SetDumpString();
        }
    private:
        p_function(Array<std::string>, DumpData) override{ return{nd(referenceSize), nd(offset)}; };
        p_function(Size, argSize) override{ return referenceSize; };
        p_function(Size, size) override{ return referenceSize; };
        Optional<Ref<FiberItem>> Replace(ReplaceVisitor const&) const override { return{}; }
    };


    class NumberOperationCode final : public FiberConnectorItem {
        typedef FiberConnectorItem baseType;
        typedef NumberOperationCode thisType;
        Size const _size;
        Size const leftSize;
        Size const rightSize;
        int const leftDepth;
        int const rightDepth;
    public:
        std::string const name;
        NumberOperationCode(std::string const& name, Size const&size, Size const&leftSize, int leftDepth, Size const&rightSize, int rightDepth)
            : _size(size)
            , leftSize(leftSize)
            , rightSize(rightSize)
            , leftDepth(leftDepth) 
            , rightDepth(rightDepth)
            , name(name)
        {
            SetDumpString();
        }
    private:
        p_function(Array<std::string>, DumpData) override
        {
            return
            {
                nd(size),
                nd(leftDepth), 
                nd(leftSize), 
                nd(name), 
                nd(rightDepth), 
                nd(rightSize)
            };
        };
        p_function(int, inCount) override{ return 2; };
        p_function(Size, size) override { return _size; };
        p_function(std::string, prefix) override { return name; };
        std::string ToCpp(CodeVisitor const& visitor) const override;
        Optional<Ref<FiberItem>> Replace(ReplaceVisitor const&) const override{ return{}; }

        Size InSize(int index) const override
        {
            if(index == 0)
                return leftDepth ? Size::Address : leftSize;
            return rightDepth ? Size::Address : rightSize;
        }

    };


    class NumberConversionCode final : public FiberItem {
        typedef FiberItem baseType;
        typedef NumberConversionCode  thisType;
        Size const _size;
        Size const _argSize;
        int const argDepth;
    public:
        NumberConversionCode(Size const&size, Size const&argSize, int argDepth)
            : _size(size)
            , _argSize(argSize)
            , argDepth(argDepth)
        {
            SetDumpString();
        }
    private:
        p_function(Array<std::string>, DumpData) override
        {
            return
            {
                nd(size),
                nd(argDepth),
                nd(_argSize)
            };
        };
        p_function(Size, size) override { return _size; };
        p_function(Size, argSize) override { return _argSize; };
        string ToCpp(CodeVisitor const& visitor) const override;
        Optional<Ref<FiberItem>> Replace(ReplaceVisitor const&) const override{ return{}; }
    };

    class TypedCode : public CodeItem {
        typedef CodeItem baseType;
        typedef TypedCode thisType;
    public:
        Type const& type;
    protected:
        explicit TypedCode(Type const& type);
    private:
        p_function(Array<std::string>,DumpData) override;
        p_function(Size,size) override;
    };


    class ArgCode final : public TypedCode{
        using baseType = TypedCode;
        typedef ArgCode thisType;
    public:
        explicit ArgCode(Type const&value);
    private:
        p_function(Closure, closure)override;
        string ToCpp(CodeVisitor const& visitor) const override;
        Optional<Ref<CodeItem>> ReplaceImpl(ReplaceVisitor const&) const override;
        Ref<CodeItem> Convert(Type const& type) const override;
    };


    class ThisCode final : public TypedCode{
        using baseType = TypedCode;
        typedef ThisCode thisType;
    public:
        explicit ThisCode(Type const& type);
    private:
        p_function(Closure, closure)override;
        string ToCpp(CodeVisitor const& visitor) const override;
        Optional<Ref<CodeItem>> ReplaceImpl(ReplaceVisitor const& arg) const override;
    };


    class FunctionArgCode final : public TypedCode{
        using baseType = TypedCode;
        typedef FunctionArgCode  thisType;
    public:
        FunctionArgCode(Type const&type);
    private:
        string ToCpp(CodeVisitor const& visitor) const override;
        p_function(Closure, closure)override{ return{}; }
        p_function(bool, isReference)override{ return true; }
        Optional<Ref<CodeItem>> ReplaceImpl(ReplaceVisitor const&) const override{ return{}; }
    };


    class DereferenceCode final : public TypedCode{
        using baseType = TypedCode;
        using thisType = DereferenceCode;
    public:
        explicit DereferenceCode(Type const&value);
    private:
        p_function(Closure, closure)override{ return{}; };
        string ToCpp(CodeVisitor const& visitor) const override;
        Optional<Ref<CodeItem>> ReplaceImpl(ReplaceVisitor const&) const override { return{}; };
    };

    class FiberConnector final : public CodeItem {
        typedef CodeItem baseType;
        typedef FiberConnector thisType;
    public:
        Array<Ref<CodeItem>> const items;
        Ref<FiberConnectorItem> const connector;
        FiberConnector(Array<Ref<CodeItem>> const&items, Ref<FiberConnectorItem> const&connector);
    private:
        p_function(Array<std::string>, DumpData) override{ return{nd(items),nd(connector)}; };
        p_function(Size, size) override{ return connector->size; };
        p_function(Closure, closure)override;
        p(bool, IsValid) {
            if(items.Count != connector->inCount)
                return false;
            for(auto i = 0; i < items.Count; i++)
                if(items[i]->size != connector->InSize(i))
                    return false;
            return true;
        };
        string ToCpp(CodeVisitor const& visitor) const override;
        Optional<Ref<CodeItem>> ReplaceImpl(ReplaceVisitor const& arg) const override;
    };

    class AssignCode final : public FiberConnectorItem {
        typedef AssignCode baseType;
        typedef NumberOperationCode thisType;
        Size const data;
    public:
        AssignCode(Size const& data)
            : data(data)
        {
            SetDumpString();
        }
    private:
        p_function(Array<std::string>, DumpData) override
        {
            return
            {
                nd(data)
            };
        };
        p_function(int, inCount) override{ return 2; };
        p_function(Size, size) override { return 0; };
        p_function(std::string, prefix) override { return "assign"; };
        std::string ToCpp(CodeVisitor const& visitor)const override;
        Optional<Ref<FiberItem>> Replace(ReplaceVisitor const&) const override{ return{}; }

        Size InSize(int index) const override
        {
            if(index == 0)
                return Size::Address ;
            return data;
        }

    };

}
