#pragma once

#include "MainTokenFactory.h"

namespace Reni
{
    template <typename TScanner, typename TTokenFactory, typename TToken>
    class Scanner final
    {
        SourcePosition _position;
        TTokenFactory const& _factory;
    public: 
        Scanner(Ref<Source const> source, TTokenFactory const&factory)
            : _position(source)
            , _factory(factory)
        {}

        Ref<TToken> const Step();
    private:
        Ref<TToken> const Step(int count, typename TTokenFactory::Class const& tokenClass);

    };

}
