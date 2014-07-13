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

    ThisRef;
private:
    p_function(String, SmartDump) override{
        return
        target->SmartDumpFrame(priority)
        + " type";
    }
    p_function(int, priority) override{ return 0; }

    ResultData const GetResultData(Context const& context, Category category) const override;
};


ResultData const TypeOperatorSyntax::GetResultData(Context const& context, Category category) const
{
    if(category.hasType)
        return target->Type(context)->typeType->GetResultDataEmpty(category);
    return context.global->voidType.GetResultDataEmpty(category);
}

p_virtual_header_implementation(Syntax, String, SmartDump);
p_virtual_header_implementation(Syntax, int, priority);

p_implementation(Syntax, Array<String>, DumpData) 
{
    return{SmartDumpFrame(priority)};
}

Reni::Syntax::Syntax(SourcePart const&part)
    : part(part)
      , resultCache([&](Context const*context)
          {
              return new ResultFromSyntaxAndContext(*this, *context);
          })
{
};

String const Syntax::SmartDumpFrame(int priority) const
{
    auto result = SmartDump;
    if(this->priority > priority)
        return result;
    return "(" + result + ")";
};

Size const Syntax::Size(Context const& context) const
{
    return resultCache(&context)->size;
};

Ref<CodeItem> const Syntax::Code(Context const&context)const
{
    return resultCache(&context)->code;
};

WeakRef<Type>const Syntax::Type(Context const&context)const
{
    return resultCache(&context)->type;
}

Externals const Syntax::Exts(Context const& context) const
{
    return resultCache(&context)->exts;
}

Ref<ResultFromSyntaxAndContext> const Syntax::GetResultCache(Context const&context)const
{
    return resultCache(&context);
}

WeakPtr<Type> const Syntax::CachedType(RegularContext const& context) const
{
    return resultCache(&context)->cachedType;
};

ResultData const Syntax::GetResultData(Context const&context, Category category)const
{
    return context
        .global
        ->voidType
        .GetResultDataEmpty(category);
};


Ref<Syntax> const Syntax::ReplaceArg(Ref<Syntax> const&arg)const
{
    SyntaxArgVisitor visitor = arg;
    return Replace(visitor) || Ref<Syntax> (thisRef);
};

Optional<Ref<Syntax>> const Syntax::Replace(SyntaxArgVisitor const&visitor) const
{
    md(visitor)  ;
    mb;
}

Ref<SyntaxContainer> const Syntax::Defines(SourcePart const& part, Ref<Syntax> const&value) const
{
    md(part, value)    ;
    mb;
}

void Syntax::AddTo(SyntaxContainer& syntaxContainer) const
{
    md(syntaxContainer)  ;
    b_;
}

Ref<Syntax> const Syntax::TypeOperator(SourcePart const part) const
{
    return new TypeOperatorSyntax(thisRef, part);
}

p_implementation(InfixSyntax, String, SmartDump)
{
    return
        left->SmartDumpFrame(priority)
        + " "
        + String(part)
        + " "
        + right->SmartDumpFrame(priority);
};


p_implementation(PrefixSyntax, String, SmartDump)
{
    return
        String(part)
        + " "
        + right->SmartDumpFrame(priority);
};


p_implementation(SuffixSyntax, String, SmartDump)
{
    return
        left->SmartDumpFrame(priority)
        + " "
        + String(part)
        ;
};
