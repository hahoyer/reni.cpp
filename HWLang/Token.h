#pragma once

namespace HWLang{
    class SourcePart; 

    template<class TTokenClass>
    class Token final : public DumpableObject{
        using baseType = DumpableObject;
        using thisType = Token;
    public:
        using TokenClass = TTokenClass;
        
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
        virtual p_function(Array<String>, DumpData)override{
            return{
                vardump(Class),
                vardump(Part)
            };
        };
        virtual p_function(String, DumpShort)override{
            return baseType::get_DumpShort() + "{" + Name + "}";
        };
    };
}   