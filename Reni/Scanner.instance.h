#pragma once

using namespace Reni;

template <typename TScanner, typename TTokenFactory, typename TToken>
Ref<TToken> const Scanner<TScanner, TTokenFactory, TToken>::Step()
{
    try
    {
        _position += TScanner::WhiteSpace(_position);

        auto count = _position.End;
        if (count.IsValid)
            return Step(count, TTokenFactory::EndOfText);

        count = TScanner::Number(_position);
        if (count.IsValid)
            return Step(count, TTokenFactory::Number);

        count = TScanner::Text(_position);
        if (count.IsValid)
            return Step(count, TTokenFactory::Text);

        count = TScanner::Any(_position);
        if (count.IsValid)
            return Step(count, TTokenFactory::TokenClass(_position.Part(count)));

        mdump();
        assert_fail;
        errorabort(Ref<TToken>);
    }
    catch (TScanner::Error const& error)
    {
        return Step(error.Count, TTokenFactory::Error(error.Id));
    }
}


template <typename TScanner, typename TTokenFactory, typename TToken>
Ref<TToken> const
Scanner<TScanner, TTokenFactory, TToken>
::Step(int count, typename TTokenFactory::Class const& tokenClass)
{
    auto part = _position.Span(count);
    _position += count;
    return new TToken(tokenClass, part);
}
