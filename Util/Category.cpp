#include "Import.h"
#include "Category.h"
#include "..\HWLib\String.h"

static bool Trace = true;

using namespace HWLib;
using namespace Util;

Category const Category::None(false, false, false);
Category const Category::Size(true, false, false);
Category const Category::Code(false, true, false);
Category const Category::Type(false, false, true);

Category const Category::Instance(bool hasSize, bool hasCode, bool hasType){
    return Category(hasSize, hasCode, hasType);
};


Category::Category(bool hasSize, bool hasCode, bool hasType)
    : hasSize(hasSize)
      , hasCode(hasCode)
      , hasType(hasType){
    SetDumpString();
}

Category::Category()
    : thisType(false, false, false){
}

p_implementation(Category, Category, typed){
    return *this | Type;
}

Category const Category::operator|(Category const other)const{
    return Category(
        hasSize || other.hasSize,
        hasCode || other.hasCode,
        hasType || other.hasType
    );
}

Category const Category::operator-(Category const other)const{
    return Category(
        hasSize && !other.hasSize,
        hasCode && !other.hasCode,
        hasType && !other.hasType
    );
}

bool Category::operator==(Category const other)const{
    return hasSize == other.hasSize
        && hasCode == other.hasCode
        && hasType == other.hasType
        ;
}

bool Category::operator<=(Category const other)const{

    if (hasSize&& !other.hasSize)
        return false;
    if (hasCode && !other.hasCode)
        return false;
    if (hasType && !other.hasType)
        return false;
    return true;
}

p_implementation(Category, Array<String>, DumpData){
    auto result =
        _({
            hasSize ? String("Size") : "",
            hasCode ? String("Code") : "",
            hasType ? String("Type") : ""
        });


    auto r2 = result.Where([](String element){
            return element != "";
        })
        ->ToArray;
    if (r2.Count)
        return r2;
    return{ "<none>" };
};