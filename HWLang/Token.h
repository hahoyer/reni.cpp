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

        DefaultAssignmentOperator;

        p(String, Name){
            if (IsEnd)
                return PrioTableConst::End;
            if (IsStart)
                return PrioTableConst::Start;
            return Part;
        };

        p(bool, IsEnd){ return Part.IsEnd; };
        p(bool, IsStart){ return Part.IsStart; };

    private:
        p_function(Array<String>,DumpData) override{
            return{
                nd(Class),
                nd(Part)
            };
        };
        p_function(String,DumpShort) override{
            return base_p_name(DumpShort) + "{" + Name + "}";
        };
    };
}   