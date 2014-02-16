#pragma once

namespace HWLang{
    template <class Token, class TTokenFactory, class Scanner>
    class ScannerInstance final: public Enumerable<Token>::Iterator, public DumpableObject{
        using thisType = ScannerInstance;
        using baseType = typename Enumerable<Token>::Iterator;
    public:
        using TokenClass = typename Token::TokenClass;
        using TokenFactory = typename TTokenFactory;
    private:
        SourcePosition position;
        bool endTokenReturned;
        bool startTokenReturned;
    public:
        ScannerInstance(Ref<Source const> source)
            : position(source)
            , endTokenReturned(false)
            , startTokenReturned(false)
        {}

        DefaultAssignmentOperator;

        p_function(bool, IsValid) override{ return !endTokenReturned; };
        Token const Step() override;
    private:
        Token const Step(int count, TokenClass const& tokenClass);
        virtual p_function(Array<String>, DumpData)override;
    };
}