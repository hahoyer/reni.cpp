#pragma once

#include "SourcePosition.h"

namespace Reni
{
    template <typename TScanner, typename TTokenFactory, typename TToken>
    class Scanner
    {
        SourcePosition _position;
    public: 
        Scanner(Ref<Source const> source)
            : _position(source)
        {}

        Ref<TToken> const CreateTokenAndAdvance();
    private:
        Ref<TToken> const CreateTokenAndAdvance(int count, typename TTokenFactory::Class const& tokenClass);

    };

    class BaseScanner{};


    class MainTokenFactory
    {
    public:
        class Class{};
    };


    class Token{};

}
