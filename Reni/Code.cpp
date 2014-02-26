#include "Import.h"
#include "Code.h"

#include "BitsConst.h"
#include "CodeItems.h"
#include "CodeVisitor.h"
#include "Size.h"
#include "..\HWLib\String.h"
#include "..\HWLib\DumpMacros.h"
#include "..\HWLib\BreakHandling.h"

using namespace Reni;
using namespace HWLib;

static bool Trace = true;

String const CodeItem::ToCpp(CodeVisitor const& visitor)const{
    md(visitor);
    b_;
    return "";
}

Ref<CodeItem> const CodeItem::Const(Ref<BitsConst> const&value){
    return new ConstCode(value->size, value);
};

String const ConstCode::ToCpp(CodeVisitor const& visitor)const{
    return visitor.Const(size, value);
}
