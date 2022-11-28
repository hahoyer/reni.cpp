#pragma once

#include "../HWLib/_EditorTemplates.h"
#include "../HWLib/DumpableObject.h"
#include "../HWLib/RefCountProvider.h"

using namespace HWLib;

namespace Reni
{
  class ReplaceVisitor;
  class Closure;
  class Context;

  class External
    : public WithId<DumpableObject, External>
  {
    using baseType = DumpableObject;
    using thisType = External;

  public:
    class This;
    class Args;
    class Function;
    class Context;

    ThisRef;
    bool const operator <(thisType const& other) const;
    bool const operator ==(thisType const& other) const;
    virtual bool IsProvided(ReplaceVisitor const&) const { return false; };
    virtual Closure const Replace(ReplaceVisitor const& arg) const;
    p_function(string, DumpShort) override { return {internalDump()}; };

  private:
    p_function(Array<string>, DumpData) override { return {internalDump()}; };
    p_function(string, DumpHeader) override { return ""; };

  protected:
    virtual string internalDump() const = 0;
  };

  class External::This final : public External
  {
    using baseType = External;
    using thisType = This;

  public:
    static thisType const Instance;

  private:
    This()
    {
      SetDumpString();
    };
    bool IsProvided(ReplaceVisitor const& arg) const override;
    string internalDump() const override { return "This"; };
    Closure const Replace(ReplaceVisitor const& arg) const override;
  };

  class External::Args final : public External
  {
    using baseType = External;
    using thisType = Args;

  public:
    static thisType const Instance;

  private:
    Args()
    {
      SetDumpString();
    };
    bool IsProvided(ReplaceVisitor const& arg) const override;
    string internalDump() const override { return "Arg"; };
    Closure const Replace(ReplaceVisitor const& arg) const override;
  };

  class External::Function : public External
  {
    using baseType = External;
    using thisType = Function;

  public:
    class Arg;
    class NewValue;

  protected:
    Function() {};
    string internalDump() const override { return "/\\"; };
  };

  class External::Function::Arg final : public Function
  {
    using baseType = Function;
    using thisType = Arg;

  public:
    static thisType const Instance;

  private:
    Arg()
    {
      SetDumpString();
    };
    string internalDump() const override { return baseType::internalDump() + "."; };

  public:
    bool IsProvided(ReplaceVisitor const&) const override;
    Closure const Replace(ReplaceVisitor const& arg) const override;
  };

  class External::Function::NewValue final : public Function
  {
    using baseType = Function;
    using thisType = NewValue;

  public:
    static thisType const Instance;

  private:
    NewValue()
    {
      SetDumpString();
    };
    string internalDump() const override { return baseType::internalDump() + "new_value"; };
  };
}
