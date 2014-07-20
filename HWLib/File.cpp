#include "Import.h"
#include "File.h"

#include "Common.h"
#include "Console.h"
#include "DumpableObject.h"
#include "DumpMacros.h"
#include "DumpToString.h"
#include "String.h"
#include "Optional.h"
#include "System.h"
#include "TemplateInstances.h"

#include <direct.h>
#include <fcntl.h>
#include <io.h>
#include <sys/stat.h>
#include <windows.h>

#undef DeleteFile

using namespace HWLib;

class File::internal final{
public:
    String _name;
    internal(String const& name) : _name(name){};

    p_mutable(String, Data){
        auto Handle = OpenFile(_O_RDONLY | _O_BINARY);
        auto Count = ::_filelength(Handle);
        if (Handle < 0) Count = 0;
        auto cReturn = new char[Count + 1];
        if (Handle >= 0) ::_read(Handle, cReturn, Count);
        cReturn[Count] = 0;
        ::_close(Handle);
        auto Return = String(cReturn);
        delete cReturn;
        return Return;
    }

    p_mutable(bool, IsValid){
        struct _stat buf;
        auto result = _stat(_name.RawData, &buf);
        return result == 0;
    }

    p_mutable(bool, IsValidFolder){
        struct _stat buf;
        auto result = _stat(_name.RawData, &buf);
        return result == 0 && (buf.st_mode & _S_IFDIR) != 0;
    }

private:
    void CreateFolder(){
        if (::_mkdir(_name.RawData) == 0)
            return;
        throw System::FormatLastErrorMessage();
    }

    void CreateFolderChain(){
        String head;
        for (auto element : _name.Split("\\")->ToArray){
            if (head == "")
                head = element;
            else
                head += "\\" + element;
            if (head.EndsWith(":"))
                continue;

            internal headFile = head;
            if (!headFile.IsValid)
                headFile.CreateFolder();
            else if (!headFile.IsValidFolder)
                throw String("Error: file exits at requested location.");
        };
    };

    void DeleteFolder(){
        if (::_rmdir(_name.RawData) == 0)
            return;
        throw System::FormatLastErrorMessage();
    };

    void DeleteFile(){
        if (::remove(_name.RawData) == 0)
            return;
        throw System::FormatLastErrorMessage();
    };

    int OpenFile(int oflag, int pmode = _S_IREAD | _S_IWRITE)const{
        int result;
        auto rc = ::_sopen_s(&result, _name.RawData, oflag, SH_DENYNO, pmode);
        return result;
    };

};


p_mutator_implementation(File::internal, bool, IsValidFolder){
    if (value)
        CreateFolderChain();
    else
        DeleteFolder();
}

p_mutator_implementation(File::internal, bool, IsValid){
    if (value)
        Data = "";
    else
        DeleteFile();
}

p_mutator_implementation(File::internal, String, Data){
    auto Count = value.Count;

    int Handle;
    do Handle = OpenFile(_O_CREAT | _O_TRUNC | _O_WRONLY | _O_BINARY);
    while (Handle < 0 && errno == EACCES);

    auto Error = System::FormatLastErrorMessage();
    auto e = errno;

    a_if(Handle >= 0, String("Error: ") + HWLib::Dump(e) + ":" + Error);

    auto WrLength = _write(Handle, value.RawData, Count);
    a_is(WrLength, == , Count);

    _close(Handle);
}



File::File(String const& name)
: _internal(new internal(name)){}

p_implementation(File, String, FullName){
    auto tReturn = ::_fullpath(0, Name.RawData, 0);
    auto Return = String(tReturn);
    delete[] tReturn;
    return Return;
}

p_implementation(File, String, Name){
    return _internal->_name;
}

p_mutator_implementation(File, String, Name){
    if (Name == value)
        return;
    auto rc = ::rename(Name.RawData, value.RawData);
    if (rc == 0)
        _internal->_name = value;
    a_is(rc, == , 0);
}

p_implementation(File, String, Data){
    return _internal->Data;
}

p_mutator_implementation(File, String, Data){
    _internal->Data = value;
}

p_implementation(File, bool, IsValid){
    return _internal->IsValid;
}

p_implementation(File, bool, IsValidFolder){
    return _internal->IsValidFolder;
}

p_mutator_implementation(File, bool, IsValidFolder){
    _internal->IsValidFolder = value;
}
