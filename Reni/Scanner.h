#pragma once

#include "MainTokenFactory.h"

namespace Reni
{
    template <typename TScanner, typename TTokenFactory, typename TToken>
    class Scanner final: public Enumerable<TToken>::Iterator, public DumpableObject
    {
        using thisType = Scanner;
        using baseType = typename Enumerable<TToken>::Iterator;

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
        TToken const Step() override;

    private:
        TToken const Step(int count, TokenClass const& tokenClass);
        virtual p_function(String, Dump)override;
    };

}
