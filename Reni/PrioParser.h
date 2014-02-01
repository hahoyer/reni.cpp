#pragma once

#include <boost/lockfree/stack.hpp>
#include <boost/type_traits.hpp>

namespace Reni
{
    template <typename T>
    struct IPosition{};

    template <typename T>
    struct Item;
    
    class PrioTable{};

    template <typename T>
    class OpenItem final
    {
        using thisType = OpenItem;

    public:
        OpenItem(OptRef<T const> left, Item<T> right);

        //DefaultAssignmentOperator;

        static OpenItem const StartItem(IPosition<T> current);
        char const Relation(String newToken, PrioTable prioTable)const;
        Ref<T const> const Create(OptRef<T const> args)const;
    };

    template <typename T>
    class Stack final
    {
        boost::lockfree::stack<T> _internal;
    public:
        Stack() = default;
        Stack(Stack const&) = delete;

        void Push(T target);
        T const Pop();
        p(T, Top);
        p(bool, IsEmpty);
    };

    template <typename T>
    struct Item
    {
        String const Name;
        Item();

        p(bool, IsEnd);
    };

    template <typename T>
    struct PositionManager
    {
        IPosition<T> const StartItem;

        PositionManager();

        Item<T> const GetItemAndAdvance(Stack<OpenItem<T>>*stack);
    };


    template <typename T>
    OptRef<T const> const Parse(PositionManager<T>& manager, PrioTable prioTable, Stack<OpenItem<T>>* initialStack = null)
    {
        Stack<OpenItem<T>>* stack = initialStack;
        if (stack == null)
        {
            stack = new Stack<OpenItem<T>>();
            stack->Push(OpenItem<T>::StartItem(manager.StartItem));
        }

        do
        {
            auto item = manager.GetItemAndAdvance(stack);
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

