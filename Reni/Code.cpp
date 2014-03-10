#include "Import.h"
#include "Code.h"

#include "BitsConst.h"
#include "CodeItems.h"
#include "CodeVisitor.h"
#include "FeatureProvider.h"
#include "ReplaceVisitor.h"
#include "Result.h"
#include "Size.h"
#include "../HWLib/RefCountContainer.instance.h"

using namespace Reni;
using namespace HWLib;

static bool Trace = true;


String const CodeItem::ToCpp(CodeVisitor const& visitor)const{
    md(visitor);
    mb;
};

pure_p_implementation(CodeItem, Size, size) ;

Ref<CodeItem> const CodeItem::Const(BitsConst const&value){
    return new ConstCode(value.size, value);
};

Ref<CodeItem> const CodeItem::DumpPrintNumber(ArrayType const&value){
    return Arg(value)
        ->Fiber({new DumpPrintNumberCode(value.size)});
};

Ref<CodeItem> const CodeItem::Arg(Type const&value){
    return new ArgCode(value);
};

Ref<CodeItem> const CodeItem::Replace(ReplaceVisitor const&arg) const{
    md(arg);
    mb;
};

Ref<CodeItem> const CodeItem::Fiber(Array<Ref<FiberItem>> const&items)const{
    md(items);
    mb;
}


String const ConstCode::ToCpp(CodeVisitor const& visitor)const{
    return visitor.Const(size, value);
};


String const DumpPrintNumberCode::ToCpp(CodeVisitor const& visitor)const{
    md(visitor);
    mb;
};


String const ArgCode::ToCpp(CodeVisitor const& visitor)const{
    md(visitor);
    mb;
};
