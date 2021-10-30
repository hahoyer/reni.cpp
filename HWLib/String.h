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
        std::string _data;
    public:
        String();
        String(char const data);
        String(char const* data);
        String(size_t count, char const* data);
        String(std::string const & other);
        String(Array<char const> const& other);

        DefaultAssignmentOperator;

        p(size_t,       Count      ){ return _data.length(); }
        p(size_t,        HashCode   );
        p(String,         Quote      );
        p(char const*,     RawData    );
        p(char *,           RawDataCopy);
        p(Array<char const>, ToArray  );

        bool    const BeginsWith(String const& target, size_t start = 0)const;
        String   const CastLeft  (size_t count, char padChar = ' ')const;
        String    const CastRight (size_t           count, char padChar = ' ')const;
        bool       const Contains  (char const&   target, size_t start = 0)const;
        bool        const Contains  (String const& target, size_t start = 0)const;
        bool         const EndsWith  (String const& target)const;
        Optional<size_t> const Find      (String const& target, size_t start = 0)const;
        String         const Indent    (bool          isLineStart = false, size_t count = 1, String const &tabString = "    ")const;
        String          const operator+ (String const& other)const;
        String           const operator* (size_t           count)const;
        char              const operator[](size_t           count)const;
        bool               const operator==(String const& other)const;
        bool                const operator< (String const& other)const;
        void                       operator+=(String const& other);
        String                const Part    (size_t           start, size_t length)const;
        String                 const Part   (size_t           start)const;
        String                  const Replace(String const& oldValue, String const&newValue)const;
        CtrlRef<Enumerable<String> > const Split(String const& delimiter)const;

        static String const Convert(bool             value);
        static String const Convert(int              value, int radix = 10){return Convert(static_cast<long long>(value), radix);};
        static String const Convert(unsigned __int32 value, int radix = 10){return Convert(static_cast<long long>(value), radix);};
        static String const Convert   (unsigned __int64 value, int radix = 10);
        static String const Convert    (__int64         value, int radix = 10);
        static String const FilePosition(String const&  fileName, size_t line, size_t column, String const&flag);
        static String const Surround   (String const&  left, Array<String> const&list, String const&right, int maxCount = 100);
        static String const Stringify(Enumerable<String> const&list, String const&delimiter);
    };

    static String const operator+ (char const*left, String const& right){ return String(left) + right; };
}

namespace std {
    template <>
    struct hash<String const>{
        size_t operator()(String const& key) const{
            return key.HashCode;
        }
    };

    template <>
    struct hash<String >{
        size_t operator()(String const& key) const{
            return key.HashCode;
        }
    };
}
//#pragma message(__FILE__ "(" STRING(__LINE__) "):")
