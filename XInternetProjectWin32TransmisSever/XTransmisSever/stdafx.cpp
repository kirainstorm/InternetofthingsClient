// stdafx.cpp : source file that includes just the standard includes
// XTransmisSever.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file
map<int, MyStruct4Process *> g_mapProcess;
CrossCriticalSection g_csProcessSession;