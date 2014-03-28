#pragma once
#include "Syntax.h"
using namespace HWLib;

namespace Reni{
    class SyntaxContainer final: public Syntax{
        typedef Syntax baseType;
        typedef SyntaxContainer thisType;
    public:
        Array<Ref<Syntax>> statements;
        Map<String, int> names;

        SyntaxContainer(SourcePart const&part);

        void Add(Ref<Syntax> const& definitionTarget, Ref<Syntax> const&value);
    private:
        p_function(Array<String>, DumpData) override;
        void AddTo(SyntaxContainer&) const override;
    };

}