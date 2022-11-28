#pragma once
#include "EmptySyntax.h"
#include "Syntax.h"
#include "TokenClass.h"

namespace Reni
{
    class LeftParenthesisToken final : public TokenClass
    {
        using baseType = TokenClass;
        using thisType = LeftParenthesisToken;

        int const level;
    public:
        LeftParenthesisToken(int level) : level(level){}

        HW_PR_GET(string, name){
            HW_ASSERT_IS(level, > , 0);
            HW_ASSERT_IS(level, < , 4);
            return string(1,level[" {[("]);
        }
    private:
        HW_PR_DECL_GETTER(Array<string>,DumpData) override{ return{HW_D_VALUE(level)}; };

        class OpenSyntax final : public Syntax
        {
            using baseType = Syntax;
            int const level;
            Optional<Ref<Syntax>> const right;
        public:
            OpenSyntax(int level, SourcePart const part, Optional<Ref<Syntax>> const right)
                : baseType(part)
                , level(level)
                , right(right)
            {
                SetDumpString();
            }

        private:
            virtual Ref<Syntax> const ParenthesisMatch(int level, SourcePart const& part)const override
            {
                if (level != this->level)
                    return baseType::ParenthesisMatch(level, part);
                return right || Ref<Syntax>(new EmptySyntax(part));
            };
            HW_PR_DECL_GETTER(string, SmartDump) override{return HW_D_VALUE(right);}
            HW_PR_DECL_GETTER(int, priority) override{ return 0; }
        };

        Ref<Syntax> const CreateSyntax(Optional<Ref<Syntax>>const left, SourcePart const& part, Optional<Ref<Syntax>>const right)const override final
        {
            HW_ASSERT_(left.IsEmpty);
            return new OpenSyntax(level, part, right);
        };
    };


    class RightParenthesisToken final : public SuffixTokenClass{
        using baseType = SuffixTokenClass;
        using thisType = RightParenthesisToken;

        int const level;
    public:
        RightParenthesisToken(int level) : level(level){}

        HW_PR_GET(string, name)
        {
            HW_ASSERT_IS(level, >, 0);
            HW_ASSERT_IS(level, <, 4);
            return string(1,level[" }])"]);
        }
    private:
        Ref<Syntax> const Create(Ref<Syntax>const left, SourcePart const& part)const override final
        {
            return left->ParenthesisMatch(level, part);
        };

        HW_PR_DECL_GETTER(Array<string>,DumpData) override
        {
            return{HW_D_VALUE(level)};
        };
    };

}