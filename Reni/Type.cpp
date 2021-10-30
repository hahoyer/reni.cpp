#include "Import.h"
#include "Type.h"

#include "AccessType.h"
#include "Address.h"
#include "AddressType.h"
#include "ArrayType.h"
#include "BitType.h"
#include "CodeItem.h"
#include "ContainerContext.h"
#include "DumpPrintToken.h"
#include "Feature.h"
#include "Fiber.h"
#include "FunctionResultCache.h"
#include "NumberType.h"
#include "Result.h"
#include "SyntaxContainer.h"
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
    FunctionCache<WeakRef<ArrayType>, size_t> array;
    ValueCache<WeakRef<NumberType>> number;
    ValueCache<WeakRef<TypeType>> type;
    ValueCache<WeakRef<AddressType>> indirect;
    ValueCache<WeakRef<EnableCutType>> enableCut;

    explicit internal(Type const&parent)
        : array([&](size_t count)
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
                  return new TypeType(parent.thisRef);
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

p_virtual_header_implementation(Type, Size, size) ;
p_virtual_header_implementation(Type, WeakRef<Global>, global) ;
p_virtual_header_implementation(Type, WeakRef<Type>, asFunctionResult) ;
p_virtual_header_implementation(Type, bool, hllw);
p_virtual_header_implementation(Type, WeakRef<Type>, toTypeTarget);
p_virtual_header_implementation(Type, Address, toAddress);
p_virtual_header_implementation(Type, Optional<WeakRef<NumberType>>, asNumberType);
p_virtual_header_implementation(Type, bool, isCopyable);

SearchResult<Feature> const Type::Declarations(TypeType const&provider) const
{
    md(provider);
    mb;
}

SearchResult<Feature> const Type::Declarations(NumberType const& provider) const
{
    md(provider);
    mb;
};

SearchResult<Feature> const Type::Declarations(EnableCutType const&provider) const
{
    md(provider);
    mb;
}

SearchResult<Feature> const Type::Declarations(AccessType const&provider) const
{
    md(provider);
    mb;
}

Array<Ref<FiberItem>> const Type::ConvertFiber(Type const& destination) const
{
    md(destination);
    mb;
}

ResultData const Type::GetResultData(Category category, function<Ref<CodeItem>()> getCode, function<Externals()> getExts) const
{
    return ResultData::Get(category, l_(hllw), l_(size), getCode, l_(&thisRef), getExts);
}

ResultData const Type::GetResultDataSmartExts(Category category, function<Ref<CodeItem>()> getCode) const
{
    return ResultData::GetSmartHllwExts(category, l_(size), getCode, l_(&thisRef));
}

ResultData const Type::GetResultDataEmpty(Category category) const
{
    a_if(hllw, Dump);
    return ResultData::GetSmartHllwExts(category, l_(size), l_(CodeItem::Const(BitsConst::Empty())), l_(&thisRef));
}

WeakRef<Type> const Type::array(size_t count)const
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

p_implementation(Type, Address, toAddress)
{
    a_if(!hllw, Dump);
    return Address(size,0);
}

p_implementation(Type, WeakRef<Type>, asFunctionResult)
{
    return(thisRef);
}

bool const Type::isConvertableTo(Type const& other) const
{
    return DeclarationsForType(other).IsValid;
}

Ref<ResultCache> const Type::ConvertTo(Type const& destination) const
{
    return new FunctionResultCache
        ([&](Category category) -> ResultData const
            {
                auto result = DeclarationsForType(destination);
                if (result.IsValid)
                    return result
                    .found
                    .ConversionResult(category, thisRef, destination);
                return{};
            }
        );
}

Ref<ResultCache> const Type::DirectConvert() const
{
    md_;
    mb;
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

SearchResult<Feature> const Type::DeclarationsForType(DeclarationType const& target) const
{
    bool Trace = true;
    md(target);
    b_;
    return_d(SearchResult<Feature>());
}

p_implementation(AddressType, Address, toAddress){return value.toAddress + 1;}

SearchResult<Feature> const AddressType::DeclarationsForType(DeclarationType const& target) const
{
    auto result = value.DeclarationsForType(target);
    if(result.IsValid)
        return result;
    return {};
}

Array<Ref<FiberItem>> const AddressType::ConvertFiber(Type const& destination) const
{
    if(destination == value)
        return FiberItem::CopyFromAddress(value);
    return baseType::ConvertFiber(destination);
};


Optional<WeakRef<TypeType>> const TypeType::Convert(Type const& target)
{
    auto result = dynamic_cast<TypeType*>(&target.thisRef);
    if(result)
        return WeakRef<TypeType>(result);
    return{};
}

TypeType::TypeType(Type const& value) 
    : value(value.toTypeTarget)
{
    SetDumpString();
}

SearchResult<Feature> const TypeType::DeclarationsForType(DeclarationType const& target) const
{
    auto result = target.Declarations(*this);
    if(result.IsValid)
        return result;
    return baseType::DeclarationsForType(target);
}



class InstanceFunctionFeature final : public Feature::Extended
{
    using baseType = Extended;
    using thisType = InstanceFunctionFeature;

    ResultData const Result(Category category, Type const&target, Type const&arg)const override
    {
        return arg.ConvertTo(TypeType::Convert(target).Value->value->thisRef)->Get(category);
    }
};


template<>
SearchResult<Feature> const TypeType::DeclarationsForType<InstanceToken>() const
{
    return Feature::From<InstanceFunctionFeature>(*this);
}


p_implementation(EnableCutType, Address, toAddress)
{
    return value.toAddress;
}

SearchResult<Feature> const EnableCutType::DeclarationsForType(DeclarationType const& target) const
{
    return target.Declarations(thisRef);
};

