#pragma once
#include "../HWLib/string.h"
#include "../Runtime/Common.h"
#include "../HWLib/DumpableObject.h"
#include "../HWLib/RefCountProvider.h"
#include "../HWLib/Ref.h"
#include "ArrayType.h"
#include "../Util/BitsConst.h"

using namespace HWLib;
using namespace Util;

namespace Reni
{
  class ContainerContext;
  class CodeVisitor;
  class Closure;
  class FiberCode;
  class FiberItem;
  class NumberType;
  class ReplaceVisitor;
  class Result;

  class CodeItem
    : public WithId<DumpableObject>
      , public RefCountProvider
  {
    typedef DumpableObject baseType;
    typedef CodeItem thisType;

  public:
    typedef ReniRuntime::Stack::dataType dataItemType;
    typedef ReniRuntime::Stack::sizeType dataItemSizeType;

    static Ref<CodeItem> Assign(Type const& data, Ref<CodeItem> left);
    static Ref<CodeItem> Arg(Type const& type);
    static Ref<CodeItem> FunctionArg(Type const& value);
    static Ref<CodeItem> Const(BitsConst const& value);
    static Ref<CodeItem> Empty() { return Const(BitsConst::Empty()); };
    static Ref<CodeItem> NumberOperation
    (
      string name,
      Address const& result,
      Type const& left,
      Type const& right
    );
    static Ref<CodeItem> DumpPrint(NumberType const& value);
    static Ref<CodeItem> This(Type const& type);
    static Ref<CodeItem> Reference(ContainerContext const& value);
    static Ref<CodeItem> CallGetter(Size const& size, size_t index, Type const& arg);
    static Ref<CodeItem> CallGetter(Size const& size, size_t index);
    static Ref<CodeItem> SmartList(Array<Optional<Ref<CodeItem>>> const& items);
    static Ref<CodeItem> List(Array<Ref<CodeItem>> const& items);
    static Ref<CodeItem> NumberConversion(Address const& result, Type const& arg);
    static Ref<CodeItem> IfThenElse(Ref<CodeItem> condition, Ref<CodeItem> then, Ref<CodeItem> _else);

    bool operator==(thisType const& other) const;

    virtual string ToCpp(CodeVisitor const& visitor) const;
    HW_PR_VIRTUAL_GET(bool, isReference) { return false; };
    HW_PR_VIRTUAL_GET(Size, size) = 0;
    HW_PR_VIRTUAL_GET(Closure, closure) = 0;
    HW_PR_VIRTUAL_GET(bool, isEmpty) { return false; };
    HW_PR_THISREF;
    Optional<Ref<CodeItem>> Replace(ReplaceVisitor const& arg) const;
    Ref<FiberCode> operator+(FiberItem const& item) const;
    virtual Ref<FiberCode> operator+(Array<Ref<FiberItem>> const& items) const;
    virtual Ref<CodeItem> ReferencePlus(Size offset) const;
    virtual Ref<CodeItem> Convert(Type const& type) const;

  protected:
    virtual Optional<Ref<CodeItem>> ReplaceImpl(ReplaceVisitor const& arg) const = 0;
  };
}
