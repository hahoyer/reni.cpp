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
        HW_PR_DECL_GETTER(Array<string>,DumpData) override
        {
            return
                {
                    HW_D_VALUE(context),
                    HW_D_VALUE(external)
                };
        };

        HW_PR_DECL_GETTER(Size, size) override{return Size::Address;}
        HW_PR_DECL_GETTER(Closure, closure) override{ return Closure(external); };

        Ref<CodeItem> ReferencePlus(Size offset) const override;
        Optional<Ref<CodeItem>> ReplaceImpl(ReplaceVisitor const& arg) const override;
        string ToCpp(CodeVisitor const& visitor) const override;
        Ref<CodeItem> Convert(Type const& type) const override;
    };

};


using namespace Reni;
static bool Trace = true;

FunctionCallContext::FunctionCallContext(ContainerContext const& parent, WeakRef<Type const> const arg)
    : baseType(static_cast<RegularContext const&>(parent))
      , container(parent)
      , arg(arg)
      , functionCallResultCache([&](size_t bodyIndex)
          {
              return new FunctionCallResultCache(*this, bodyIndex);
          })
{
    SetDumpString();
}

ResultData const FunctionCallContext::ReferenceResult(Category category, External::Function const& external) const
{
    bool Trace = false;
    HW_D_METHOD(category, external);
    ResultData result = ResultData::GetSmartHollowSizeClosure
        (
            category,
            l_(new FunctionCallReferenceCode(thisRef, external)),
            l_(arg->indirectType)
        );

    return_d(result);
}

HW_PR_IMPL_GETTER(FunctionCallContext, WeakRef<Type>, objectType)
{
    return container.dataType;
}


Ref<CodeItem> FunctionCallReferenceCode::ReferencePlus(Size offset) const
{
    HW_D_METHOD(offset);
    HW_BREAK_AND_THROW;
    return thisRef;
}

string FunctionCallReferenceCode::ToCpp(CodeVisitor const& visitor) const
{
    if(external != External::Function::Arg::Instance)
        return baseType::ToCpp(visitor);
    return "arg";
}

Ref<CodeItem> FunctionCallReferenceCode::Convert(Type const& type) const
{
    HW_D_METHOD(type);
    HW_BREAK_AND_THROW;
}

Optional<Ref<CodeItem>> FunctionCallReferenceCode::ReplaceImpl(ReplaceVisitor const& arg) const
{
    arg.AssumeFunctionArg(external, *context.arg);
    return arg.GetCode(external);
}
