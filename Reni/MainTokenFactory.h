#pragma once

namespace Reni
{
    struct MainTokenFactory final
    {
        class Class{};
    private:
        Class const _;
    public:
        MainTokenFactory();
        Class const& EndOfText;
        Class const& Number;
        Class const& Text;
        Class const& TokenClass(String const&name)const;
        Class const& Error(String const&name)const;
    };

    struct Token final
    {
        MainTokenFactory::Class const&Class;
        SourcePart const Part;

        Token(MainTokenFactory::Class const&class_, SourcePart const& part)
            : Class(class_)
            , Part(part)
        {}
    };

}
