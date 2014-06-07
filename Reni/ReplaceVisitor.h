#pragma once
#include "../HWLib/DumpableObject.h"
#include "../HWLib/Ref.h"
#include "../HWLib/WeakRef.h"

using namespace HWLib;

namespace Reni
{
    class Type;
    class CodeItem;

    class ReplaceVisitor : public DumpableObject
    {
        typedef DumpableObject baseType;
        typedef ReplaceVisitor thisType;
    public:
        bool Trace;

        virtual Ref<CodeItem, true> const Arg(Type const&type, int depth)const = 0;
        virtual Ref<CodeItem, true> const This(Type const&type, int depth)const = 0;
        
        virtual_p(bool, hasArg) { return false; };
        virtual_p(bool, hasThis) { return true; };
    private:
        p_function(Array<String>,DumpData) override{return {};};
    };

    inline pure_p_implementation(ReplaceVisitor, bool, hasArg);
    inline pure_p_implementation(ReplaceVisitor, bool, hasThis);
}



