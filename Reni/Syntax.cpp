#include "Import.h"
#include "Syntax.h"

#include "Code.h"
#include "Context.h"
#include "FeatureProvider.h"
#include "Result.h"
#include "TokenClass.h"
#include "SyntaxContainer.h"
#include "SyntaxVisitor.h"
#include "../HWLib/RefCountContainer.instance.h"

using namespace Reni;
static bool Trace = true;


Reni::Syntax::Syntax(SourcePart const&part)
    : part(part)
      , resultCache([&](Context const*context){
          return new ResultFromSyntaxAndContext(*this, *context);
      }){
};

Ref<CodeItem> const Syntax::Code(Context const&context)const{
    return resultCache(&context)->code;
}

WeakRef<Type>const Syntax::Type(Context const&context)const{
    return resultCache(&context)->type;
}

Ref<ResultFromSyntaxAndContext> const Syntax::GetResultCache(Context const&context)const {
    Ref<ResultFromSyntaxAndContext> r = resultCache(&context);
    return r;
}

ResultData const Syntax::GetResultData(Context const&context, Category category)const{
    md(context, category);
    b_;
    return_d(ResultData());
}


Ref<Syntax> const Syntax::ReplaceArg(Ref<Syntax> const&arg)const{
    SyntaxArgVisitor visitor = arg;
    return Replace(visitor) || Ref<Syntax> (thisRef);
}

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
