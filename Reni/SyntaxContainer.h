#pragma once
#include "Syntax.h"
#include "../HWLib/Map.h"

using namespace HWLib;

namespace Reni
{
    class SyntaxContainer final: public Syntax
    {
        typedef Syntax baseType;
        typedef SyntaxContainer thisType;

        Ref<Syntax> const emptySyntax;
    public:
        Array<Ref<Syntax>> statements;
        Map<DefineableToken const*, int> names;

        explicit SyntaxContainer(SourcePart const&part);
        SyntaxContainer(SyntaxContainer const&) = delete;
        virtual ~SyntaxContainer(){};

        ThisRef;

        void Add(Ref<Syntax> const& definitionTarget, Ref<Syntax> const&value);
        void Add(Optional<Ref<Syntax>> const& value);
    private:
        p_function(String, SmartDump) override;
        p_function(int, priority) override{ return -12; }
        void AddTo(SyntaxContainer&) const override;
        ResultData const GetResultData(Context const& context, Category category) const override;
        Ref<CodeItem> const GetCode(Context const& context) const;
        Ref<CodeItem> const GetCode(Context const& context, int index) const;
    };
}