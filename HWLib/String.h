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
        String(int count, char const* data);
        String(std::string const & other);
        String(Array<char const> const& other);

        DefaultAssignmentOperator;

        p(int,          Count      ){ return _data.length(); }
        p(std::size_t,   HashCode   );
        p(String,         Quote      );
        p(char const*,     RawData    );
        p(char *,           RawDataCopy);
        p(Array<char const>, ToArray  );

        bool    const BeginsWith(String const& target, int start = 0)const;
        String   const CastLeft  (int           count, char padChar = ' ')const;
        String    const CastRight (int           count, char padChar = ' ')const;
        bool       const Contains  (char const&   target, int start = 0)const;
        bool        const Contains  (String const& target, int start = 0)const;
        bool         const EndsWith  (String const& target)const;
        Optional<int> const Find      (String const& target, int start = 0)const;
        String         const Indent    (bool          isLineStart = false, int count = 1, String const &tabString = "    ")const;
        String          const operator+ (String const& other)const;
        String           const operator* (int           count)const;
        char              const operator[](int           count)const;
        bool               const operator==(String const& other)const;
        bool                const operator< (String const& other)const;
        void                       operator+=(String const& other){ *this = *this + other; };
        String                const Part    (int           start, int length)const;
        String                 const Part   (int           start)const;
        String                  const Replace(String const& oldValue, String const&newValue)const;
        CtrlRef<Enumerable<String> > const Split(String const& delimiter)const;

        static String const Convert(bool             value);
        static String const Convert(int              value, int radix = 10);
        static String const Convert(unsigned __int32 value, int radix = 10);
        static String const Convert   (unsigned __int64 value, int radix = 10);
        static String const Convert    (__int64         value, int radix = 10);
        static String const FilePosition(String const&  fileName, int line, int column, String const&flag);
        static String const Surround   (String const&  left, Array<String> const&list, String const&right, int maxCount = 100);
    };

    static String const operator+ (char const*left, String const& right){ return String(left) + right; };
}

namespace std {

    template <>
    struct hash<String const>{
        std::size_t operator()(String const& key) const{
            return key.HashCode;
        }
    };

    template <>
    struct hash<String >{
        std::size_t operator()(String const& key) const{
            return key.HashCode;
        }
    };

}
//#pragma message(__FILE__ "(" STRING(__LINE__) "):")
