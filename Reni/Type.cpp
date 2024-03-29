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
#include "TypeType.h"
#include "UserDefinedToken.h"

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

  explicit internal(Type const& parent)
    : array([&](size_t count)
      {
        return new ArrayType(parent, count);
      })
      , number([&]
      {
        return parent.CreateNumberType();
      })
      , type([&]
      {
        return new TypeType(parent.thisRef);
      })
      , indirect([&]
      {
        return new AddressType(parent.thisRef);
      })
      , enableCut([&]
      {
        return new EnableCutType(parent.thisRef);
      })
  { };
};


Type::Type() : _internal(new internal(*this))
{}

HW_PR_VIRTUAL_GETTER_WRAPPER(Type, Size, size) ;
HW_PR_VIRTUAL_GETTER_WRAPPER(Type, WeakRef<Global>, global) ;
HW_PR_VIRTUAL_GETTER_WRAPPER(Type, WeakRef<Type>, asFunctionResult) ;
HW_PR_VIRTUAL_GETTER_WRAPPER(Type, bool, hollow);
HW_PR_VIRTUAL_GETTER_WRAPPER(Type, WeakRef<Type>, toTypeTarget);
HW_PR_VIRTUAL_GETTER_WRAPPER(Type, Address, toAddress);
HW_PR_VIRTUAL_GETTER_WRAPPER(Type, Optional<WeakRef<NumberType>>, asNumberType);
HW_PR_VIRTUAL_GETTER_WRAPPER(Type, bool, isCopyable);

SearchResult<Feature> const Type::Declarations(TypeType const& provider) const
{
  HW_D_METHOD(provider);
  HW_BREAK_AND_THROW;
}

SearchResult<Feature> const Type::Declarations(NumberType const& provider) const
{
  HW_D_METHOD(provider);
  HW_BREAK_AND_THROW;
};

SearchResult<Feature> const Type::Declarations(EnableCutType const& provider) const
{
  HW_D_METHOD(provider);
  HW_BREAK_AND_THROW;
}

SearchResult<Feature> const Type::Declarations(AccessType const& provider) const
{
  HW_D_METHOD(provider);
  HW_BREAK_AND_THROW;
}

Array<Ref<FiberItem>> Type::ConvertFiber(Type const& destination) const
{
  HW_D_METHOD(destination);
  HW_BREAK_AND_THROW;
}

ResultData Type::GetResultData(Category category, function<Ref<CodeItem>()> getCode,
                               function<Closure()> getClosure) const
{
  return ResultData::Get(category, l_(hollow), l_(size), getCode, l_(&thisRef), getClosure);
}

ResultData Type::GetResultDataSmartClosure(Category category, function<Ref<CodeItem>()> getCode) const
{
  return ResultData::GetSmartHollowClosure(category, l_(size), getCode, l_(&thisRef));
}

ResultData Type::GetResultDataEmpty(Category category) const
{
  HW_ASSERT(hollow, Dump);
  return ResultData::GetSmartHollowClosure(category, l_(size), l_(CodeItem::Const(BitsConst::Empty())), l_(&thisRef));
}

WeakRef<Type> Type::array(size_t count) const
{
  return _internal->array(count)->thisRef;
};

HW_PR_IMPL_GETTER(Type, WeakRef<NumberType>, numberType)
{
  return &_internal->number.Value->thisRef;
};

HW_PR_IMPL_GETTER(Type, WeakRef<TypeType>, typeType)
{
  return &_internal->type.Value->thisRef;
};

HW_PR_IMPL_GETTER(Type, WeakRef<Type>, indirectType)
{
  return &_internal->indirect.Value->thisRef;
};

HW_PR_IMPL_GETTER(Type, WeakRef<EnableCutType>, enableCutType)
{
  return &_internal->enableCut.Value->thisRef;
};

HW_PR_IMPL_GETTER(Type, Address, toAddress)
{
  HW_ASSERT(!hollow, Dump);
  return Address(size, 0);
}

HW_PR_IMPL_GETTER(Type, WeakRef<Type>, asFunctionResult)
{
  return thisRef;
}

bool const Type::isConvertableTo(Type const& other) const
{
  return DeclarationsForType(other).IsValid;
}

Ref<ResultCache> const Type::ConvertTo(Type const& destination) const
{
  return new FunctionResultCache([&](Category category) -> ResultData const
    {
      auto result = DeclarationsForType(destination);
      if(result.IsValid)
        return result
               .found
               .ConversionResult(category, thisRef, destination);
      return {};
    }
  );
}

Ref<ResultCache> Type::DirectConvert() const
{
  HW_D_METHOD_;
  HW_BREAK_AND_THROW;
}

WeakRef<Type> const Type::Common(Type const& other) const
{
  if(other.isConvertableTo(*this))
    return thisRef;
  if(isConvertableTo(other))
    return other.thisRef;
  HW_D_METHOD(other);
  HW_BREAK_AND_THROW;
}

WeakRef<Type> const Type::IndirectType(int depth) const
{
  HW_ASSERT_IS(depth, >=, 0);
  if(depth)
    return IndirectType(depth - 1)->indirectType;
  return thisRef;
};

WeakRef<NumberType> const Type::CreateNumberType() const
{
  auto a = dynamic_cast<ArrayType const*>(this);
  HW_ASSERT(a && dynamic_cast<BitType const*>(&a->elementType),
            HW_D_VALUE(*this) + " cannot be flagged as number type");
  return new NumberType(a->thisRef);
};

SearchResult<Feature> Type::DeclarationsForType(DeclarationType const& target) const
{
  bool Trace = true;
  HW_D_METHOD(target);
  HW_BREAKPOINT;
  return_d(SearchResult<Feature>());
}

HW_PR_IMPL_GETTER(AddressType, Address, toAddress) { return value.toAddress + 1; }

SearchResult<Feature> AddressType::DeclarationsForType(DeclarationType const& target) const
{
  auto result = value.DeclarationsForType(target);
  if(result.IsValid)
    return result;
  return {};
}

Array<Ref<FiberItem>> AddressType::ConvertFiber(Type const& destination) const
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
  return {};
}

TypeType::TypeType(Type const& value)
  : value(value.toTypeTarget)
{
  SetDumpString();
}

SearchResult<Feature> TypeType::DeclarationsForType(DeclarationType const& target) const
{
  auto result = target.Declarations(*this);
  if(result.IsValid)
    return result;
  return baseType::DeclarationsForType(target);
}

HW_PR_IMPL_GETTER(EnableCutType, Address, toAddress)
{
  return value.toAddress;
}

SearchResult<Feature> EnableCutType::DeclarationsForType(DeclarationType const& target) const
{
  return target.Declarations(thisRef);
}

ResultData Reni::InstanceFunctionFeature::Result(const Category& category, const Type& target, const Type& arg) const
{
  return arg.ConvertTo(TypeType::Convert(target).Value->value->thisRef)->Get(category);
};
