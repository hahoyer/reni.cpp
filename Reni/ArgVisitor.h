#pragma once
#include "ReplaceVisitor.h"

using namespace HWLib;

namespace Reni
{
    class ResultCache;

    class ArgVisitor: public ReplaceVisitor 
    {
        typedef ReplaceVisitor baseType;
        typedef ArgVisitor thisType;
        ResultCache const& result;
    public:
        ArgVisitor(ResultCache const& result);
    private:
        override_p_function(Array<String>, DumpData){ return{nd(result)}; }
        virtual Ref<CodeItem> const Arg(Type const&) const override;;
    };

}
