#pragma once
//#pragma message(__FILE__ "(" STRING(__LINE__) "): ")

#include "../HWLib/Ref.h"
#include "../HWLib/RefCountProvider.h"
#include "../HWLib/CtrlRef.h"
#include "../HWLib/DumpableObject.h"
#include "../HWLib/WeakRef.h"
#include "../Util/Category.h"
#include "../Util/Size.h"
#include "Result.h"
#include "SearchTarget.h"
#include "Feature.h"

using namespace HWLib;
using namespace Util;

namespace Reni
{
  class Address;
  class ArrayType;
  class CodeItem;
  class ContextReference;
  class DefineableToken;
  class DefinitionPoint;
  class DumpPrintToken;
  class EnableCutType;
  class Closure;
  class FiberItem;
  class FunctionToken;
  class Global;
  class InstanceToken;
  class NumberType;
  class PlusToken;
  class ResultData;
  class StarToken;
  class TypeType;

  class Type
    : public WithId<DumpableObject, Type>
      , public DeclarationType
  {
    using baseType = WithId<DumpableObject, Type>;
    using thisType = Type;
    struct internal;
    CtrlRef<internal> _internal;
  protected:
    Type();
  public:
    Type(const Type&) = delete;
    ThisRef;

    bool operator==(const Type& other) const { return this == &other; }

    p_virtual(bool, hollow) = 0;
    p_virtual(Size, size) = 0;
    p_virtual(WeakRef<Global>, global) = 0;
    p_virtual_definition(WeakRef<Type>, toTypeTarget);
    p_virtual_function(WeakRef<Type>, toTypeTarget);
    virtual const WeakRef<Type> get_toTypeTarget() const { return thisRef; };
    p_virtual(Address, toAddress);
    p_virtual(bool, isCopyable) { return true; };

    const WeakRef<Type> array(size_t count) const;
    p(WeakRef<NumberType>, numberType);
    p_definition(WeakRef<TypeType>, typeType);
    const WeakRef<TypeType> get_typeType() const;
    p(WeakRef<Type>, indirectType);
    p(WeakRef<EnableCutType>, enableCutType);
    p_virtual(WeakRef<Type>, asFunctionResult);
  protected:
    p_virtual(Optional<WeakRef<NumberType>>, asNumberType) { return {}; };
  public:
    template <class TDestination>
    const Optional<WeakRef<TDestination>> As() const;

    template <>
    const Optional<WeakRef<NumberType>> As() const { return asNumberType; }

    const ResultData GetResultData(Category category, function<Ref<CodeItem>()> getCode,
                                   function<Closure()> getClosure) const;
    const ResultData GetResultDataSmartClosure(Category category, function<Ref<CodeItem>()> getCode) const;
    const ResultData GetResultDataEmpty(Category category) const;

    virtual SearchResult<Feature> DeclarationsForType(const DeclarationType& target) const;
    const WeakRef<NumberType> CreateNumberType() const;
    const WeakRef<Type> IndirectType(int depth) const;
    const WeakRef<Type> Common(const Type& other) const;
    const bool isConvertableTo(const Type& other) const;
    const Ref<ResultCache> ConvertTo(const Type& destination) const;
    virtual Ref<ResultCache> DirectConvert() const;
    const SearchResult<Feature> Declarations(const NumberType& provider) const override;
    const SearchResult<Feature> Declarations(const TypeType&) const override;
    const SearchResult<Feature> Declarations(const EnableCutType&) const override;
    const SearchResult<Feature> Declarations(const AccessType&) const override;
    virtual Array<Ref<FiberItem>> ConvertFiber(const Type& destination) const;
  private:
    p_function(Array<string>, DumpData) override
    {
      return {};
    };
  };


  class EnableCutType final : public Type
  {
    typedef Type baseType;
    typedef EnableCutType thisType;
  public:
    const Type& value;

    EnableCutType(const Type& value) : value(value)
    {
      SetDumpString();
      a_if_(!this->value.hollow);
    }

    ThisRef;

  private:
    p_function(Array<string>, DumpData) override { return {HW_D_VALUE(value)}; };
    p_function(bool, hollow) { return false; };
    p_function(Size, size) override { return value.size; }
    p_function(WeakRef<Global>, global) override { return value.global; }
    p_function(Optional<WeakRef<NumberType>>, asNumberType) override { return value.As<NumberType>(); }
    p_function(Address, toAddress) override;
    SearchResult<Feature> DeclarationsForType(const DeclarationType& target) const override;
  };

  class InstanceFunctionFeature final : public Feature::Extended
  {
    using baseType = Extended;
    using thisType = InstanceFunctionFeature;

    ResultData Result(const Category& category, const Type& target, const Type& arg) const override;
  };
}

//#pragma message(__FILE__ "(" STRING(__LINE__) "): ")
