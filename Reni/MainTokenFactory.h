#pragma once

namespace Reni
{
    class TokenClass;

    class MainTokenFactory 
    {
    public:
        static TokenClass const& Start;
        static TokenClass const& End;
        static TokenClass const& Number;
        static TokenClass const& Text;
        static TokenClass const& GetTokenClass(String const&name);
        static TokenClass const& GetError(String const&name);
    };

}
