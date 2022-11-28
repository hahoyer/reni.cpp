#pragma once
#include "Type.h"

using namespace HWLib;

namespace Reni
{
  class AccessData;
  class ContainerContext;
  class ColonEqual;

  class AccessType final : public Type
  {
    using baseType = Type;
    using thisType = AccessType;

    WeakRef<AccessData> data;

  public:
    AccessType(AccessType const&) = delete;
    explicit AccessType(AccessData const& data);
    HW_PR_THISREF;

    template <class TTokenClass>
    SearchResult<Feature> const DeclarationsForType() const;
    static Optional<WeakRef<AccessType>> Convert(Type const& target);

  private:
    HW_PR_GET(WeakRef<Type>, value);
    HW_PR_DECL_GETTER(bool, hollow) { return false; };
    HW_PR_DECL_GETTER(Array<string>, DumpData) override;
    HW_PR_DECL_GETTER(Size, size) override { return Size::Address; }
    HW_PR_DECL_GETTER(WeakRef<Global>, global) override;
    Optional<WeakRef<NumberType>> get_asNumberType() const override;
    SearchResult<Feature> DeclarationsForType(DeclarationType const& token) const override;
    Ref<ResultCache> DirectConvert() const override;
    Ref<CodeItem> DirectConvertCode() const;

    class AssignmentFeature final : public Feature::Extended
    {
      using baseType = Extended;
      using thisType = AssignmentFeature;

      ResultData Result(Category const& category, Type const& target, Type const& arg) const override;
    };
  };
};


#include "DumpPrintToken.h"

using namespace Reni;

template <>
inline SearchResult<Feature> const AccessType::DeclarationsForType<ColonEqual>() const
{
  return Feature::From<AssignmentFeature>(*this);
}

template <class TTokenClass>
SearchResult<Feature> const AccessType::DeclarationsForType() const
{
  return Feature::Error(Dump + "\n" + typeid(TTokenClass).name());
};
