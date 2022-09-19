#pragma once

#include "Macro.h"

#ifdef STACK_VECTOR
#include "Stack_Vector.h"
#else
#include "Stack_List.h"
#endif