#pragma once

namespace HWLang
{
  class SourcePart;

  template <class TTokenClass>
  class Token final : public DumpableObject
  {
    using baseType = DumpableObject;
    using thisType = Token;

  public:
    typedef TTokenClass TokenClass;

    TokenClass const& Class;
    SourcePart const Part;

    Token(TokenClass const& class_, SourcePart const& part)
      : Class(class_)
        , Part(part)
    {
      SetDumpString();
    };

    HW_DO_PLACEMENT_ASSIGN;

    HW_PR_GET(string, Name)
    {
      if(IsEnd)
        return End;
      if(IsStart)
        return Start;
      return Part;
    };

    HW_PR_GET(bool, IsEnd) { return Part.IsEnd; };
    HW_PR_GET(bool, IsStart) { return Part.IsStart; };

  private:
    HW_PR_DECL_GETTER(Array<string>, DumpData) override
    {
      return {
        HW_D_VALUE(Class),
        HW_D_VALUE(Part)
      };
    };
    HW_PR_DECL_GETTER(string, DumpShort) override
    {
      return HW_PR_BASE_GETTER_NAME(DumpShort) + "{" + Name + "}";
    };
  };
}
