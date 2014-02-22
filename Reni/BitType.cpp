#include "Import.h"
#include "BitType.h"

#include "BitsConst.h"
#include "Result.h"

static bool Trace = true;

using namespace Reni;

ResultData const BitType::GetResultData(Category category, Ref<BitsConst> const&value)const{
    if (category == Category::Code)
        return CodeItem::Const(value);

    md(category, value);
    b_;
    return_d(ResultData());
};
