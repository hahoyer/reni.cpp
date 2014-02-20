#include "Import.h"
#include "Category.h"

static bool Trace = true;

using namespace Reni;

Category const Category::None(false, false);
Category const Category::Code(false, true);

Category::Category(bool hasSize, bool hasCode)
: hasSize(hasSize)
, hasCode(hasCode)
{
    SetDumpString();
}

Category::Category()
: thisType(false, false)
{}

Category const Category::operator+(Category const other)const{
    return Category(
        hasSize && other.hasSize,
        hasCode && other.hasCode
        );
}

Category const Category::operator-(Category const other)const{
    return Category(
        hasSize && !other.hasSize,
        hasCode && !other.hasCode
        );
}
bool Category::operator==(Category const other)const{
    return
        hasSize == other.hasSize
        &&
        hasCode == other.hasCode
        ;
}


override_p_implementation(Category, Array<String>, DumpData){
    auto result =
        _({
        hasSize ? String("Size") : "",
        hasCode ? String("Code") : ""
    });
    
    
    auto r2 = result.Where([](String element){return element != ""; })
    ->ToArray;
        if (r2.Count)
            return r2;
        return{ "<none>" };
};
