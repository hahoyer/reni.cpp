#include "Import.h"
#include "FunctionCallContext.h"

#include "ContainerContext.h"
#include "ContextIncludes.h"
#include "FunctionCallResultCache.h"
#include "Syntax.h"
#include "ReplaceVisitor.h"
#include "SyntaxContainer.h"

#include "../HWLib/RefCountContainer.instance.h"

namespace Reni
{
    class FunctionCallReferenceCode final : public CodeItem
    {
        typedef CodeItem baseType;
        typedef FunctionCallReferenceCode thisType;
        FunctionCallContext const&context;
        External::Function const& external;
    public:
        FunctionCallReferenceCode(FunctionCallContext const&context, External::Function const& external)
            : context(context)
              , external(external)
        {
            SetDumpString();
        }

    private:
        p_function(Array<String>, DumpData) override
        {
            return
                {
                    nd(context),
                    nd(external)
                };
        };

        p_function(Size, size) override{return Size::Address;}
        p_function(Externals, exts) override{ return Externals(external); };

        Ref<CodeItem> const ReferencePlus(Size offset) const override;
        Optional<Ref<CodeItem>> const ReplaceImpl(ReplaceVisitor const&arg) const override;
        String const ToCpp(CodeVisitor const& visitor) const override;
        Ref<CodeItem> const Convert(Type const& type) const override;
    };

};


using namespace Reni;
static bool Trace = true;

FunctionCallContext::FunctionCallContext(ContainerContext const& parent, WeakRef<Type const> const arg)
    : baseType(static_cast<RegularContext const&>(parent))
      , container(parent)
      , arg(arg)
      , functionCallResultCache([&](int bodyIndex)
          {
              return new FunctionCallResultCache(*this, bodyIndex);
          })
{
    SetDumpString();
}

ResultData const FunctionCallContext::ReferenceResult(Category category, External::Function const& external) const
{
    bool Trace = false;
    md(category, external);
    ResultData result = ResultData::GetSmartHllwSizeExts
        (
            category,
            l_(new FunctionCallReferenceCode(thisRef, external)),
            l_(arg->indirectType)
        );

    return_d(result);
}

p_implementation(FunctionCallContext, WeakRef<Type>, objectType)
{
    return container.dataType;
}


Ref<CodeItem> const FunctionCallReferenceCode::ReferencePlus(Size offset) const
{
    md(offset);
    mb;
    return thisRef;
}

String const FunctionCallReferenceCode::ToCpp(CodeVisitor const& visitor) const
{
    if(external != External::Function::Arg::Instance)
        return baseType::ToCpp(visitor);
    return "arg";
}

Ref<CodeItem> const FunctionCallReferenceCode::Convert(Type const& type) const
{
    md(type);
    mb;
}

Optional<Ref<CodeItem>> const FunctionCallReferenceCode::ReplaceImpl(ReplaceVisitor const& arg) const
{
    arg.AssumeFunctionArg(external, *context.arg);
    return arg.GetCode(external);
}
