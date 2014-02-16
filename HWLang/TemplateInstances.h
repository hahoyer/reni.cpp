#pragma once

using namespace HWLang;

template <class Token, class TokenFactory, class Scanner>
Token const 
ScannerInstance<Token, TokenFactory, Scanner>
::Step(){
    try{
        if (!startTokenReturned){
            startTokenReturned = true;
            return Token(TokenFactory::Start, position.Span(0));
        }

        static bool Trace = true;
        position += Scanner::WhiteSpace(position);

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
        return Step(error.Count, TokenFactory::GetError(error.Id));
    }
}


template <class Token, class TokenFactory, class Scanner>
Token const
ScannerInstance<Token, TokenFactory, Scanner>
::Step(int count, TokenClass const& tokenClass){
    auto part = position.Span(count);
    position += count;
    return Token(tokenClass, part);
};


template <class Token, class TokenFactory, class Scanner>
Array<String> const
ScannerInstance<Token, TokenFactory, Scanner>
::get_DumpData()const{
    return{
        nd(position)
    };
}