#pragma once

using namespace Reni;

template <typename TScanner, typename TTokenFactory, typename TToken>
TToken const Scanner<TScanner, TTokenFactory, TToken>::Step()
{
    try
    {
        _position += TScanner::WhiteSpace(_position);

        auto count = _position.End;
        if (count.IsValid)
        {
            _endTokenReturned = true;
            return Step(count, _factory.EndOfText);
        }

        count = TScanner::Number(_position);
        if (count.IsValid)
            return Step(count, _factory.Number);

        count = TScanner::Text(_position);
        if (count.IsValid)
            return Step(count, _factory.Text);

        count = TScanner::Any(_position);
        if (count.IsValid)
            return Step(count, _factory.TokenClass(_position.Part(count)));

        mdump();
        assert_fail;
        errorabort(TToken const&);
    }
    catch (TScanner::Error const& error)
    {
        return Step(error.Count, _factory.Error(error.Id));
    }
}


template <typename TScanner, typename TTokenFactory, typename TToken>
TToken const
Scanner<TScanner, TTokenFactory, TToken>
::Step(int count, typename TTokenFactory::Class const& tokenClass)
{
    auto part = _position.Span(count);
    _position += count;
    return TToken(tokenClass, part);
};


template <typename TScanner, typename TTokenFactory, typename TToken>
String const
Scanner<TScanner, TTokenFactory, TToken>
::get_Dump()const
{
    return vardump(_position);
}

