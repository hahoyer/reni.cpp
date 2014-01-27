#pragma once

namespace Reni
{
    class ITokenFactory
    {
    public:
        IType<IParsedSyntax> const EndOfText;
        IType<IParsedSyntax> const Number;
        IType<IParsedSyntax> const Text;
        
        virtual Ref<IType<IParsedSyntax>> const TokenClass(String const&name)const = 0;

        ITokenFactory();

    };
}
