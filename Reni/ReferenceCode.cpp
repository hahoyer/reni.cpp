#include "Import.h"
#include "ReferenceCode.h"

#include "CodeItems.h"
#include "ContainerContext.h"
#include "ReplaceVisitor.h"
#include "SyntaxContainer.h"
#include "../HWLib/RefCountContainer.instance.h"


using namespace Reni;
using namespace HWLib;

static bool Trace = true;




ReferenceCode::ReferenceCode(ContainerContext const& value) : value(value)
{
    SetDumpString();
}

p_implementation(ReferenceCode, Array<string>,DumpData) { return{HW_D_VALUE(value)}; };
p_implementation(ReferenceCode, Closure, closure){return Closure(value.asExternal);}

inline Ref<CodeItem> ReferenceCode::ReferencePlus(Size offset) const
{
    if(offset == 0)
        return thisRef;
    return (thisRef + *new ReferencePlusCode(size, offset))
        ->thisRef;
}

Optional<Ref<CodeItem>> ReferenceCode::ReplaceImpl(ReplaceVisitor const& visitor) const
{
    return visitor.GetCode(value.asExternal);
}


std::string AssignCode::ToCpp(CodeVisitor const& visitor) const
{
    md(visitor);
    mb;
}
