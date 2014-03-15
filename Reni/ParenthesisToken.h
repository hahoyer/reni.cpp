#pragma once
#include "TokenClass.h"
#include "FeatureClass.h"

namespace Reni{
    class LeftParenthesisToken final : public TokenClass{
        using baseType = TokenClass;
        using thisType = LeftParenthesisToken;

        int const level;
    public:
        LeftParenthesisToken(int level) : level(level){}

        p(String, name){
            a_is(level, > , 0);
            a_is(level, < , 4);
            return level[" {[("];
        }
    private:
        p_function(Optional<bool>,HasLeft) override{ return false; }
        p_function(Optional<bool>,HasRight) override{ return true; }

        Ref<Syntax > const Prefix(SourcePart const&part, Ref<Syntax>const right)const  override{
            return new OpenSyntax(level, part, right);
        };
        p_function(Array<String>,DumpData) override{
            return{nd(level)};
        };

        class OpenSyntax final : public Syntax{
            using baseType = Syntax;
            int const level;
            Ref<Syntax > const right;
        public:
            OpenSyntax(int level, SourcePart const part, Ref<Syntax > const right)
                : baseType(part)
                , level(level)
                , right(right)
            {
                SetDumpString();
            }
        private:
            virtual Ref<Syntax > const ParenthesisMatch(int level, SourcePart const&part)const override{
                if(level != this->level)
                    return baseType::ParenthesisMatch(level, part);
                return right;
            };
            p_function(Array<String>,DumpData) override{
                return{nd(level), nd(right)};
            }
        };
    private:
        GenericFeatureClass<thisType> feature;
        p_function(WeakRef<FeatureClass>,featureClass) override{ return &feature.ref; }
    };


    class RightParenthesisToken final : public TokenClass{
        using baseType = TokenClass;
        using thisType = RightParenthesisToken;

        int const level;
    public:
        RightParenthesisToken(int level) : level(level){}
        p(String, name){
            a_is(level, >, 0);
            a_is(level, <, 4);
            return level[" }])"];
        }
    private:
        bool AcceptsMatch(bool isMatch) const override{ return isMatch; }
        p_function(Optional<bool>,HasLeft) override{ return true; }
        p_function(Optional<bool>,HasRight) override{ return false; }

        virtual Ref<Syntax > const Suffix(Ref<Syntax >const left, SourcePart const&part)const  override{
            return left->ParenthesisMatch(level, part);
        };

        p_function(Array<String>,DumpData) override{
            return{nd(level)};
        };
    private:
        GenericFeatureClass<thisType> feature;
        p_function(WeakRef<FeatureClass>,featureClass) override{ return &feature.ref; }
    };

}