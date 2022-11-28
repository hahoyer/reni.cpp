#include "Import.h"
#include "Syntax.h"

#include "CodeItem.h"
#include "Context.h"
#include "ContainerContext.h"
#include "Global.h"
#include "Result.h"
#include "SyntaxContainer.h"
#include "SyntaxVisitor.h"
#include "TokenClass.h"
#include "TypeType.h"

#include "../HWLib/RefCountContainer.instance.h"
#include "../HWLib/DumpMacros.h"

using namespace Reni;
static bool Trace = true;


class TypeOperatorSyntax final : public Syntax{
    typedef Syntax baseType;
    typedef TypeOperatorSyntax thisType;
    Ref<Syntax> const target;
public:
    TypeOperatorSyntax(Ref<Syntax> const target, SourcePart const& part)
        :baseType(part)
         , target(target){
        SetDumpString();
    }

    HW_PR_THISREF;
private:
    HW_PR_DECL_GETTER(string, SmartDump) override{
        return
        target->SmartDumpFrame(priority)
        + " type";
    }
    HW_PR_DECL_GETTER(int, priority) override{ return 0; }

    ResultData GetResultData(const Context& context, Category const& category) const override;
};


ResultData TypeOperatorSyntax::GetResultData(const Context& context, Category const& category) const
{
    if(category.hasType)
        return target->GetType(context)->typeType->GetResultDataEmpty(category);
    return context.global->voidType.GetResultDataEmpty(category);
}

HW_PR_VIRTUAL_GETTER_WRAPPER(Syntax, string, SmartDump);
HW_PR_VIRTUAL_GETTER_WRAPPER(Syntax, int, priority);

HW_PR_IMPL_GETTER(Syntax, Array<string>,DumpData)
{
    return{SmartDumpFrame(priority)};
}

Reni::Syntax::Syntax(SourcePart const&part)
    : resultCache([&](Context const*context)
      {
        return new ResultFromSyntaxAndContext(*this, *context);
      })
      , part(part)
{
};

string Syntax::SmartDumpFrame(int priority) const
{
    auto result = SmartDump;
    if(this->priority > priority)
        return result;
    return "(" + result + ")";
};

Size Syntax::GetSize(Context const& context) const
{
    return resultCache(&context)->size;
};

Ref<CodeItem> Syntax::GetCode(Context const& context) const
{
    return resultCache(&context)->code;
};

WeakRef<Type> Syntax::GetType(Context const& context) const
{
    return resultCache(&context)->type;
}

Closure Syntax::GetClosure(Context const& context) const
{
    return resultCache(&context)->closure;
}

Ref<ResultFromSyntaxAndContext> Syntax::GetResultCache(Context const& context) const
{
    return resultCache(&context);
}

Optional<WeakRef<Type>> Syntax::CachedType(RegularContext const& context) const
{
    return resultCache(&context)->cachedType;
};

ResultData Syntax::GetResultData(Context const& context, Category const&category) const
{
    return context
        .global
        ->voidType
        .GetResultDataEmpty(category);
};


Ref<Syntax> Syntax::ReplaceArg(Ref<Syntax> const& arg) const
{
  const SyntaxArgVisitor visitor = arg;
    return Replace(visitor) || Ref<Syntax> (thisRef);
};

Optional<Ref<Syntax>> Syntax::Replace(SyntaxArgVisitor const& visitor) const
{
    HW_D_METHOD(visitor)  ;
    HW_BREAK_AND_THROW;
}

Ref<SyntaxContainer> Syntax::Defines(SourcePart const& part, Ref<Syntax> const& value) const
{
    HW_D_METHOD(part, value)    ;
    HW_BREAK_AND_THROW;
}

void Syntax::AddTo(SyntaxContainer& syntaxContainer) const
{
    HW_D_METHOD(syntaxContainer)  ;
    HW_BREAKPOINT;
}

Ref<Syntax> Syntax::TypeOperator(SourcePart const& part) const
{
    return new TypeOperatorSyntax(thisRef, part);
}

HW_PR_IMPL_GETTER(InfixSyntax, string, SmartDump)
{
    return
        left->SmartDumpFrame(priority)
        + " "
        + string(part)
        + " "
        + right->SmartDumpFrame(priority);
};


HW_PR_IMPL_GETTER(PrefixSyntax, string, SmartDump)
{
    return
        string(part)
        + " "
        + right->SmartDumpFrame(priority);
};


HW_PR_IMPL_GETTER(SuffixSyntax, string, SmartDump)
{
    return
        left->SmartDumpFrame(priority)
        + " "
        + string(part)
        ;
};
