#include "Import.h"
#include "File.h"

#include <fcntl.h>
#include <io.h>
#include <share.h>
#include <sys/stat.h>

#include "BreakHandling.h"
#include "Console.h"
#include "DumpMacros.h"
#include "DumpToString.h"
#include "Storage.h"
#include "String.h"

using namespace HWLib;

class File::local
{
public:
    String _name;
    local(String const& other);
};
   
File::File(String const& other)
: _data(HeapAllocate local(other))
{
}

File::~File(){HeapFree _data;}

String const File::get_FullName()const
{
    auto tReturn = ::_fullpath(0, Name.Data, 0);
    auto Return = String(tReturn);
    delete [] tReturn;
    return Return;
}

String const File::get_Name()const{return _data._name;}

void File::set_Name(String const& value)
{
    auto rc = ::rename(Name.Data, value.Data);
    if (rc == 0)
        _data._name = value;
    assertx(rc == 0, vardump(rc));
}

static int _openX(char const* name, int oflag, int pmode = _S_IREAD | _S_IWRITE)
{
    int result;
    auto rc = _sopen_s(&result, name, oflag, SH_DENYNO, pmode);
    return result;
};

String const File::get_Data()const
{ 
    auto Handle = ::_openX(Name.Data, _O_RDONLY | _O_BINARY);
    auto Length = ::_filelength(Handle);
    if (Handle < 0) Length = 0;
    auto cReturn = new char[Length + 1];
    if (Handle >= 0) ::_read(Handle, cReturn, Length);
    cReturn[Length] = 0;
    ::_close(Handle);
    auto Return = String(cReturn);
    delete cReturn;
    return Return;
}

void File::set_Data(String const& value)
{
    auto rc = ::rename(Name.Data, value.Data);
    if (rc == 0)
        _data._name = value;
    assertx(rc == 0, vardump(rc));
}
