#pragma once
#include "Syntax.h"
#include "../HWLib/Map.h"

using namespace HWLib;

namespace Reni{
    class SyntaxContainer final: public Syntax{
        typedef Syntax baseType;
        typedef SyntaxContainer thisType;
    public:
        Array<Ref<Syntax>> statements;
        Map<String, int> names;

        SyntaxContainer(SourcePart const&part);
        virtual ~SyntaxContainer(){};

        void Add(Ref<Syntax> const& definitionTarget, Ref<Syntax> const&value);
    private:
        p_function(Array<String>, DumpData) override;
        void AddTo(SyntaxContainer&) const override;
        ResultData const GetResultData(Context const& context, Category category) const override;
        Ref<CodeItem>const GetCode(Context const& context) const;
        Ref<CodeItem> const GetCode(Context const& context, int index) const;
    };
}