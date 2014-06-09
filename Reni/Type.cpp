#include "Import.h"
#include "Type.h"

#include "AddressType.h"
#include "ArrayType.h"
#include "BitType.h"
#include "CodeItem.h"
#include "DumpPrintToken.h"
#include "Feature.h"
#include "NumberType.h"
#include "Result.h"
#include "TypeType.h"
#include "UserDefinedToken.h"

#include "../HWLib/_EditorTemplates.h"
#include "../HWLib/FunctionCache.h"
#include "../HWLib/RefCountContainer.instance.h"
#include "../HWLib/ValueCache.h"


using namespace Reni;
static bool Trace = true;

struct Type::internal
{
    FunctionCache<WeakRef<ArrayType>, int> array;
    ValueCache<WeakRef<NumberType>> number;
    ValueCache<WeakRef<TypeType>> type;
    ValueCache<WeakRef<AddressType>> indirect;
    ValueCache<WeakRef<EnableCutType>> enableCut;

    explicit internal(Type const&parent)
        : array([&](int count)
              {
                  return new ArrayType(parent, count);
              })
          , number([&]
              {
                  return parent.CreateNumberType();
              })
          , indirect([&]
              {
                  return new AddressType(parent.thisRef);
              })
          , type([&]
              {
                  return new TypeType(*parent.dereferencedType);
              })
          , enableCut([&]
              {
                  return new EnableCutType(parent.thisRef);
              })
    {
    };
};


Type::Type() : _internal(new internal(*this))
{
}

pure_p_implementation(Type, Size, size) ;
pure_p_implementation(Type, WeakRef<Global>, global) ;
pure_p_implementation(Type, WeakRef<Type>, asFunctionResult) ;
pure_p_implementation(Type, int, addressLevel);
pure_p_implementation(Type, bool, isTypeTarget);
pure_p_implementation(Type, WeakRef<Type>, dereferencedType);

SearchResult<Feature> const Type::Search(TypeType const&provider) const
{
    md(provider);
    mb;
}

SearchResult<Feature> const Type::Search(NumberType const& provider) const
{
    md(provider);
    mb;
};

ResultData const Type::GetResultData(Category category, function<Ref<CodeItem>()> getCode) const
{
    return ResultData::Get(category,getCode,*this);
}

ResultData const Type::GetResultData(Category category, CodeItem const& code) const
{
    return ResultData::Get(category, code, *this);
}

ResultData const Type::GetResultData(Category category) const
{
    a_if(!HasData, Dump);
    return ResultData::Get(category, l_(CodeItem::Const(BitsConst::Empty())), *this);
}

WeakRef<Type> const Type::array(int count)const
{
    return _internal->array(count)->thisRef;
};

p_implementation(Type, WeakRef<NumberType>, numberType)
{
    return &_internal->number.Value->thisRef;
};

p_implementation(Type, WeakRef<TypeType>, typeType)
{
    return &_internal->type.Value->thisRef;
};

p_implementation(Type, WeakRef<Type>, indirectType)
{
    return &_internal->indirect.Value->thisRef;
};

p_implementation(Type, WeakRef<EnableCutType>, enableCutType)
{
    return &_internal->enableCut.Value->thisRef;
};

p_implementation(Type, WeakRef<Type>, asFunctionResult)
{
    return(thisRef);
}

bool const Type::isConvertableTo(Type const& other) const
{
    return Search(other).IsValid;
}

WeakRef<Type> const Type::Common(Type const& other) const
{
    if(other.isConvertableTo(*this))
        return thisRef;
    if(isConvertableTo(other))
        return other.thisRef;
    md(other);
    mb;
}

ResultData const Type::Constructor(Category category, Type const& arg) const
{
    bool Trace = true;
    md(category, arg);
    b_;
    return_d(ResultData());
}

WeakRef<Type> const Type::IndirectType(int depth) const
{
    a_is(depth, >= , 0);
    if(depth)
        return IndirectType(depth - 1)->indirectType;
    return thisRef;
};

WeakRef<NumberType> const Type::CreateNumberType() const
{
    auto a = dynamic_cast<ArrayType const*>(this);
    a_if(a && dynamic_cast<BitType const*>(&a->elementType), nd(*this) + " cannot be flagged as number type");
    return new NumberType(a->thisRef);
};

ResultData const Type::ContextAccessResult(Category category, Type const& target, function<Size()> getOffset)const
{
    if(!HasData)
        return GetResultData(category);

    return indirectType
        ->GetResultData
        (category,l_(CodeItem::Reference(target)->ReferencePlus(getOffset())));
};

SearchResult<Feature> const Type::Search(SearchTarget const& target) const
{
    bool Trace = true;
    md(target);
    b_;
    return_d(SearchResult<Feature>());
}

SearchResult<Feature> const AddressType::Search(SearchTarget const& target) const
{
    auto result = value.Search(target);
    if(result.IsValid)
        return result;
    return {};
};


WeakPtr<TypeType> const TypeType::Convert(Type const& target)
{
    return dynamic_cast<TypeType*>(&target.thisRef);
}

TypeType::TypeType(Type const& value) 
    : value(value.thisRef)
{
    SetDumpString();
    a_if_(value.isTypeTarget);
}

SearchResult<Feature> const TypeType::Search(SearchTarget const& target) const
{
    auto result = target.Search(*this);
    if(result.IsValid)
        return result;
    return baseType::Search(target);
}



class InstanceFunctionFeature final : public Feature::Extended
{
    using baseType = Extended;
    using thisType = InstanceFunctionFeature;

    ResultData const Result(Category category, Type const&target, Type const&arg)const override
    {
        auto targetType = TypeType::Convert(target)->value;
        if(category <= Category::Type.replenished)
            return targetType->thisRef;
        return targetType->Constructor(category, arg);
    }
};


template<>
SearchResult<Feature> const TypeType::Search<InstanceToken>() const
{
    return Feature::From<InstanceFunctionFeature>();
}

