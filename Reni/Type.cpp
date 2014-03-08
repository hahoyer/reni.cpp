#include "Import.h"
#include "Type.h"

#include "ArrayType.h"
#include "BitsConst.h"
#include "Code.h"
#include "Feature.h"
#include "FeatureProvider.h"
#include "Size.h"
#include "Result.h"

#include "../HWLib/FunctionCache.h"


using namespace Reni;
static bool Trace = true;

struct Type::internal{
    FunctionCache<int, Ref<ArrayType>> arrayCache;

    explicit internal(Type const&parent)
        : arrayCache([&](int count){return new ArrayType(parent, count); }){};
};


Type::Type() : _internal(new internal(*this)){}

pure_p_implementation(Type, Size, size);

ResultData const Type::GetResultData(Category category, BitsConst const&value)const{
    return ResultData(value.size, CodeItem::Const(value), &this->ref);
};

Ref<Type> const Type::array(int count)const{
    return &_internal->arrayCache[count]->ref;
};

Type::operator Ref<FeatureProvider<DumpPrintToken>, true>()const{ return{}; }

#include "../HWLib/RefCountContainer.instance.h"
