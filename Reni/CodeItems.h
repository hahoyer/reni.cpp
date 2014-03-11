#pragma once

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
        override_p_function(Array<String>, DumpData){ return{ nd(size), nd(value) }; };
        override_p_function(Size, size){ return size; };
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
        override_p_function(Array<String>, DumpData){ return{nd(size)}; };
        override_p_function(Size, inSize){ return Size(0); };
        override_p_function(Size, outSize){ return size; };
        virtual String const ToCpp(CodeVisitor const& visitor)const override;
        virtual Ref<FiberItem, true> const Replace(ReplaceVisitor const&) const{ return {}; }
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
        override_p_function(Array<String>, DumpData){ return{nd(type)}; };
        override_p_function(Size, size){ return size; };
        virtual String const ToCpp(CodeVisitor const& visitor)const override;
        virtual Ref<CodeItem, true> const Replace(ReplaceVisitor const&arg) const override;
    };
}
