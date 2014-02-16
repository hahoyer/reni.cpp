#pragma once
//#pragma message(__FILE__ "(" STRING(__LINE__) "): ")

#include "Array.h"
#include "DefaultAssignmentOperator.h"
#include "Properties.h"
#include <string>

namespace HWLib
{
    template<typename T> class Optional;

    class String final
    {
        using thisType = String;
        std::string const _data;
    public:
        String();
        String(char const data);
        String(char const* data);
        String(std::string const & other);
        String(Array<char const> const& other);

        DefaultAssignmentOperator;

        p(int,            Count  ){ return _data.length(); }
        p(String,          Quote  );
        p(char const*,      RawData);
        p(Array<char const>, ToArray);

        bool       const BeginsWith(String const& target, int start = 0)const;
        String      const CastLeft  (int           count, char padChar = ' ')const;
        String       const CastRight (int           count, char padChar = ' ')const;
        bool          const Contains  (char const&   target, int start = 0)const;
        Optional<int>  const Find      (String const& target, int start = 0)const;
        String          const Indent    (bool          isLineStart = false, int count = 1, String const &tabString = "    ")const;
        String           const operator+ (String const& other)const;
        String            const operator* (int           count)const;
        char               const operator[](int           count)const;
        bool                const operator==(String const& other)const;
        void                       operator+=(String const& other){*this = *this + other;};
        String                const Part    (int           start, int length)const;
        String                 const Part   (int           start)const;
        String                  const Replace(String const& oldValue, String const&newValue)const;
        Ref<Enumerable<String> > const Split(String const& delimiter)const;

        static String const Convert   (bool value);
        static String const Convert    (int value);
        static String const FilePosition(String const&fileName, int line, int column, String const&flag);
        static String const Surround   (String const&left, Array<String> const&list, String const&right, int maxCount = 100);
    };

    static String const operator+ (char const*left, String const& right){ return String(left) + right; };
}

//#pragma message(__FILE__ "(" STRING(__LINE__) "):")
