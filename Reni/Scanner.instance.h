#pragma once

using namespace Reni;

template <typename TTokenFactory>
Token const Scanner<TTokenFactory>::Step()
{
    try
    {
        static bool Trace = true;
        _position += ReniScanner::WhiteSpace(_position);

        auto count = _position.End;
        if (count.IsValid)
        {
            _endTokenReturned = true;
            return Step(count, _factory.EndOfText);
        }

        count = ReniScanner::Number(_position);
        if (count.IsValid)
            return Step(count, _factory.Number);

        count = ReniScanner::Text(_position);
        if (count.IsValid)
            return Step(count, _factory.Text);

        count = ReniScanner::Any(_position);
        if (count.IsValid)
            return Step(count, _factory.GetTokenClass(_position.Part(count)));

        mdump();
        assert_fail;
        errorabort(Token const&);
    }
    catch (ReniScanner::Error const& error)
    {
        return Step(error.Count, _factory.GetError(error.Id));
    }
}


template <typename TTokenFactory>
Token const
Scanner<TTokenFactory>
::Step(int count, TokenClass const& tokenClass)
{
    auto part = _position.Span(count);
    _position += count;
    return Token(tokenClass, part);
};


template <typename TTokenFactory>
Array<String> const
Scanner<TTokenFactory>
::get_DumpData()const
{
    return{
        vardump(_position)
    };
}

