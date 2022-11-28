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
#include <stdexcept>
#include <sys/stat.h>

#undef DeleteFile

using namespace HWLib;

class File::internal final
{
public:
  std::string Name;

  internal(const std::string& name)
    : Name(name)
  {};

  HW_PR_VAR(string, Data);

  HW_PR_VAR(bool, IsValid)
  {
    struct _stat buf{};
    const auto result = _stat(Name.c_str(), &buf);
    return result == 0;
  }

  HW_PR_VAR(bool, IsValidFolder)
  {
    struct _stat buf{};
    const auto result = _stat(Name.c_str(), &buf);
    return result == 0 && (buf.st_mode & _S_IFDIR) != 0;
  }

private:
  void CreateFolder() const
  {
    if(_mkdir(Name.c_str()) == 0)
      return;
    System::ThrowLastErrorMessage();
  }

  void CreateFolderChain() const
  {
    string head;
    for(auto element : (Name | Split("\\"))->ToArray)
    {
      if(head == "")
        head = element;
      else
        head += "\\" + element;
      if(head | EndsWith(":"))
        continue;

      internal headFile = head;
      if(!headFile.IsValid)
        headFile.CreateFolder();
      else if(!headFile.IsValidFolder)
        throw runtime_error("Error: file exits at requested location.");
    };
  };

  void DeleteFolder() const
  {
    if(_rmdir(Name.c_str()) == 0)
      return;
    System::ThrowLastErrorMessage();
  };

  void DeleteFile() const
  {
    if(::remove(Name.c_str()) == 0)
      return;
    System::ThrowLastErrorMessage();
  };
};


HW_PR_IMPL_SETTER(File::internal, bool, IsValidFolder)
{
  if(value)
    CreateFolderChain();
  else
    DeleteFolder();
}

HW_PR_IMPL_SETTER(File::internal, bool, IsValid)
{
  if(value)
    Data = "";
  else
    DeleteFile();
}

FILE* OpenFile(const char* name, bool isWrite)
{
  FILE* file;
  while(file = _fsopen(name, isWrite ? "wbS" : "rbS", _SH_DENYNO), file == nullptr && errno == EACCES)
    continue;
  return file;
}

HW_PR_IMPL_GETTER(File::internal, string, Data)
{
  const auto file = OpenFile(Name.c_str(), false);
  if(file == nullptr)
    return "";

  _fseeki64(file, 0,SEEK_END);
  const auto count = _ftelli64(file);
  string result(count, 0);
  fread(result.data(), result.size(), count, file);
  fclose(file);
  return result;
}

HW_PR_IMPL_SETTER(File::internal, string, Data)
{
  FILE* file = OpenFile(Name.c_str(), true);
  HW_ASSERT(file, string("Error: ") + HWLib::Dump(errno) + ":" + System::FormatLastErrorMessage());

  const auto count = value.size();
  const auto countWritten = fwrite(value.c_str(), count, count, file);
  fclose(file);
  HW_ASSERT(count == countWritten,
       string("Error: ") + HWLib::Dump(errno) + ":" + System::FormatLastErrorMessage() + "\ncount=" + to_string(count) +
       " countWritten=" + to_string(countWritten));
}


File::File(const string& name)
  : _internal(new internal(name)) {}

HW_PR_IMPL_GETTER(File, string, FullName)
{
  const auto temporaryResult = _fullpath(nullptr, Name.c_str(), 0);
  const auto result = string(temporaryResult);
  delete[] temporaryResult;
  return result;
}

HW_PR_IMPL_GETTER(File, string, Name)
{
  return _internal->Name;
}

HW_PR_IMPL_SETTER(File, string, Name)
{
  if(Name == value)
    return;
  const auto rc = rename(Name.c_str(), value.c_str());
  if(rc == 0)
    _internal->Name = value;
  HW_ASSERT_IS(rc, ==, 0);
}

HW_PR_IMPL_GETTER(File, string, Data)
{
  return _internal->Data;
}

HW_PR_IMPL_SETTER(File, string, Data)
{
  _internal->Data = value;
}

HW_PR_IMPL_GETTER(File, bool, IsValid)
{
  return _internal->IsValid;
}

HW_PR_IMPL_GETTER(File, bool, IsValidFolder)
{
  return _internal->IsValidFolder;
}

HW_PR_IMPL_SETTER(File, bool, IsValidFolder)
{
  _internal->IsValidFolder = value;
}
