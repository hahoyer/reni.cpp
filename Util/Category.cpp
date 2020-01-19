#include "Import.h"
#include "Category.h"
#include "..\HWLib\String.h"

static bool Trace = true;

using namespace HWLib;
using namespace Util;

Category const Category::None(false, false, false, false, false);
Category const Category::Hllw(true, true, false, false, false);
Category const Category::Size(false, true, false, false, false);
Category const Category::Code(false, false, true, false, false);
Category const Category::Type(false, false, false, true, false);
Category const Category::Exts(false, false, false, false, true);

Category const Category::Instance(bool hasHllw, bool hasSize, bool hasCode, bool hasType, bool hasExts)
{
    return Category(hasHllw, hasSize, hasCode, hasType, hasExts);
};

Category::Category(Category const& other)
    : thisType(other.hasHllw, other.hasSize, other.hasCode, other.hasType, other.hasExts)
{
};

Category::Category(bool hasHllw, bool hasSize, bool hasCode, bool hasType, bool hasExts)
    : hasHllw(hasHllw)
    , hasSize(hasSize)
    , hasCode(hasCode)
    , hasType(hasType)
    , hasExts(hasExts)
{
    SetDumpString();
};

Category::Category()
    : thisType(false, false, false, false, false)
{
};

p_implementation(Category, Category, typed){return *this | Type;}

p_implementation(Category, Category, replenished)
{
    auto result = *this;
    if (result.hasCode)
    {
        result |= Hllw;
        result |= Size;
        result |= Exts;
    }

    if (result.hasType)
    {
        result |= Hllw;
        result |= Size;
    }

    if (result.hasSize)
        result |= Hllw;

    return result;
}

Category const Category::operator|(Category const other)const
{
    return Category(
        hasHllw || other.hasHllw,
        hasSize || other.hasSize,
        hasCode || other.hasCode,
        hasType || other.hasType,
        hasExts || other.hasExts
    );
}

Category const Category::operator&(Category const other) const
{
    return Category(
        hasHllw && other.hasHllw,
        hasSize && other.hasSize,
        hasCode && other.hasCode,
        hasType && other.hasType,
        hasExts && other.hasExts
    );
}

Category const Category::operator-(Category const other)const
{
    return Category(
        hasHllw && !other.hasHllw,
        hasSize && !other.hasSize,
        hasCode && !other.hasCode,
        hasType && !other.hasType,
        hasExts && !other.hasExts
    );
}

bool Category::operator==(Category const other)const
{
    return hasHllw == other.hasHllw
        && hasSize == other.hasSize
        && hasCode == other.hasCode
        && hasType == other.hasType
        && hasExts == other.hasExts
        ;
}

bool Category::operator<=(Category const other)const
{
    if(hasHllw && !other.hasHllw)
        return false;
    if(hasSize && !other.hasSize)
        return false;
    if (hasCode && !other.hasCode)
        return false;
    if (hasType && !other.hasType)
        return false;
    if (hasExts && !other.hasExts)
        return false;
    return true;
}

p_implementation(Category, Array<String>, DumpData)
{
    auto result =
        _({
            hasHllw ? String("Hllw") : "",
            hasSize ? String("Size") : "",
            hasCode ? String("Code") : "",
            hasType ? String("Type") : "",
            hasExts ? String("Exts") : ""
        });


    auto r2 = result.Where([](String element)
        {
            return element != "";
        })->Stringify(",");
    if (r2 == "")
        return{"<none>"};
    return{r2};
};

