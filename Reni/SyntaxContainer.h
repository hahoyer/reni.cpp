#pragma once
#include "Syntax.h"
using namespace HWLib;

namespace Reni{
    class SyntaxContainer final: public Syntax{
        typedef Syntax baseType;
        typedef SyntaxContainer thisType;
        Array<Ref<Syntax>> statements;
        Map<String, int> names;
    public:
        SyntaxContainer(SourcePart const&part);

        void Add(Ref<Syntax> const& definitionTarget, Ref<Syntax> const&value);
    private:
        p_function(Array<String>, DumpData) override;
    };

}