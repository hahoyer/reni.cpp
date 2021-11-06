#pragma once

namespace HWLang{
    class SourcePart; 

    template<class TTokenClass>
    class Token final : public DumpableObject{
        using baseType = DumpableObject;
        using thisType = Token;
    public:
        typedef TTokenClass TokenClass;
        
        TokenClass const&Class;
        SourcePart const Part;

        Token(TokenClass const&class_, SourcePart const& part)
            : Class(class_)
            , Part(part){
            SetDumpString();
        };

        HW_DO_PLACEMENT_ASSIGN;

        p(string, Name){
            if (IsEnd)
                return PrioTableConst::End;
            if (IsStart)
                return PrioTableConst::Start;
            return Part;
        };

        p(bool, IsEnd){ return Part.IsEnd; };
        p(bool, IsStart){ return Part.IsStart; };

    private:
        p_function(Array<string>,DumpData) override{
            return{
                HW_D_VALUE(Class),
                HW_D_VALUE(Part)
            };
        };
        p_function(string,DumpShort) override{
            return p_base_name(DumpShort) + "{" + Name + "}";
        };
    };
}   