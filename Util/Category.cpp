#include "Import.h"
#include "Category.h"
#include "..\HWLib\String.h"

static bool Trace = true;

using namespace HWLib;
using namespace Util;

Category const Category::None(false, false, false,false);
Category const Category::Size(true, false, false, false);
Category const Category::Code(false, true, false, false);
Category const Category::Type(false, false, true, false);
Category const Category::Exts(false, false, false, true);

Category const Category::Instance(bool hasSize, bool hasCode, bool hasType, bool hasExts){
    return Category(hasSize, hasCode, hasType, hasExts);
};


Category::Category(bool hasSize, bool hasCode, bool hasType, bool hasExts)
    : hasSize(hasSize)
      , hasCode(hasCode)
      , hasType(hasType)
      , hasExts(hasExts)
{
    SetDumpString();
}

Category::Category()
    : thisType(false, false, false, false){
    SetDumpString();
}

p_implementation(Category, Category, typed){
    return *this | Type;
}

p_implementation(Category, Category, replenished){
    auto result = *this;
    if(result.hasCode)
    {
        result |= Size;
        result |= Exts;
    }

    if(result.hasType)
        result |= Size;

    //if(result.HasSize)
        //result |= IsDataLess;
    return result;
}

Category const Category::operator|(Category const other)const{
    return Category(
        hasSize || other.hasSize,
        hasCode || other.hasCode,
        hasType || other.hasType,
        hasExts|| other.hasExts
    );
}

Category const Category::operator&(Category const other) const
{
    return Category(
        hasSize && other.hasSize,
        hasCode && other.hasCode,
        hasType && other.hasType,
        hasExts && other.hasExts
        );
}

Category const Category::operator-(Category const other)const{
    return Category(
        hasSize && !other.hasSize,
        hasCode && !other.hasCode,
        hasType && !other.hasType,
        hasExts &&!other.hasExts
    );
}

bool Category::operator==(Category const other)const{
    return hasSize == other.hasSize
        && hasCode == other.hasCode
        && hasType == other.hasType
        && hasExts == other.hasExts
        ;
}

bool Category::operator<=(Category const other)const{

    if (hasSize&& !other.hasSize)
        return false;
    if (hasCode && !other.hasCode)
        return false;
    if (hasType && !other.hasType)
        return false;
    if(hasExts && !other.hasExts)
        return false;
    return true;
}

p_implementation(Category, Array<String>, DumpData){
    auto result =
        _({
            hasSize ? String("Size") : "",
            hasCode ? String("Code") : "",
            hasType ? String("Type") : "",
            hasExts ? String("Externals") : ""
        });


    auto r2 = result.Where([](String element){return element != "";})->Stringify(",");
    if (r2=="")
        return{ "<none>" };
    return{r2};
};