#pragma once

#include "Syntax.h"

using namespace HWLib;
using namespace HWLang;

namespace Reni
{
    class SyntaxContainer;

    class EmptySyntax final : public Syntax
    {
        using baseType = Syntax;
        using thisType = EmptySyntax;
    public:
        explicit EmptySyntax(SourcePart const& part)
            : Syntax(part) {}
    private:
        p_function(string, SmartDump) override{ return ""; };
        p_function(int, priority) override{ return 0; }

        void AddTo(SyntaxContainer& main) const override;
    };

};
