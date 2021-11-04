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
        p_virtual(Ref<Syntax>, arg) = 0;
    private:
        p_function(Array<string>,DumpData) override{ return{}; };
    };

    class SyntaxArgVisitor : public ReplaceSyntaxVisitor
    {
        typedef ReplaceSyntaxVisitor baseType;
        typedef SyntaxArgVisitor thisType;
        Ref<Syntax> const value;
    public:
        SyntaxArgVisitor(Ref<Syntax> const& value) : value(value){ SetDumpString(); }
    private:
        p_function(Array<string>,DumpData) override{ return{nd(value)}; };
        p_function(Ref<Syntax>,arg) override{ return value; };
    };
}

using namespace Reni;

inline p_virtual_header_implementation(ReplaceSyntaxVisitor, Ref<Syntax>, arg);