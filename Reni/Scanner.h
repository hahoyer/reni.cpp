#pragma once

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

        Ref<TToken> const Step();
    private:
        Ref<TToken> const Step(int count, typename TTokenFactory::Class const& tokenClass);

    };

    class MainTokenFactory
    {
    public:
        class Class{};
        static Class const& EndOfText;
        static Class const& Number;
        static Class const& Text;
        static Class const& TokenClass(String const&name);
        static Class const& Error(String const&name);
    };


    class Token
    {
    public:
        Token(MainTokenFactory::Class const&Class, SourcePart const& part);
    };

}
