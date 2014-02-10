#pragma once
#include <stdlib.h>
#include <boost/shared_ptr.hpp>
#include <functional>
#include <boost/concept/assert.hpp>
#include <boost/concept/requires.hpp>
#include <boost/concept_check.hpp>
#include "../SetupWarnings.h"
#include <initializer_list>

#undef assert

using std::function;
using namespace boost;

template<typename T>
using List = std::initializer_list<T>;

#define STRING2(x) #x
#define STRING(x) STRING2(x)
#define ID(x) x
