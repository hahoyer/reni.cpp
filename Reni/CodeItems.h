#pragma once

#include "BitsConst.h"
#include "Code.h"
#include "Size.h"
#include "Fiber.h"

namespace Reni{
    class ConstCode final : public CodeItem{
        typedef CodeItem baseType;
        typedef ConstCode thisType;
    public:
        Size const size;
        BitsConst const value;
        ConstCode(Size const&size, BitsConst const&value)
            : size(size)
            , value(value)
        {}
    private:
        p_function(Array<String>,DumpData) override{ return{ nd(size), nd(value) }; };
        p_function(Size,size) override{ return size; };
        virtual String const ToCpp(CodeVisitor const& visitor)const override;
    };


    class DumpPrintNumberCode final : public FiberItem {
        typedef FiberItem baseType;
        typedef DumpPrintNumberCode thisType;
    public:
        Size const size;
        DumpPrintNumberCode(Size const&size)
            : size(size)
        {}
    private:
        p_function(Array<String>,DumpData) override{ return{nd(size)}; };
        p_function(Size,inSize) override{ return size; };
        p_function(Size,outSize) override{ return 0; };
        virtual String const ToCpp(CodeVisitor const& visitor)const override;
        virtual Ref<FiberItem, true> const Replace(ReplaceVisitor const&) const{ return {}; }
    };


    class BinaryOperationCode final : public FiberItem {
        typedef FiberItem baseType;
        typedef BinaryOperationCode thisType;
    public:
        Size const size;
        Size const leftSize;
        Size const rightSize;
        String const name;
        BinaryOperationCode(String const& name, Size const&size, Size const&leftSize, Size const&rightSize)
            : name(name)
            , size(size)
            , leftSize(leftSize)
            , rightSize(rightSize)
        {}
    private:
        p_function(Array<String>, DumpData) override{ return{nd(size), nd(leftSize), nd(name), nd(rightSize)}; };
        p_function(Size, inSize) override{ return leftSize + rightSize; };
        p_function(Size, outSize) override{ return size; };
        virtual String const ToCpp(CodeVisitor const& visitor)const override;
        virtual Ref<FiberItem, true> const Replace(ReplaceVisitor const&) const{ return{}; }
    };


    class ArgCode final : public CodeItem {
        typedef CodeItem baseType;
        typedef ArgCode thisType;
    public:
        Type const& type;
        ArgCode(Type const&type)
            : type(type)
        {}
    private:
        p_function(Array<String>,DumpData) override{ return{nd(type)}; };
        p_function(Size,size) override{ return type.size; };
        virtual String const ToCpp(CodeVisitor const& visitor)const override;
        virtual Ref<CodeItem, true> const Replace(ReplaceVisitor const&arg) const override;
    };


    class ThisCode final : public CodeItem {
        typedef CodeItem baseType;
        typedef ThisCode thisType;
    public:
        Type const& type;
        ThisCode(Type const&type)
            : type(type)
        {}
    private:
        p_function(Array<String>, DumpData) override{ return{nd(type)}; };
        p_function(Size, size) override{ return type.size; };
        virtual String const ToCpp(CodeVisitor const& visitor)const override;
        virtual Ref<CodeItem, true> const Replace(ReplaceVisitor const&arg) const override;
    };


    class PairCode final : public CodeItem {
        typedef CodeItem baseType;
        typedef PairCode thisType;
    public:
        Ref<CodeItem> const left;
        Ref<CodeItem> const right;
        PairCode(CodeItem const&left, CodeItem const&right)
            : left(left.ref)
            , right(right.ref)
        {}
    private:
        p_function(Array<String>, DumpData) override{ return{nd(left),nd(right)}; };
        p_function(Size, size) override{ return left->size + right->size; };
        virtual String const ToCpp(CodeVisitor const& visitor)const override;
        virtual Ref<CodeItem, true> const Replace(ReplaceVisitor const&arg) const override;
    };
}
