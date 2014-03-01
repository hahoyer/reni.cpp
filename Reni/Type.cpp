#include "Import.h"
#include "Type.h"

#include "BitsConst.h"
#include "Result.h"
#include "SearchResult.h"
#include "..\HWLib\FunctionCache.h"

using namespace Reni;
static bool Trace = true;

namespace Reni{
    class ArrayType final : public Type{
        using baseType = Type;
        using thisType = ArrayType;
        Type const& elementType;
        int count;
    public:
        ArrayType(Type const& elementType, int count)
            : elementType(elementType)
            , count(count){}
    private:
        override_p_function(Size, size){
            return elementType.size * count;
        };
    };
};


struct Type::internal{
    FunctionCache<int, Ref<ArrayType>> arrayCache;

    internal(Type const&parent)
        : arrayCache([&](int count){return new ArrayType(parent, count); }){};
};


Type::Type() : _internal(new internal(*this)){}

pure_p_implementation(Type, Size, size);

SearchResult const Type::Search(TokenClass const&tokenClass)const{
    md(tokenClass);
    b_;
    return{};
}

ResultData const Type::GetResultData(Category category, BitsConst const&value)const{


    if (category == Category::Code)
        return CodeItem::Const(value);
    if (category == Category::Type)
        return const_cast<Type*>(this);

    md(category, value);
    b_;
    return_d(ResultData());
};

Type const& Type::array(int count)const{
    return *_internal->arrayCache[count];
}