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
        virtual_p(Ref<Syntax>, arg) = 0;
    private:
        override_p_function(Array<String>, DumpData){ return{}; };
    };

    class SyntaxArgVisitor : public ReplaceSyntaxVisitor
    {
        typedef ReplaceSyntaxVisitor baseType;
        typedef SyntaxArgVisitor thisType;
        Ref<Syntax> const value;
    public:
        SyntaxArgVisitor(Ref<Syntax> const& value): value(value){}
    private:
        override_p_function(Array<String>, DumpData){ return{nd(value)}; };
        override_p_function(Ref<Syntax>, arg){ return value; };
    };
}

using namespace Reni;

inline pure_p_implementation(ReplaceSyntaxVisitor, Ref<Syntax>, arg);