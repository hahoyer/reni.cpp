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
        Class const& BeginOfText;
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

        p(String, Name);
        p(bool, IsEnd);

        template<typename T>
        Ref<T const> const Create(OptRef<T const> const left, OptRef<T const> right)const;
    };

}
