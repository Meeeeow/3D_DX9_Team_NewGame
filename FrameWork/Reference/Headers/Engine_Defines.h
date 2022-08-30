#pragma once

#pragma warning (disable : 4251)

#include "d3d9.h"
#include "d3dx9.h"

#define DIRECTINPUT_VERSION 0x0800
#include "dinput.h"

#include <string>
#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <map>
#include <list>
#include <vector>
#include <typeinfo.h>
#include <process.h>

using namespace std;

#include "Engine_Macro.h"
#include "Engine_Typedef.h"
#include "Engine_Function.h"
#include "Engine_Functor.h"
#include "Engine_Struct.h"
#include "Engine_Const.h"

#ifndef _AFX

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif

#endif // _DEBUG

#endif // !_AFX

using namespace Engine;