#pragma once

namespace ReniRuntime{
    class Stack{
        using thisType = Stack;
    public:
        using dataType = unsigned int;
        using sizeType = unsigned int;
    private:
        struct Item{
            Item* next;
            sizeType size;
            dataType value;
        };

        Item* top;
    public:
        void Push(dataType size, sizeType value){
            top = new Item{ top, size, value };
        }

    };
    
    
    inline void push(Stack& stack, Stack::sizeType size, Stack::dataType value){
        stack.Push(size, value);
    };
}

