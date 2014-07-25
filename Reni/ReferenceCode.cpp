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

p_implementation(ReferenceCode, Array<String>, DumpData) { return{nd(value)}; };
p_implementation(ReferenceCode, Externals, exts){return Externals(value.asExternal);}

inline Ref<CodeItem> const ReferenceCode::ReferencePlus(Size offset) const
{
    if(offset == 0)
        return thisRef;
    return (thisRef + *new ReferencePlusCode(size, offset))
        ->thisRef;
}

Optional<Ref<CodeItem>> const ReferenceCode::ReplaceImpl(ReplaceVisitor const&visitor) const
{
    return visitor.GetCode(value.asExternal);
}


String const AssignCode::ToCpp(CodeVisitor const& visitor) const
{
    md(visitor);
    mb;
}
