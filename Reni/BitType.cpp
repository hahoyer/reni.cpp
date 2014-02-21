#include "Import.h"
#include "BitType.h"

#include "BitsConst.h"
#include "Result.h"

static bool Trace = true;

using namespace Reni;

ResultData const BitType::GetResultData(Category category, Ref<BitsConst> value)const{
    md(category, value);
    b_;
    return_d(ResultData());
};
