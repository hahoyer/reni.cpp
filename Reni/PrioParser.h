#pragma once

#include "PrioTable.h"
#include "MainTokenFactory.h"


namespace Reni
{
    template <typename T>
    struct Item;

    template <typename T>
    class OpenItem final
    {
        using thisType = OpenItem;

        OptRef<T const> const _left; 
        Token const _token;
    public:
        OpenItem() = delete;
        OpenItem(OptRef<T const> left, Token token)
            :_left(left)
            , _token(token)
        {}

        DefaultAssignmentOperator;

        static OpenItem const StartItem(Token startToken){ return OpenItem<T>(null, startToken); };
        char const Relation(String newTokenName, PrioTable prioTable)const{ return prioTable.Relation(newTokenName, _token.Name); };
        Ref<T const> const Create(OptRef<T const> right)const{ return _token.Create(_left, right); }
    };

    template <typename T>
    struct Item
    {
        String const Name;
        Item(SourcePosition current);

        Ref<T const> const Create(OptRef<T const> const left, OptRef<T const> args)const;
        p(bool, IsEnd);
    };

    template <typename T>
    class PositionManager
    {
        Scanner<MainTokenFactory> _scanner;
    public:
        PositionManager(Ref<Source const> source, MainTokenFactory const&factory, Token const startToken)
            : _scanner(source, factory)
            , StartToken(startToken)
        {};

        Token const StartToken;
        Token const Step(Stack<OpenItem<T>>*){ return _scanner.Step(); }
    };


    template <typename T>
    OptRef<T const> const Parse(PositionManager<T>& manager, PrioTable prioTable, Stack<OpenItem<T>>* initialStack = null)
    {
        Stack<OpenItem<T>>* stack = initialStack;
        if (stack == null)
        {
            stack = new Stack<OpenItem<T>>();
            stack->Push(OpenItem<T>::StartItem(manager.StartToken));
        }

        do
        {
            auto item = manager.Step(stack);
            OptRef<T const> result;
            do
            {
                auto topItem = stack->Top;
                auto relation = topItem.Relation(item.Name, prioTable);

                if (relation != '+')
                    result = stack->Pop().Create(result);

                if (relation == '-')
                    continue;

                if (item.IsEnd)
                    return result;

                stack->Push(OpenItem<T>(result, item));
                result = OptRef<T const>();
            } while (result.IsValid);
        } while (true);
    }

    template <typename T>
    class IOperator{};
    class IOperatorPart{};

    template <typename T>
    OptRef<T const> const Operation(IOperator<T> operator_, T left, IOperatorPart token, T right)
    {
        return left == null
            ? (
            right == null
            ? operator_.Terminal(token)
            : operator_.Prefix(token, right)
            )
            : (
            right == null
            ? operator_.Suffix(left, token)
            : operator_.Infix(left, token, right)
            );
    }
}

