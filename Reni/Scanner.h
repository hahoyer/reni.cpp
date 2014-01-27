#pragma once

#include "SourcePosition.h"

namespace Reni
{
    class SourcePosition;
    template<typename T>
    class IType
    {};

    template<typename T>
    class IPart
    {};

    template<typename T>
    class Item
    {
    public:
        IType<T> const Type;
        IPart<T> const Part;

        Item(IType<T> type, IPart<T> part)
        {
            Type = type;
            Part = part;
        }

        p(String, Name){ return Type == null ? PrioTable.BeginOfText : Type.PrioTableName; }
        p(bool, IsEnd){ Type != null && Type.IsEnd; }
        
        T Create(T left, T right)
        {
            if (Type != null)
                return Type.Create(left, Part, right);
            Tracer.Assert(left == null);
            return right;
        }
    };

    class IParsedSyntax;
    class ITokenFactory;

    class Scanner
    {

        SourcePosition _position;
    public: 
        Scanner(Ref<Source const> source);

    protected:
        virtual int WhiteSpace(SourcePosition const& position) const = 0;
        virtual Optional<int> Number(SourcePosition const& position) const = 0;
        virtual Optional<int> Text(SourcePosition const& position) const = 0;
        virtual Optional<int> Any(SourcePosition const& position) const = 0;

    public:
        Item<IParsedSyntax> CreateToken(Ref<ITokenFactory const> tokenFactory);

    private:
        Optional<Item<IParsedSyntax>> CreateAndAdvance(int count,Ref<IType<IParsedSyntax>> tokenClass);

    };
}
