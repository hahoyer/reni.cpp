#include "Import.h"
#include "Compiler.h"

namespace Reni
{
    class Source
    {
        String const _fileName;
    public:
        Source(String const& fileName) :_fileName(fileName){}
    };
}
using namespace Reni;

class Compiler::internal
{
    String const _fileName;
    ValueCache<Source> const _source;
public:
    internal() = delete;
    internal(internal const&) = delete;

    internal(String const&fileName)
        : _fileName(fileName)
        , _source([&]{return _fileName; })
    {
    }

};


Compiler::Compiler(String const&fileName)
: _internal(*new internal(fileName))
{}

Compiler::~Compiler()
{
    _(_internal).Delete();
}
