#pragma once

#include <windows.h>
#include <cstdint>
#include <numbers>
#include <cmath>
#include <list>
#include <vector>
#include <stdio.h>
#include <string>
#include <thread>
#include <fstream>
#include <iostream>
#include <Psapi.h>
using namespace std::chrono_literals;


///macros
#define C_PTR( x ) ((void*) x )
#define U_PTR( x ) ((PULONG64) x )
#define PointerToOffset(Base, Offset)		((ULONG64)(((ULONG64)(Base)) + ((ULONG64)(Offset))))
#define OffsetToPointer(Pointer, Base)		((ULONG64)(((ULONG64)(Pointer)) - ((ULONG64)(Base))))


/// 1 radian = 57.295779513 deg
#define _1RAD 57.295779513
///PIE
#define M_PI 3.141592653589793238463

///size of view matrix memory
#define _MATRIX 64

///maxPlayers
#define _MAX_PLAYERS 64

///api
#define STATUS_SUCCESS 0x0








