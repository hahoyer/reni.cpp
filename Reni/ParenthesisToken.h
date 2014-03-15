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
        override_p_function(Optional<bool>, HasLeft){ return false; }
        override_p_function(Optional<bool>, HasRight){ return true; }

        Ref<Syntax > const Prefix(SourcePart const&part, Ref<Syntax>const right)const  override{
            return new OpenSyntax(level, part, right);
        };
        override_p_function(Array<String>, DumpData){
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
            override_p_function(Array<String>, DumpData){
                return{nd(level), nd(right)};
            }
        };
    private:
        GenericFeatureClass<thisType> feature;
        override_p_function(WeakRef<FeatureClass>, featureClass){ return &feature.ref; }
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
        override_p_function(Optional<bool>, HasLeft){ return true; }
        override_p_function(Optional<bool>, HasRight){ return false; }

        virtual Ref<Syntax > const Suffix(Ref<Syntax >const left, SourcePart const&part)const  override{
            return left->ParenthesisMatch(level, part);
        };

        override_p_function(Array<String>, DumpData){
            return{nd(level)};
        };
    private:
        GenericFeatureClass<thisType> feature;
        override_p_function(WeakRef<FeatureClass>, featureClass){ return &feature.ref; }
    };

}