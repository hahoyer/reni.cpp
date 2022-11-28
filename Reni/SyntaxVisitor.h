#pragma once
#include "ReplaceVisitor.h"
#include "Syntax.h"

using namespace HWLib;

namespace Reni
{
    class ReplaceSyntaxVisitor : public DumpableObject
    {
        typedef DumpableObject baseType;
        typedef ReplaceSyntaxVisitor thisType;
    public:
        HW_PR_VIRTUAL_GET(Ref<Syntax>, arg) = 0;
    private:
        HW_PR_DECL_GETTER(Array<string>,DumpData) override{ return{}; };
    };

    class SyntaxArgVisitor : public ReplaceSyntaxVisitor
    {
        typedef ReplaceSyntaxVisitor baseType;
        typedef SyntaxArgVisitor thisType;
        Ref<Syntax> const value;
    public:
        SyntaxArgVisitor(Ref<Syntax> const& value) : value(value){ SetDumpString(); }
    private:
        HW_PR_DECL_GETTER(Array<string>,DumpData) override{ return{HW_D_VALUE(value)}; };
        HW_PR_DECL_GETTER(Ref<Syntax>,arg) override{ return value; };
    };
}

using namespace Reni;

inline HW_PR_VIRTUAL_GETTER_WRAPPER(ReplaceSyntaxVisitor, Ref<Syntax>, arg);