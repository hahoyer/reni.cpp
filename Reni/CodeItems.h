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
        HW_PR_DECL_GETTER(Array<std::string>,DumpData) override{ return{ HW_D_VALUE(size), HW_D_VALUE(value) }; };
        HW_PR_DECL_GETTER(Size,size) override{ return size; };
        HW_PR_DECL_GETTER(Closure, closure)override{ return{}; };
        HW_PR_DECL_GETTER(bool, isEmpty)override;
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
        HW_PR_DECL_GETTER(Array<std::string>,DumpData) override{ return{HW_D_VALUE(result),HW_D_VALUE(index), HW_D_VALUE(arg)}; };
        HW_PR_DECL_GETTER(Size,argSize) override{ return arg; };
        HW_PR_DECL_GETTER(Size,size) override{ return result; };
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
        HW_PR_DECL_GETTER(Array<std::string>, DumpData) override{ return{HW_D_VALUE(_size)}; };
        HW_PR_DECL_GETTER(Size, argSize) override{ return _size; };
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
        HW_PR_DECL_GETTER(Array<std::string>, DumpData) override{ return{HW_D_VALUE(referenceSize), HW_D_VALUE(offset)}; };
        HW_PR_DECL_GETTER(Size, argSize) override{ return referenceSize; };
        HW_PR_DECL_GETTER(Size, size) override{ return referenceSize; };
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
        HW_PR_DECL_GETTER(Array<std::string>, DumpData) override
        {
            return
            {
                HW_D_VALUE(size),
                HW_D_VALUE(leftDepth), 
                HW_D_VALUE(leftSize), 
                HW_D_VALUE(name), 
                HW_D_VALUE(rightDepth), 
                HW_D_VALUE(rightSize)
            };
        };
        HW_PR_DECL_GETTER(int, inCount) override{ return 2; };
        HW_PR_DECL_GETTER(Size, size) override { return _size; };
        HW_PR_DECL_GETTER(std::string, prefix) override { return name; };
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
        HW_PR_DECL_GETTER(Array<std::string>, DumpData) override
        {
            return
            {
                HW_D_VALUE(size),
                HW_D_VALUE(argDepth),
                HW_D_VALUE(_argSize)
            };
        };
        HW_PR_DECL_GETTER(Size, size) override { return _size; };
        HW_PR_DECL_GETTER(Size, argSize) override { return _argSize; };
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
        HW_PR_DECL_GETTER(Array<std::string>,DumpData) override;
        HW_PR_DECL_GETTER(Size,size) override;
    };


    class ArgCode final : public TypedCode{
        using baseType = TypedCode;
        typedef ArgCode thisType;
    public:
        explicit ArgCode(Type const&value);
    private:
        HW_PR_DECL_GETTER(Closure, closure)override;
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
        HW_PR_DECL_GETTER(Closure, closure)override;
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
        HW_PR_DECL_GETTER(Closure, closure)override{ return{}; }
        HW_PR_DECL_GETTER(bool, isReference)override{ return true; }
        Optional<Ref<CodeItem>> ReplaceImpl(ReplaceVisitor const&) const override{ return{}; }
    };


    class DereferenceCode final : public TypedCode{
        using baseType = TypedCode;
        using thisType = DereferenceCode;
    public:
        explicit DereferenceCode(Type const&value);
    private:
        HW_PR_DECL_GETTER(Closure, closure)override{ return{}; };
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
        HW_PR_DECL_GETTER(Array<std::string>, DumpData) override{ return{HW_D_VALUE(items),HW_D_VALUE(connector)}; };
        HW_PR_DECL_GETTER(Size, size) override{ return connector->size; };
        HW_PR_DECL_GETTER(Closure, closure)override;
        HW_PR_GET(bool, IsValid) {
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
        HW_PR_DECL_GETTER(Array<std::string>, DumpData) override
        {
            return
            {
                HW_D_VALUE(data)
            };
        };
        HW_PR_DECL_GETTER(int, inCount) override{ return 2; };
        HW_PR_DECL_GETTER(Size, size) override { return 0; };
        HW_PR_DECL_GETTER(std::string, prefix) override { return "assign"; };
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
