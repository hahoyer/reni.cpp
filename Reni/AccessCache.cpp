#include "Import.h"
#include "AccessCache.h"

static bool Trace = true;

using namespace HWLib;
using namespace Reni;

ResultData const AccessCache::GetResultData(Category category) const
{
    md(category);
    mb;
    return{};
}