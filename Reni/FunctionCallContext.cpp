#include "Import.h"
#include "FunctionCallContext.h"

#include "ContainerContext.h"
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
    };

};


using namespace Reni;
static bool Trace = true;

int FunctionCallContext::nextIndex = 0;

FunctionCallContext::FunctionCallContext(ContainerContext const& parent, WeakRef<Type const> const args)
    : baseType(static_cast<RegularContext const&>(parent))
      , container(parent)
      , args(args)
      , index(nextIndex++)
{
    SetDumpString();
}

ResultData const FunctionCallContext::ReferenceResult(Category category, External::Function const& external) const
{
    bool Trace = false;
    md(category, external);
    ResultData result = ResultData::Get
        (
            category,
            l_(new FunctionCallReferenceCode(thisRef, external)),
            l_(&args->indirectType->thisRef),
            l_(Externals(external))
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

Optional<Ref<CodeItem>> const FunctionCallReferenceCode::ReplaceImpl(ReplaceVisitor const& arg) const
{
    md(arg);
    mb;
    return{};
}
