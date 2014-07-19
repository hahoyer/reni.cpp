#pragma once;

namespace HWLib
{
    namespace internal
    {
        template <class T>
        struct RawContainer
        {
            using thisType = RawContainer;
            std::aligned_union<sizeof(T), T> rawData[sizeof(T)];
            void Initialize(T const&data){ new (rawData)T(data); }
            void Dispose(){ reinterpret_cast<T*>(rawData)->~T(); }

            T const& operator*()const { return *reinterpret_cast<T const*>(rawData); };
            T const* operator->()const { return reinterpret_cast<T const*>(rawData); };
            T & operator*(){ return *reinterpret_cast<T *>(rawData); };
            T * operator->(){ return reinterpret_cast<T *>(rawData); };
        };

        template<class... TItems>
        class UnionData
        {
        public:
            template<class T> static int const typeId();
            void Dispose(int typeId);
            template<class T> T const get()const;
        };

        template<>
        class UnionData < >
        {
        public:
            void Dispose(int)
            {
                throw "Invalid typeid";
            }

            template<class T> static int const typeId();
        };

        template<class TItem, class... TItems>
        class UnionData < TItem, TItems... >
        {
            using thisType = UnionData;
            union
            {
                RawContainer<TItem> item;
                RawContainer<UnionData<TItems...>> tail;
            };
        public:
            UnionData(TItem const& data)
            {
                item.Initialize(data);
            };

            template<class T>
            UnionData(T const& data)
            {
                tail.Initialize(data);

            };

            void Dispose(int typeId)
            {
                if(typeId)
                    tail->Dispose(typeId - 1);
                else
                    item.Dispose();
            }

            template<class T> T const get(int typeId)const
            {
                if(!typeId)
                    throw "invalid type";
                return tail->get<T>(typeId - 1);
            }

            template<> TItem const get<TItem>(int typeId)const
            {
                if(typeId)
                    throw "invalid type";
                return *item;
            }

            template<class T> static int const typeId(){ return UnionData<TItems...>::typeId<T>() + 1; }
            template<> static int const typeId<TItem>(){ return 0; };

        };
    }

    template<class... TItems> class Union
    {
        using thisType = Union;
        int const typeId;
        internal::UnionData<TItems...> data;
    public:
        template<class T> 
        bool const is()const{ return internal::UnionData<TItems...>::typeId<T>() == typeId; };
        template<class T> 
        Union(T const&data)
            : typeId(internal::UnionData<TItems...>::typeId<T>())
            , data(data)
        {};

        ~Union(){ Dispose(); }

        template<class T>
        T const get()const{ return data.get<T>(typeId); };
        
        void Dispose(){ data.Dispose(typeId); }
    };

}
