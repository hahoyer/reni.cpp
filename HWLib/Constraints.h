#pragma once

#pragma warning (push)
#pragma warning (disable:4189 )

template<class TTarget>
struct Where
{
    template<class TOther>
    static void Is()
    {
        TOther const* tempDestination = reinterpret_cast<TTarget const*>(0);
    }
};

#pragma warning (pop)