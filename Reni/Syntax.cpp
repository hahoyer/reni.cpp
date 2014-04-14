#include "Import.h"
#include "Syntax.h"

#include "Code.h"
#include "Context.h"
#include "FeatureProvider.h"
#include "Global.h"
#include "Result.h"
#include "TokenClass.h"
#include "SyntaxContainer.h"
#include "SyntaxVisitor.h"
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
    p_function(Array<String>, DumpData) override{
        return{
                nd(target)
            };
    }

    ResultData const GetResultData(Context const& context, Category category) const override;
};


ResultData const TypeOperatorSyntax::GetResultData(Context const& context, Category category) const{
    auto result = context.global->voidType.GetResultData() & category;
    if(category.hasType)
        result = result .With(*target->Type(context)->typeType);
    return result;
}

Reni::Syntax::Syntax(SourcePart const&part)
    : part(part)
      , resultCache([&](Context const*context){
          return new ResultFromSyntaxAndContext(*this, *context);
      }){
};

Size const Syntax::Size(Context const& context) const{
    return resultCache(&context)->size;
};

Ref<CodeItem> const Syntax::Code(Context const&context)const{
    return resultCache(&context)->code;
};

WeakRef<Type>const Syntax::Type(Context const&context)const{
    return resultCache(&context)->type;
};

Ref<ResultFromSyntaxAndContext> const Syntax::GetResultCache(Context const&context)const {
    Ref<ResultFromSyntaxAndContext> r = resultCache(&context);
    return r;
};

ResultData const Syntax::GetResultData(Context const&context, Category category)const{
    md(context, category);
    b_;
    return_d(ResultData());
};


Ref<Syntax> const Syntax::ReplaceArg(Ref<Syntax> const&arg)const{
    SyntaxArgVisitor visitor = arg;
    return Replace(visitor) || Ref<Syntax> (thisRef);
};

Ref<Syntax,true> const Syntax::Replace(SyntaxArgVisitor const&visitor) const{
    md(visitor);
    mb;
}

Ref<SyntaxContainer> const Syntax::Defines(SourcePart const& part, Ref<Syntax> const&value) const{
    md(part, value);
    mb;
}

void Syntax::AddTo(SyntaxContainer& syntaxContainer) const{
    md(syntaxContainer);
    b_;
}

Ref<Syntax> const Syntax::TypeOperator(SourcePart const part) const{
    return new TypeOperatorSyntax(thisRef, part);
}

p_implementation(InfixSyntax, Array<String>, DumpData){
    return {
        nd(left),
        nd(tokenClass),
        nd(right)
    };
};


p_implementation(PrefixSyntax, Array<String>, DumpData){
    return {
        nd(tokenClass),
        nd(right)
    };
};


p_implementation(SuffixSyntax, Array<String>, DumpData){
    return {
        nd(left),
        nd(tokenClass)
    };
};
