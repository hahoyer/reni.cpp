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
        ArgVisitor() {SetDumpString();}

        ArgVisitor& Assign(Tag const*tag, ResultCache const& result);

    private:
        p_function(Array<String>, DumpData) override;
        p_function(bool, hasArg) override;
        p_function(bool, hasThis) override;
        Optional<Ref<CodeItem>> const Arg(Type const&type, int depth) const override;
        Optional<Ref<CodeItem>> const This(Type const&type, int depth) const override;
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
