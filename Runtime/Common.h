#pragma once
#include <iostream>  

namespace ReniRuntime{
    class Stack{
        using thisType = Stack;
    public:
        typedef unsigned int dataType;
        typedef unsigned int sizeType;
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

    inline void DumpPrint(int value){
        std::cout << value;
    };
}

