#pragma once

using namespace HWLib;

namespace HWLang{
    template <class TToken, class TTokenFactory, class Scanner>
    class ScannerInstance : public Enumerable<TToken>::Iterator, public DumpableObject{
        typedef ScannerInstance thisType;
        typedef typename Enumerable<TToken>::Iterator baseType;
    public:
        typedef typename TToken::TokenClass TokenClass;
        typedef typename TTokenFactory TokenFactory;
    private:
        SourcePosition position;
        bool endTokenReturned;
        bool startTokenReturned;
    public:
        ScannerInstance(Source const& source)
            : position(source)
            , endTokenReturned(false)
            , startTokenReturned(false)
        {}

        DefaultAssignmentOperator;

        override_p_function(bool, IsValid){ return !endTokenReturned; };
        TToken const Step() override;
    private:
        TToken const Step(int count, TokenClass const& tokenClass);
        override_p_function(Array<String>, DumpData);
    };
}