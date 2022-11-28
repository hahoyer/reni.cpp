#pragma once

#include "../HWLib/_EditorTemplates.h"
#include "../HWLib/DumpableObject.h"
#include "../HWLib/CtrlRef.h"
#include "../HWLib/DumpToString.h"
#include "../HWLib/Enumerable.h"
#include "../HWLib/Common.h"
#include "External.h"

using namespace HWLib;

namespace Reni
{
  class CodeItem;
  class External;
  class ReplaceVisitor;

  class Closure final : public DumpableObject
  {
    using baseType = DumpableObject;
    using thisType = Closure;
    Array<WeakRef<External>> const data;

  public:
    Closure() { SetDumpString(); }
    Closure(External const& item);
    Closure(Closure const& other);
    static Closure const Aggregate(CtrlRef<Enumerable<Closure>> const& other);

  private:
    explicit Closure(Array<WeakRef<External>> const& other);

  public:
    HW_PR_THISREF;
    HW_DO_PLACEMENT_ASSIGN;

    HW_PR_GET(bool, isEmpty) { return data.Count == 0; }

    Closure const operator+(Closure const& other) const;
    Closure const operator-(Closure const& other) const;
    bool const operator==(Closure const& other) const;
    bool const operator==(External const& other) const;
    bool const Contains(External const& item) const;

    Optional<thisType> const Replace(ReplaceVisitor const& arg) const;

  private:
    HW_PR_DECL_GETTER(string, DumpHeader) override { return ""; };
    HW_PR_DECL_GETTER(Array<string>, DumpData) override;
    HW_PR_DECL_GETTER(string, DumpShort) override { return Dump; };
  };
}
