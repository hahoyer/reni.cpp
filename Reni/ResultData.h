#pragma once

#include "Closure.h"
#include "../Util/Category.h"
#include "../Util/Size.h"

using namespace HWLib;
using namespace Util;


namespace Reni
{
  class Type;
  class ResultCache;

  class ResultData final : public DumpableObject
  {
    using baseType = DumpableObject;
    using thisType = ResultData;

  public:
    Optional<bool> const hollow;
    Optional<Size> const size;
    Optional<WeakRef<Type>> const type;
    Optional<Ref<CodeItem>> const code;
    Optional<Closure> const closure;

    ResultData() { SetDumpString(); };
    ResultData(Ref<CodeItem> code);
    ResultData(Type const& type);
    ResultData(Closure const& closure);

  private:
    ResultData(
      Optional<bool> const& hollow,
      Optional<Size> const& size,
      Optional<Ref<CodeItem>> const& code,
      Optional<WeakRef<Type>> type,
      Optional<Closure> const& closure
    )
      : hollow(hollow)
        , size(size)
        , code(code)
        , type(type)
        , closure(closure)
    {
      SetDumpString();
      AssertValid();
    };

    static ResultData const FullGet(
      Category category,
      Optional<bool> const& hollow,
      Optional<Size> const& size,
      Optional<Ref<CodeItem>> const& code,
      Optional<WeakRef<Type>> type,
      Optional<Closure> const& closure
    )
    {
      AssertValid(category, hollow, size, code, type, closure);
      return ResultData(hollow, size, code, type, closure) & category;
    };

  public:
    static ResultData Get(
      Category const& category,
      function<bool()> const& getHollow,
      function<Size()> getSize,
      function<Ref<CodeItem>()> getCode,
      function<WeakRef<Type>()> getType,
      function<Closure()> getClosure
    );
    static ResultData GetSmartHollowClosure(
      Category const& category,
      function<Size()> getSize,
      function<Ref<CodeItem>()> getCode,
      function<WeakRef<Type>()> getType
    );
    static ResultData GetSmartHollowSize(
      Category const& category,
      function<Ref<CodeItem>()> getCode,
      function<WeakRef<Type>()> getType,
      function<Closure()> getClosure
    );
    static ResultData GetSmartHollowSizeClosure(
      Category const& category,
      function<Ref<CodeItem>()> getCode,
      function<WeakRef<Type>()> getType
    );

    HW_DO_PLACEMENT_ASSIGN;
    ResultData operator|(ResultData const& other) const;
    bool IsConsistent(ResultData const& other) const;
    ResultData operator&(Category const& other) const;
    bool operator==(thisType const& other) const;
    HW_PR_GET(Category, complete)
    {
      return Category::Instance(hollow.IsValid, size.IsValid, !code.IsEmpty, !type.IsEmpty, closure.IsValid);
    }

    ResultData Replace(ReplaceVisitor const& arg) const;
    ResultData Replace(External const& tag, ResultCache const& result) const;
    ResultData Convert(Type const& destination) const;
    HW_PR_GET(ResultData, asFunctionResult);

  private:
    HW_PR_DECL_GETTER(Array<string>, DumpData) override;
    void AssertValid() const;
    static Optional<bool> ReplenishHollow(Category const& category, function<Ref<CodeItem>()> getCode,
                                          function<WeakRef<Type>()> getType);
    static Optional<Size> ReplenishSize(Category const& category, function<Ref<CodeItem>()> getCode,
                                        function<WeakRef<Type>()> getType);
    static Optional<Closure> ReplenishExternals(Category const& category, function<Ref<CodeItem>()> getCode);
    static void AssertValid(
      const Category& category,
      Optional<bool> const& hollow,
      Optional<Size> size,
      Optional<Ref<CodeItem>> code,
      Optional<WeakRef<Type>> type,
      Optional<Closure> const& closure
    );
  };
}
