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
    HW_PR_THISREF;

    bool operator==(const Type& other) const { return this == &other; }

    HW_PR_VIRTUAL_GET(bool, hollow) = 0;

  public:
    HW_PR_VIRTUAL_GET(Size, size) = 0;

  public:
    HW_PR_VIRTUAL_GET(WeakRef<Global>, global) = 0;

  public:
    HW_PR_VIRTUAL_GET(WeakRef<Type>, toTypeTarget) { return thisRef; };

  public:
    HW_PR_VIRTUAL_GET(Address, toAddress);

  public:
    HW_PR_VIRTUAL_GET(bool, isCopyable) { return true; };

  public:
    WeakRef<Type> array(size_t count) const;
    HW_PR_GET(WeakRef<NumberType>, numberType);
    HW_PR_GET(WeakRef<TypeType>, typeType);
    HW_PR_GET(WeakRef<Type>, indirectType);
    HW_PR_GET(WeakRef<EnableCutType>, enableCutType);
    HW_PR_VIRTUAL_GET(WeakRef<Type>, asFunctionResult);

  public:
    HW_PR_VIRTUAL_GET(Optional<WeakRef<NumberType>>, asNumberType) { return {}; };

  public:
    template <class TDestination>
    Optional<WeakRef<TDestination>> As() const;

    template <>
    Optional<WeakRef<NumberType>> As() const { return asNumberType; }

    ResultData GetResultData(Category category, function<Ref<CodeItem>()> getCode,
                             function<Closure()> getClosure) const;
    ResultData GetResultDataSmartClosure(Category category, function<Ref<CodeItem>()> getCode) const;
    ResultData GetResultDataEmpty(Category category) const;

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
    HW_PR_DECL_GETTER(Array<string>, DumpData) override
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
      HW_ASSERT_(!this->value.hollow);
    }

    HW_PR_THISREF;

  private:
    HW_PR_DECL_GETTER(Array<string>, DumpData) override { return {HW_D_VALUE(value)}; };
    HW_PR_DECL_GETTER(bool, hollow) { return false; };
    HW_PR_DECL_GETTER(Size, size) override { return value.size; }
    HW_PR_DECL_GETTER(WeakRef<Global>, global) override { return value.global; }
    HW_PR_DECL_GETTER(Optional<WeakRef<NumberType>>, asNumberType) override { return value.As<NumberType>(); }
    HW_PR_DECL_GETTER(Address, toAddress) override;
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
