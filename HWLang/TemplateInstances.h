#pragma once

using namespace HWLang;

template <class Token, class TokenFactory, class Scanner>
Token const 
ScannerInstance<Token, TokenFactory, Scanner>
::Step(){
    try{
        if (!startTokenReturned){
            startTokenReturned = true;
            SetDumpString();
            return Token(TokenFactory::Start, position.Span(0));
        }

        static bool Trace = true;
        position += Scanner::WhiteSpace(position);
        SetDumpString();

        auto count = position.End;
        if (count.IsValid){
            endTokenReturned = true;
            return Step(count, TokenFactory::End);
        }

        count = Scanner::Number(position);
        if (count.IsValid)
            return Step(count, TokenFactory::Number);

        count = Scanner::Text(position);
        if (count.IsValid)
            return Step(count, TokenFactory::Text);

        count = Scanner::Any(position);
        if (count.IsValid)
            return Step(count, TokenFactory::GetTokenClass(position.Part(count)));

        md_
        a_fail_;
        errorabort(Token const&);
    }
    catch (Scanner::Error const& error){
        return Step(error.Count, TokenFactory::GetErrorClass(error.Id));
    }
}


template <class Token, class TokenFactory, class Scanner>
Token const
ScannerInstance<Token, TokenFactory, Scanner>
::Step(int count, TokenClass const& tokenClass){
    auto part = position.Span(count);
    position += count;
    SetDumpString();
    return Token(tokenClass, part);
};


template <class Token, class TokenFactory, class Scanner>
p_implementation(ScannerInstance<Token COMMA TokenFactory COMMA Scanner>, Array<String>, DumpData){
    return{
        nd(position)
    };
}