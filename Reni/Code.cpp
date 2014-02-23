#include "Import.h"
#include "Code.h"

#include "BitsConst.h"
#include "CodeItems.h"
#include "Size.h"

using namespace Reni;
static bool Trace = true;

pure_p_implementation(CodeItem, String, ToCpp);

Ref<CodeItem> const CodeItem::Const(Ref<BitsConst> const&value){
    return new ConstCode(value->size, value);
};

override_p_implementation(ConstCode, String, ToCpp){
    return "push(" + size.format + ", " + value->format + ");\n";
}
