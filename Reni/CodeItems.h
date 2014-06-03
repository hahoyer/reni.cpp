#pragma once

#include "Code.h"
#include "Fiber.h"
#include "../HWLib/DumpToString.h"

namespace Reni{
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
        p_function(Array<String>,DumpData) override{ return{ nd(size), nd(value) }; };
        p_function(Size,size) override{ return size; };
        p_function(Array<Ref<External>>, externals)override{ return{}; };
        virtual String const ToCpp(CodeVisitor const& visitor)const override;
        Ref<CodeItem, true> const ReplaceImpl(ReplaceVisitor const&) const override{ return{}; }
    };


    class DumpPrintNumberCode final : public FiberItem {
        typedef FiberItem baseType;
        typedef DumpPrintNumberCode thisType;
    public:
        Size const size;

        explicit DumpPrintNumberCode(Size const&size)
            : size(size) {
            SetDumpString();
        }
    private:
        p_function(Array<String>,DumpData) override{ return{nd(size)}; };
        p_function(Size,argSize) override{ return size; };
        p_function(Size,size) override{ return 0; };
        p_function(Array<Ref<External>>, externals)override{ return{}; };
        virtual String const ToCpp(CodeVisitor const& visitor)const override;
        Ref<FiberItem, true> const Replace(ReplaceVisitor const&) const override{ return{}; }
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
        p_function(Array<String>, DumpData) override{ return{nd(referenceSize), nd(offset)}; };
        p_function(Size, argSize) override{ return referenceSize; };
        p_function(Size, size) override{ return referenceSize; };
        p_function(Array<Ref<External>>, externals)override{ return{}; };
        Ref<FiberItem, true> const ReplaceImpl(ReplaceVisitor const&) const{ return{}; }
    };


    class BinaryOperationCode final : public FiberConnectorItem {
        typedef FiberConnectorItem baseType;
        typedef BinaryOperationCode thisType;
        Size const _size;
        Size const leftSize;
        Size const rightSize;
        int const leftDepth;
        int const rightDepth;
    public:
        String const name;
        BinaryOperationCode(String const& name, Size const&size, Size const&leftSize, int leftDepth, Size const&rightSize, int rightDepth)
            : name(name)
            , _size(size)
            , leftSize(leftSize)
            , rightSize(rightSize) 
            , leftDepth(leftDepth)
            , rightDepth(rightDepth)
        {
            SetDumpString();
        }
    private:
        p_function(Array<String>, DumpData) override
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
        p_function(Array<Ref<External>>, externals)override{ return{}; };
        virtual String const ToCpp(CodeVisitor const& visitor)const override;
        Ref<FiberItem, true> const Replace(ReplaceVisitor const&) const override{ return{}; }

        Size const InSize(int index) const override
        {
            if(index == 0)
                return leftDepth ? Size::Address : leftSize;
            return rightDepth ? Size::Address : rightSize;
        }

    };


    class TypedCode : public CodeItem {
        typedef CodeItem baseType;
        typedef TypedCode thisType;
    public:
        Type const& type;
        int const depth;
        TypedCode(Type const&type, int depth)
            : type(type)
            , depth(depth)
        {
        }
    private:
        p_function(Array<String>,DumpData) override{ return{nd(type),nd(depth)}; };
        p_function(Size,size) override;
    };


    class ArgCode final : public TypedCode{
        using baseType = TypedCode;
        typedef ArgCode thisType;
    public:
        ArgCode(Type const&type, int depth)
            : baseType(type,depth)
        {
            SetDumpString();
        }
    private:
        p_function(Array<Ref<External>>, externals)override;
        virtual String const ToCpp(CodeVisitor const& visitor)const override;
        Ref<CodeItem, true> const ReplaceImpl(ReplaceVisitor const&) const;
    };


    class ThisCode final : public TypedCode{
        using baseType = TypedCode;
        typedef ThisCode thisType;
    public:
        ThisCode(Type const&type, int depth)
            : baseType(type, depth)
        {
            SetDumpString();
        }
    private:
        p_function(Array<Ref<External>>, externals)override;
        virtual String const ToCpp(CodeVisitor const& visitor)const override;
        Ref<CodeItem, true> const ReplaceImpl(ReplaceVisitor const&arg) const override;
    };


    class FiberConnector final : public CodeItem {
        typedef CodeItem baseType;
        typedef FiberConnector thisType;
    public:
        Array<Ref<CodeItem>> const items;
        Ref<FiberConnectorItem> const connector;
        FiberConnector(Array<Ref<CodeItem>> const&items, Ref<FiberConnectorItem> const&connector);
    private:
        p_function(Array<String>, DumpData) override{ return{nd(items),nd(connector)}; };
        p_function(Size, size) override{ return connector->size; };
        p_function(Array<Ref<External>>, externals)override;
        p(bool, IsValid) {
            if(items.Count != connector->inCount)
                return false;
            for(auto i = 0; i < items.Count; i++)
                if(items[i]->size != connector->InSize(i))
                    return false;
            return true;
        };
        virtual String const ToCpp(CodeVisitor const& visitor)const override;
        Ref<CodeItem, true> const ReplaceImpl(ReplaceVisitor const&arg) const override;
    };


    class ReferenceCode final : public CodeItem{
        typedef CodeItem baseType; 
        typedef ReferenceCode thisType;
        Type const&value;
    public:
        explicit ReferenceCode(Type const&value)
            : value(value){
            SetDumpString();
        }
    private:
        p_function(Array<String>, DumpData) override{ return{nd(value)}; };
        p_function(Size, size) override{ return Size::Address; }
        p_function(Array<Ref<External>>, externals)override;

        Ref<CodeItem> const ReferencePlus(Size offset) const override;
        Ref<CodeItem, true> const ReplaceImpl(ReplaceVisitor const&arg) const override;
    };

}
