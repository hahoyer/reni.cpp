#pragma once
#include "ReplaceVisitor.h"
#include "../HWLib/Map.h"

using namespace HWLib;

namespace Reni
{
    class ResultCache;

    class ArgVisitor: public ReplaceVisitor 
    {
        typedef ReplaceVisitor baseType;
        typedef ArgVisitor thisType;
    public:
        struct Tag{
            static Tag expressionThis;
            static Tag expressionArg;
        };
    private:
        Map<Tag const*, Ref<ResultCache>> results;
    public:
        ArgVisitor&  Assign(Tag const*tag, Ref<ResultCache> result){
            results.Assign(tag, result);
            return *this;
        }

    private:
        p_function(Array<String>, DumpData) override;
        Ref<CodeItem, true> const Arg(Type const&type) const override;
        Ref<CodeItem, true> const This(Type const&type) const override;
    };
}

template<>
inline String const HWLib::Dump(Reni::ArgVisitor::Tag const*value){
    if(value == &Reni::ArgVisitor::Tag::expressionThis)
        return  "expressionThis";
    if(value == &Reni::ArgVisitor::Tag::expressionArg)
        return  "expressionArg";
    return "<unknown>";
}
