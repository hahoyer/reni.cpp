#include "Import.h"
#include "NumberType.h"

#include "AddressType.h"
#include "ArrayType.h"
#include "CodeItems.h"
#include "DefineableToken.h"
#include "ExpressionSyntax.h"
#include "Feature.h"
#include "Context.h"
#include "Global.h"
#include "ReplaceVisitor.h"
#include "Result.h"
#include "VoidType.h"

#include "../HWLib/RefCountContainer.instance.h"


namespace Reni
{
    class NumberConversionProvider final : public Feature::Extended
    {
        using baseType = Extended;
        using thisType = NumberConversionProvider;

        ResultData const Result(Category category, Type const& target, Type const& destination) const override
        {
            auto const& targetBase = *target.As<NumberType>();
            auto const& destinationBase = *destination.As<NumberType>();


            bool Trace = true;
            md(category, target, destination, targetBase, destinationBase);
            b_;
            return{};
        }
    };
}


using namespace Reni;
static bool Trace = true;

NumberType::NumberType(WeakRef<ArrayType> const parent)
    : parent(*parent)
{
    SetDumpString();
}

p_implementation(NumberType, Size, size){return parent.size;};
p_implementation(NumberType, WeakRef<Global>, global){return parent.global;};
p_implementation(NumberType, Array<String>, DumpData){ return{nd(parent)}; };


p_implementation(NumberType, String, DumpShort)
{
    return base_p_name(DumpShort) + " size=" + size.DumpShort;
}

SearchResult<Feature> const NumberType::DeclarationsForType(DeclarationType const& target) const
{
    return target.Declarations(*this);
};

SearchResult<Feature> const NumberType::Declarations(NumberType const& provider) const
{
    if (size < provider.size)
        return{};
    return Feature::From<NumberConversionProvider>();
}

SearchResult<Feature> const NumberType::Declarations(EnableCutType const& ) const
{
    return Feature::From<NumberConversionProvider>();
}

WeakRef<NumberType> const NumberType::Resize(int newSize)const
{
    return parent
        .elementType
        .array(newSize)
        ->numberType
        ->thisRef;
}