#pragma once

#include "SourcePart.h"

using namespace Reni;

template <typename TScanner, typename TTokenFactory, typename TToken>
Ref<TToken> const Scanner<TScanner, TTokenFactory, TToken>::CreateTokenAndAdvance()
{
    try
    {
        _position += TScanner::WhiteSpace(_position);

        auto count = _position.End;
        if (count.IsValid)
            return CreateAndAdvance(count, TTokenFactory::EndOfText);

        count = TScanner::Number(_position);
        if (count.IsValid)
            return CreateAndAdvance(count, TTokenFactory::Number);

        count = TScanner::Text(_position);
        if (count.IsValid)
            return CreateAndAdvance(count, TTokenFactory::Text);

        count = TScanner::Any(_position);
        if (count.IsValid)
            return CreateAndAdvance(count, TTokenFactory::TokenClass(_position.Part(count)));

        mdump();
        bp();
        return null;
    }
    catch (TScanner::Error const& error)
    {
        return CreateAndAdvance(error.Count, TTokenFactory::Error(error.Id));
    }
}


template <typename TScanner, typename TTokenFactory, typename TToken>
Ref<TToken> const
Scanner<TScanner, TTokenFactory, TToken>
::CreateTokenAndAdvance(int count, typename TTokenFactory::Class const& tokenClass)
{
    auto part = _position.Part(count);
    position += count;
    return new TToken(tokenClass, part);
}
