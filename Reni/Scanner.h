#pragma once

#include "MainTokenFactory.h"

namespace Reni
{
    class Token;

    template <typename TTokenFactory>
    class Scanner final: public Enumerable<Token>::Iterator, public DumpableObject
    {
        using thisType = Scanner;
        using baseType = typename Enumerable<Token>::Iterator;

        SourcePosition _position;
        bool _endTokenReturned;
        TTokenFactory const& _factory;
    public: 
        Scanner(Ref<Source const> source, TTokenFactory const&factory)
            : _position(source)
            , _factory(factory)
            , _endTokenReturned(false)
        {}

        DefaultAssignmentOperator;

        p_function(bool, IsValid) override{ return !_endTokenReturned; }
        Token const Step() override;

    private:
        Token const Step(int count, TokenClass const& tokenClass);
        virtual p_function(Array<String>, DumpData)override;
    };

}
