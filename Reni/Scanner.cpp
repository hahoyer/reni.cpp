#include "Import.h"
#include "Scanner.h"

#include "SourcePosition.h"
#include "TokenFactory.h"

using namespace Reni;

class Exception
{
public:
    SourcePosition const SourcePosition;
    int Count;
    IType<IParsedSyntax> TokenClass;

    Exception();
};


Scanner::Scanner(Ref<Source const> source)
: _position(source)
{}


Item<IParsedSyntax> Scanner::CreateToken(Ref<ITokenFactory const> tokenFactory)
{
    try
    {
        _position += WhiteSpace(_position);

        auto count = _position.End;
        if (count.IsValid)
            return CreateAndAdvance(count, tokenFactory->EndOfText);

        count = Number(_position);
        if (count.IsValid)
            return CreateAndAdvance(count, tokenFactory->Number);

        count = Text(_position);
        if (count.IsValid)
            return CreateAndAdvance(count, tokenFactory->Text);

        count = Any(_position);
        if (count.IsValid)
            return CreateAndAdvance(count, tokenFactory->TokenClass(_position.Part(count)));

        mdump();
        bp();
        return null;
    }
    catch (Exception const& exception)
    {
        assert(exception.SourcePosition == _position);
        return CreateAndAdvance(exception.Count, exception.TokenClass);
    }
}


Optional<Item<IParsedSyntax>> Scanner::CreateAndAdvance(int count,Ref<IType<IParsedSyntax>> tokenClass)
{
    auto result = new Item<IParsedSyntax>(tokenClass, TokenData.Span(_position, count));
    position += count;
    return result;
}


internal sealed class Exception : System.Exception
{
    public readonly SourcePosition SourcePosition;
    public readonly IType<IParsedSyntax> TokenClass;
    public readonly int Length;

    public Exception(SourcePosition position, IType<IParsedSyntax> tokenClass, int length)
    {
        SourcePosition = position;
        TokenClass = tokenClass;
        Length = length;
    }
}

