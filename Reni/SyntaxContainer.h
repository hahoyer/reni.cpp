#pragma once
#include "Syntax.h"
#include "../HWLib/Map.h"

using namespace HWLib;

namespace Reni
{
    class Type;

    class SyntaxContainer final: public Syntax
    {
        typedef Syntax baseType;
        typedef SyntaxContainer thisType;

        Ref<Syntax> const emptySyntax;
    public:
        Array<Ref<Syntax>> statements;
        Map<DefineableToken const*, size_t> names;

        explicit SyntaxContainer(SourcePart const&part);
        SyntaxContainer(SyntaxContainer const&) = delete;
        virtual ~SyntaxContainer(){};

        HW_PR_THISREF;

        void Add(Ref<Syntax> const& definitionTarget, Ref<Syntax> const&value);
        void Add(Optional<Ref<Syntax>> const& value);
    private:
        HW_PR_DECL_GETTER(string, SmartDump) override;
        HW_PR_DECL_GETTER(int, priority) override{ return -12; }
        void AddTo(SyntaxContainer&) const override;
        ResultData GetResultData(const Context& context, Category const& category) const override;
        WeakRef<Reni::Type> GetType(Context const& context) const;
        Ref<CodeItem> GetCode(Context const& context) const;
        Ref<CodeItem> GetCode(Context const& context, size_t index) const;
        Closure GetClosure(Context const& context) const;
        Closure GetClosure(Context const& context, size_t index) const;
    };
}