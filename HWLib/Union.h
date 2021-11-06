#pragma once

using namespace std;

namespace HWLib
{
    class InvalidUnionTypeException : public exception{};
    class IncorrectUnionTypeException : public exception{};
    class UnexpectedUnionTypeException : public exception{};

    namespace internal
    {
        using typeIdType = unsigned __int8;
        static typeIdType const maxTypeId = 255;

        template<class... TItems>
        struct UnionHelper;

        template<>
        struct UnionHelper<>
        {
            template<class T> static typeIdType const typeId(){static_assert(false, "Invalid type ");};
            static void Dispose(int, void*){ throw InvalidUnionTypeException(); };
            static void Initialize(typeIdType, void const*, void*){ throw UnexpectedUnionTypeException(); };
        };

        template<class TItem, class... TItems>
        struct UnionHelper<TItem, TItems...>
        {
            template <class T>
            static typeIdType typeId()
            {
                static_assert(sizeof...(TItems) < maxTypeId, "too much items for union");
                return UnionHelper<TItems...>::template typeId<T>() + 1;
            };
            template<>
            typeIdType typeId<TItem>(){ return 0; };

            static void Dispose(typeIdType typeId, void* data)
            {
                if(typeId)
                    UnionHelper<TItems...>::Dispose(typeId - 1, data);
                else
                    reinterpret_cast<TItem*>(data)->~TItem();
            };

            static void Initialize(typeIdType typeId, void const* sourceData, void* data)
            {
                if(typeId)
                    UnionHelper<TItems...>::Initialize(typeId - 1, sourceData, data);
                else
                    new (data)TItem(*reinterpret_cast<TItem const*>(sourceData));
            };
        };


    }

    template<class... TItems>
    class Union
    {
        using thisType = Union;
        internal::typeIdType const typeId;
        typename std::aligned_union<0, TItems...>::type rawData;
    public:
        Union() = delete;
        Union(Union const&other)
            : typeId(other.typeId)
        {
            internal::UnionHelper<TItems...>::Initialize(other.typeId, &other.rawData, &rawData);
        };

        template<class T> Union(T const&data)
            : typeId(internal::UnionHelper<TItems...>::template typeId<T>())
        {
            new (&rawData) T(data);
        };

        HW_DO_PLACEMENT_ASSIGN;
        ~Union(){ internal::UnionHelper<TItems...>::Dispose(typeId, &rawData); }

        template<class T> bool const is()const{ return internal::UnionHelper<TItems...>::template typeId<T>() == typeId; };
        template<class T> T const get()const
        {
            if(is<T>())
                return get_unchecked<T>();
            throw IncorrectUnionTypeException();
        };

        template<class T> T const& const get_unchecked()const
        {
            return *reinterpret_cast<T const*>(static_cast<void const*>(&rawData));
        };
    };

    template<>class Union<> {};


}
