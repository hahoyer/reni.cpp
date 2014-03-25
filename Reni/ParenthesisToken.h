#pragma once
#include "TokenClass.h"
#include "FeatureClass.h"

namespace Reni{
    class LeftParenthesisToken final : public PrefixTokenClass{
        using baseType = PrefixTokenClass;
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
        Ref<Syntax > const Create(SourcePart const&part, Ref<Syntax>const right)const  override final{
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


    class RightParenthesisToken final : public SuffixTokenClass{
        using baseType = SuffixTokenClass;
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
        p_function(bool, AcceptsMatch) override{ return true; }
        Ref<Syntax > const Create(Ref<Syntax >const left, SourcePart const&part)const  override final{
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