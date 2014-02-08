#pragma once

namespace Reni
{
    class TokenClass;

    class MainTokenFactory final
    {
    public:
        MainTokenFactory();
        TokenClass  const& BeginOfText;
        TokenClass const& EndOfText;
        TokenClass const& Number;
        TokenClass const& Text;
        TokenClass const& GetTokenClass(String const&name)const;
        TokenClass const& GetError(String const&name)const;
    };

}
