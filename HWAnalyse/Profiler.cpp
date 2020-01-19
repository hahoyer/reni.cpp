#include "Import.h"
#include "Profiler.h"

#include "Profiler.include.h"
#include <algorithm>

using namespace HWAnalyse;
using namespace std;


__int64 Profiler::internal::SuspendTime = 0;
__int64 Profiler::internal::ResumeTime = 0;

void Profiler::internal::Start(const char*FileName, int Line, const char*Flag){

    if(Top)
        Top->Suspend(SuspendTime);

    Entry* entry = Find(FileName,Flag);
    if(entry == NULL){
        entry = new Entry(FileName, Line, Flag);
        Entries.push_back(entry);
    };

    Stack.push_back(entry);
    Top->counter++;
    Top->Resume(ResumeTime);
};

void Profiler::internal::End(){
    auto top = Top;
    if(top == NULL)
        return;
    top->Suspend();
    Stack.pop_back();
    if(Top)
        Top->Resume();
};

void Profiler::internal::Reset(){
    while(Top)
        End();
    for(auto entry: Entries)
        delete entry;
};

p_implementation(Profiler::internal, int, count){
    auto result = 0;
    for(auto entry : Entries)
        result++;
    return result;
}

p_implementation(Profiler::internal, __int64, ticks){
    __int64 result = 0;
    for(auto entry : Entries)
        result += entry->ticks;
    return result;
}

void Profiler::internal::CalibrationValues(){
    for(auto counter = 0; counter<1000; counter++){
        Start("***Calibrate***", -1, "suspend time");
        Start("***Calibrate***", -1, "resume time");
        End();
        End();
    };

    for(auto entry : Entries)
        entry->CalibrationValues(SuspendTime,ResumeTime);
    SuspendTime -= ResumeTime;
}

Profiler::internal::Entry *const Profiler::internal::Find(char const* FileName, char const* Flag) const{
    for(auto entry : Entries)
        if(strcmp(FileName, entry->FileName) == 0 && Flag == entry->Flag)
            return entry;
    return{};
}

void Profiler::internal::dumpprint(int Hide)const{
    if(isEmpty){
        ::dumpprint("\n\r=========== Profile empty ============\n\r");
        return;
    }

    ::dumpprint("\n\r=========== Profile ==================\n\r");

    vector<Entry*> entries = Entries;
    std::sort(entries.begin(), entries.end(), Entry::PointerLessThan());

    auto All = ticks;
    auto ShowTime = All;
    auto TimeNotShown = All;
    int ShowCount = count;
    if(Hide>0)
        ShowTime -= All / Hide;
    else if(-Hide > 0 && -Hide < count)
        ShowCount = -Hide;
    int LFileName = 0;

    int i = 0;
    for(; i < ShowCount && ShowTime > 0; i++){
        auto That = entries[i];
        __int64 ticks = That->ticks;
        if(ticks < 0) ticks = 0;
        ShowTime -= ticks;
        TimeNotShown -= ticks;
        int iLFileName = ::strlen(That->FileName);

        if(LFileName < iLFileName)
            LFileName = iLFileName;
    };
    ShowCount = i;

    for(i = 0; i < ShowCount; i++){
        auto That = entries[i];
        __int64 ticks = That->ticks;
        if(ticks < 0) ticks = 0;
        ::dumpprint(That->FileName);
        ::dumpprint("(");
        int LLine = ::dumpprint(That->Line);
        ::dumpprint("):");

        int Spaces = LFileName + 6 - ::strlen(That->FileName) - LLine;
        while(Spaces > 0){
            ::dumpprint(" ");
            --Spaces;
        };

        ::dumpprint(i);
        ::dumpprint(":\t");
        int counter = ::dumpprint(That->counter);
        ::dumpprint("x");
        while(counter++ < 6) ::dumpprint(" ");
        ::dumpprint("\t");
        ::dumpprinttime(ticks, That->counter);
        ::dumpprint(" \t");
        ::dumpprinttime(ticks, 1);
        ::dumpprint(" \t");
        ::dumpprint(__int64(100.0*float(ticks) / float(All)));
        ::dumpprint("%\t");
        ::dumpprint(That->Flag);
        ::dumpprint("\n\r");
    };

    ::dumpprint("Total: \t");
    ::dumpprinttime(All, 1);

    if(i < count){
        ::dumpprint(" (");
        ::dumpprint(count - i);
        ::dumpprint(" not-shown-items ");
        ::dumpprinttime(TimeNotShown, 1);
        ::dumpprint(")");
    };

    ::dumpprint("\n\r======================================\n\r");

};


Profiler::Profiler()
    : _internal(*new internal){
    _internal.CalibrationValues();
};

Profiler::~Profiler(){delete &_internal;}
void Profiler::Start(const char*FileName, int Line, const char*Flag){return _internal.Start(FileName, Line, Flag);}
void Profiler::End(){return _internal.End();}
void Profiler::Reset(){return _internal.Reset();}
void Profiler::dumpprint(int Hide)const{_internal.dumpprint(Hide);}


void Profiler::internal::Entry::CalibrationValues(long long&suspend, long long&resume) const{
    switch(*Flag){
    case 's':
        suspend = ticks / counter;
        break;
    case 'r':
        resume = ticks / counter;
        break;
    };
};

