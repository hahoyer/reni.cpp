#pragma once

namespace HWLang
{
    template <typename TTokenFactory>
    class Scanner final: public DumpableObject
    {
        using thisType = Scanner;
        using baseType = DumpableObject;
    public:
        using TokenFactory = TTokenFactory;
        using TokenClass = TTokenFactory::TokenClass;
    private:
        SourcePosition _position;
        bool _endTokenReturned;
        TokenFactory const& _factory;
    public: 
        Scanner(Ref<Source const> source, TokenFactory const&factory)
            : _position(source)
            , _factory(factory)
            , _endTokenReturned(false)
        {}

        DefaultAssignmentOperator;

        p_function(bool, IsValid) override{ return !_endTokenReturned; }
        Token const Step() override;

    private:
        Token const Step(int count, class TTokenFactory::TokenClass const& tokenClass);
        virtual p_function(Array<String>, DumpData)override;
    };

}
