#pragma once
#include "../HWLib/DumpableObject.h"
#include "BitType.h"
#include "VoidType.h"
#include "../HWLib/FunctionCache.h"

using namespace HWLib;

namespace Reni
{
    class FunctionCallResultCache;

    class Global: public DumpableObject
    {
        typedef DumpableObject baseType;
        typedef Global thisType;
        struct internal;
    public:
        class Function final : public DumpableObject
        {
            struct Xetter;
            class CodeVisitor;
            friend struct internal;
            using baseType = DumpableObject;
            using thisType = Function;

            CtrlRef<Xetter> setter;
            CtrlRef<Xetter> getter;
        public:
            Function();

            p(String, cppCode);
            void GetterIsUsed()const;
            void SetterIsUsed()const;
        private:
            p_function(Array<String>, DumpData) override;
            bool Ensure(FunctionCallResultCache const& source) const;
        };
    private:
        CtrlRef<internal> _internal;
    public:
        Global();
        ThisRef;

        BitType const bitType;
        VoidType const voidType;

        int const FunctionIndex(FunctionCallResultCache const& target)const;
        p(Array<Function>, functions);
    private:
        p_function(Array<String>, DumpData) override;
    };
}