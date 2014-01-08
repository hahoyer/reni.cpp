#include "Import.h"
#include "Ref.h"

#include "BreakHandling.h"
#include "DumpMacros.h"
#include "DumpToString.h"
#include "String.h"

using namespace HWLib;

class CActiveList
{
public:
    void* This;
    CActiveList* Next;
    CActiveList(void*xThis, CActiveList*xNext) : This(xThis), Next(xNext) {};

    static bool Match(void*);
    virtual operator String()const;
};

static CActiveList* Active;


bool CActiveList::Match(void*x)
{
    if (Active == 0) return false;
    CActiveList* A = Active;
    if (A->This == x)
    {
        Active = A->Next;
        delete A;
        return true;
    };

    for (;;)
    {
        CActiveList* AA = A;
        A = A->Next;
        if (A == 0) return false;
        if (A->This == x)
        {
            AA->Next = A->Next;
            delete A;
            return true;
        };
    };
};


CActiveList::operator String()const
{
    String Return = DumpToString(This, 16);
    if (Next)
        return Return + " " + (*Next);
    else
        return Return;
};


//////////////////////////////////////////////////////////////


template<typename T>
void* RefCounted<T>::operator new(size_t size)
{
    Active = ::new CActiveList(::operator new(size), Active);
#ifdef DUMP_WATCH
    (String("--new-> ") + String(Active->This, 16) + " [" + (*Active) + "]" + "\n").print();
#endif
    return Active->This;
};


template<typename T>
void* RefCounted<T>::operator new (size_t s, void*p){ return ::operator new (s, p); };

template<typename T>
void RefCounted<T>::operator delete(void*p)
{
#ifdef DUMP_WATCH
    (String("--del-> ") + String(p, 16) + "\n").print();
#endif
    ::operator delete(p);
};


String RefSym(int refcount)
{
    if (refcount > 10000 || refcount < 0)
        return String("?") * 20;
    String c = "#";
    if (refcount & 1) c = "*";
    refcount /= 2;
    if (refcount > 20)
        return c * 19 + "...";
    return c*refcount;
};


template<typename T>
RefCounted<T>::RefCounted()
: count(-1)
{
    Initialize();
};


template<typename T>
RefCounted<T>::RefCounted(RefCounted const&)
: count(-1)
{
    Initialize();
};


template<typename T>
void RefCounted<T>::Initialize()
{
    if (Active)
    {
#ifdef DUMP_WATCH
        (String("--act-> [") + (*Active) + "]" + "\n").print();
#endif
        if (Active->Match(this))
            count = 0;
    };
#ifdef DUMP_WATCH
    (String("-ctor-> ") + DumpToString(this, 16) + " " + RefSym(refbits) + "\n").print();
    if (Active)
        (String("--act-> [") + (*Active) + "]" + "\n").print();
#endif
};


template<typename T>
RefCounted<T>::~RefCounted()
{
    assertx(count <= 0, DumpToString(this, 16) + " " + vardump(count));
#ifdef DUMP_WATCH
    (String("-dtor-> ") + DumpToString(this, 16) + " " + RefSym(count) + "\n").print();
#endif
};


template<typename T>
void RefCounted<T>::AddRef()const
{
    if (IsAutomatic)
        return;

    count++;
#ifdef DUMP_WATCH
    (String("--ref-> ") + DumpToString(this, 16) + " " + RefSym(count) + "\t" + dump() + "\n").print();
#endif
};


template<typename T>
void RefCounted<T>::RemoveRef()const
{
    if (IsAutomatic)
        return;

    count--;
#ifdef DUMP_WATCH
    String odump;
    try { odump = dump(); }
    catch (...) { odump = "??"; };
    (String("-uref-> ") + DumpToString(this, 16) + " " + RefSym(count) + "\t" + odump + "\n").print();
#endif
    assertx(count >= 0, String("Illegal unref:") + DumpToString(*this) + vardump(count));
    if (count == 0)
        delete this;
};



/////////////////////////////////////////////////////////////////////////////


