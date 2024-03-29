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
            friend struct internal;
            using baseType = DumpableObject;
            using thisType = Function;

            CtrlRef<Xetter> setter;
            CtrlRef<Xetter> getter;
        public:
            Function();

            HW_PR_GET(string, cppCode);
            HW_PR_GET(string, cppDeclarations);
            void GetterIsUsed()const;
            void SetterIsUsed()const;
        private:
            HW_PR_DECL_GETTER(Array<string>, DumpData) override;
            bool Ensure(FunctionCallResultCache const& source) const;
        };
    private:
        CtrlRef<internal> _internal;
    public:
        Global();
        HW_PR_THISREF;

        BoolType const boolType;
        BitType const bitType;
        VoidType const voidType;

        size_t const FunctionIndex(FunctionCallResultCache const& target)const;
        HW_PR_GET(Array<Function>, functions);
    private:
        HW_PR_DECL_GETTER(Array<string>, DumpData) override;
    };
}