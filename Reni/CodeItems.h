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
        virtual String const ToCpp(CodeVisitor const& visitor)const override;
        Ref<CodeItem, true> const ReplaceImpl(ReplaceVisitor const&) const override{ return{}; }
    };


    class DumpPrintNumberCode final : public FiberItem {
        typedef FiberItem baseType;
        typedef DumpPrintNumberCode thisType;
    public:
        Size const size;
        DumpPrintNumberCode(Size const&size)
            : size(size) {
            SetDumpString();
        }
    private:
        p_function(Array<String>,DumpData) override{ return{nd(size)}; };
        p_function(Size,argSize) override{ return size; };
        p_function(Size,size) override{ return 0; };
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
        Ref<FiberItem, true> const ReplaceImpl(ReplaceVisitor const&) const{ return{}; }
    };


    class BinaryOperationCode final : public FiberConnector {
        typedef FiberConnector baseType;
        typedef BinaryOperationCode thisType;
        Size const _size;
        Size const _leftSize;
        Size const _rightSize;
        int leftDepth;
        int rightDepth;
    public:
        String const name;
        BinaryOperationCode(String const& name, Size const&size, Size const&leftSize, int leftDepth, Size const&rightSize, int rightDepth)
            : name(name)
            , _size(size)
            , _leftSize(leftSize)
            , _rightSize(rightSize) 
            , leftDepth(leftDepth)
            , rightDepth(rightDepth)
        {
            SetDumpString();
        }
    private:
        p_function(Array<String>, DumpData) override{ return{nd(size), nd(leftDepth), nd(leftSize), nd(name), nd(rightDepth), nd(rightSize)}; };
        p_function(Size, leftSize) override{ return leftDepth? Size::Address : _leftSize; };
        p_function(Size, rightSize) override { return rightDepth ? Size::Address : _rightSize; };
        p_function(Size, size) override {return _size;};
        virtual String const ToCpp(CodeVisitor const& visitor)const override;
        Ref<FiberItem, true> const Replace(ReplaceVisitor const&) const override{ return{}; }
    };


    class ArgCode final : public CodeItem {
        typedef CodeItem baseType;
        typedef ArgCode thisType;
    public:
        Type const& type;
        ArgCode(Type const&type)
            : type(type) {
            SetDumpString();
        }
    private:
        p_function(Array<String>,DumpData) override{ return{nd(type)}; };
        p_function(Size,size) override{ return type.size; };
        virtual String const ToCpp(CodeVisitor const& visitor)const override;
        Ref<CodeItem, true> const ReplaceImpl(ReplaceVisitor const&) const;
    };


    class ThisCode final : public CodeItem {
        typedef CodeItem baseType;
        typedef ThisCode thisType;
    public:
        Type const& type;
        ThisCode(Type const&type)
            : type(type) {
            SetDumpString();
        }
    private:
        p_function(Array<String>, DumpData) override{ return{nd(type)}; };
        p_function(Size, size) override{ return Size::Address; };
        virtual String const ToCpp(CodeVisitor const& visitor)const override;
        Ref<CodeItem, true> const ReplaceImpl(ReplaceVisitor const&arg) const override;
    };


    class PairCode final : public CodeItem {
        typedef CodeItem baseType;
        typedef PairCode thisType;
    public:
        Ref<CodeItem> const left;
        Ref<CodeItem> const right;
        Ref<FiberConnector> const connector;
        PairCode(Ref<CodeItem> const&left, Ref<CodeItem> const&right, Ref<FiberConnector> const&connector);
    private:
        p_function(Array<String>, DumpData) override{ return{nd(left),nd(right),nd(connector)}; };
        p_function(Size, size) override{ return connector->size; };
        p(bool, IsValid) {
            return left->size == connector->leftSize 
                && right->size == connector->rightSize;
        };
        virtual String const ToCpp(CodeVisitor const& visitor)const override;
        Ref<CodeItem, true> const ReplaceImpl(ReplaceVisitor const&arg) const override;
    };


    class ReferenceCode final : public CodeItem{
        typedef CodeItem baseType; 
        typedef ReferenceCode thisType;
        Type const&value;
    public:
        ReferenceCode(Type const&value)
            : value(value){
            SetDumpString();
        }
    private:
        p_function(Array<String>, DumpData) override{ return{nd(value)}; };
        p_function(Size, size) override{ return Size::Address; }

        Ref<CodeItem> const ReferencePlus(Size offset) const override;
        Ref<CodeItem, true> const ReplaceImpl(ReplaceVisitor const&arg) const override;
    };

}
