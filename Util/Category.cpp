#include "Import.h"
#include "Category.h"
#include "..\HWLib\string.h"

static bool Trace = true;

using namespace HWLib;
using namespace Util;

Category const Category::None(false, false, false, false, false);
Category const Category::Hollow(true, true, false, false, false);
Category const Category::Size(false, true, false, false, false);
Category const Category::Code(false, false, true, false, false);
Category const Category::Type(false, false, false, true, false);
Category const Category::Closure(false, false, false, false, true);

Category Category::Instance(bool hasHollow, bool hasSize, bool hasCode, bool hasType, bool hasClosure)
{
    return Category(hasHollow, hasSize, hasCode, hasType, hasClosure);
};

Category::Category(Category const& other)
    : thisType(other.hasHollow, other.hasSize, other.hasCode, other.hasType, other.hasClosure)
{
};

Category::Category(bool hasHollow, bool hasSize, bool hasCode, bool hasType, bool hasClosure)
    : hasHollow(hasHollow)
    , hasSize(hasSize)
    , hasCode(hasCode)
    , hasType(hasType)
    , hasClosure(hasClosure)
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
        result |= Hollow;
        result |= Size;
        result |= Closure;
    }

    if (result.hasType)
    {
        result |= Hollow;
        result |= Size;
    }

    if (result.hasSize)
        result |= Hollow;

    return result;
}

Category Category::operator|(Category const& other) const
{
    return Category(
        hasHollow || other.hasHollow,
        hasSize || other.hasSize,
        hasCode || other.hasCode,
        hasType || other.hasType,
        hasClosure || other.hasClosure
    );
}

Category Category::operator&(Category const& other) const
{
    return Category(
        hasHollow && other.hasHollow,
        hasSize && other.hasSize,
        hasCode && other.hasCode,
        hasType && other.hasType,
        hasClosure && other.hasClosure
    );
}

Category Category::operator-(Category const& other) const
{
    return Category(
        hasHollow && !other.hasHollow,
        hasSize && !other.hasSize,
        hasCode && !other.hasCode,
        hasType && !other.hasType,
        hasClosure && !other.hasClosure
    );
}

bool Category::operator==(Category const& other)const
{
    return hasHollow == other.hasHollow
        && hasSize == other.hasSize
        && hasCode == other.hasCode
        && hasType == other.hasType
        && hasClosure == other.hasClosure
        ;
}

bool Category::operator<=(Category const& other)const
{
    if(hasHollow && !other.hasHollow)
        return false;
    if(hasSize && !other.hasSize)
        return false;
    if (hasCode && !other.hasCode)
        return false;
    if (hasType && !other.hasType)
        return false;
    if (hasClosure && !other.hasClosure)
        return false;
    return true;
}

p_implementation(Category, Array<string>, DumpData)
{
  const auto result =
        _({
            hasHollow ? string("Hollow") : "",
            hasSize ? string("Size") : "",
            hasCode ? string("Code") : "",
            hasType ? string("Type") : "",
            hasClosure ? string("Closure") : ""
        });


    auto r2 = result.Where([](string element)
        {
            return element != "";
        })->Stringify(",");
    if (r2 == "")
        return{"<none>"};
    return{r2};
};

