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

        HW_DO_PLACEMENT_ASSIGN;

        HW_PR_DECL_GETTER(bool,IsValid) override{ return !endTokenReturned; };
        TToken Step() override;
    private:
        TToken Step(size_t count, TokenClass const& tokenClass);
        HW_PR_DECL_GETTER(Array<string>,DumpData) override;
    };
}