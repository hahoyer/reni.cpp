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
    private:
        p_function(Array<String>,DumpData) override{return {};};
    };

}

