#pragma once
#include <stdlib.h>
#include <boost/shared_ptr.hpp>
#include <functional>
#include <boost/concept/assert.hpp>
#include <boost/concept/requires.hpp>
#include <boost/concept_check.hpp>
#include "../SetupWarnings.h"

#undef assert

using std::function;
using namespace boost;

#define STRING2(x) #x
#define STRING(x) STRING2(x)
#define ID(x) x
