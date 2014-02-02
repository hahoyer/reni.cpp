#pragma once

namespace Reni
{
    struct PrioTable final
    {
        const String Any = "(any)";
        const String EndOfText = "(eot)";
        const String BeginOfText = "(bot)";
        const String Error = "(err)";

        PrioTable()
            : _dataCache([&]{return AllocData(); })
        {}

        char const Relation(String newTokenName, String recentTokenName)const
        {
            if (newTokenName == BeginOfText || recentTokenName == EndOfText)
                return ' ';

            return (*_dataCache.Value)[Index(newTokenName)][Index(recentTokenName)];
        }
    private:
        Array<String> const _token;
        ValueCache<Array<Array<char>>> const _dataCache;

        p(int, Count){ return _token.Count; };

        int const Index(String name)const
        {
            for (auto i = 0; i < Count; i++)
                if (_token[i] == name)
                    return (i);

            for (auto i = 0; i < Count; i++)
                if (_token[i] == Any)
                    return (i);

            throw "missing " + Any + " entry in priority table";

        }

        Array<Array<char>> const AllocData()const
        {
            return Array<Array<char>>(Count, [=](int){return Array<char>(Count, [](int){return ' '; }); });
        }

    };
}

