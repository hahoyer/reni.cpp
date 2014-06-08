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
        p_function(Externals, externals) override{ return{external}; };

        Ref<CodeItem> const ReferencePlus(Size offset) const override;
        Ref<CodeItem, true> const ReplaceImpl(ReplaceVisitor const&arg) const override;
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
    if(category == Category::Type && &external == &External::Function::Arg::Instance)
        return args->indirectType->thisRef;

    if(category == Category::Code)
        return new FunctionCallReferenceCode(thisRef, external);

    md(category, external);
    mb;
    return{};
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

Ref<CodeItem, true> const FunctionCallReferenceCode::ReplaceImpl(ReplaceVisitor const& arg) const
{
    md(arg);
    mb;
    return{};
}
