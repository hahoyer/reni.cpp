#pragma once
#include "Type.h"

namespace Reni
{
  class AddressType final : public Type
  {
    typedef Type baseType;
    typedef AddressType thisType;

  public:
    Type const& value;

    AddressType(Type const& value) : value(value)
    {
      SetDumpString();
      HW_ASSERT_(!this->value.hollow);
    }

    HW_PR_THISREF;

  private:
    HW_PR_DECL_GETTER(Optional<WeakRef<NumberType>>, asNumberType) override { return value.As<NumberType>(); }
    HW_PR_DECL_GETTER(Array<string>, DumpData) override { return {HW_D_VALUE(value)}; };
    HW_PR_DECL_GETTER(WeakRef<Global>, global) override { return value.global; }
    HW_PR_DECL_GETTER(bool, hollow) { return false; };
    HW_PR_DECL_GETTER(WeakRef<Type>, toTypeTarget) override { return value.toTypeTarget; };
    HW_PR_DECL_GETTER(Size, size) override { return Size::Address; }
    HW_PR_DECL_GETTER(Address, toAddress) override;

    virtual SearchResult<Feature> DeclarationsForType(DeclarationType const& token) const override;
    virtual Array<Ref<FiberItem>> ConvertFiber(Type const& destination) const override;
  };
};
