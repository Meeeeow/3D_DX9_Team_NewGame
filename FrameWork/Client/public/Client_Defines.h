#pragma once
#ifndef __CLIENT_DEFINES_H__
#define __CLIENT_DEFINES_H__

static unsigned int g_iWinCX = 1280;
static unsigned int g_iWinCY = 720;

enum LEVEL { LEVEL_STATIC, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_TEST, LEVEL_BOSS_SKELETON, LEVEL_BOSS_CIRCUS, LEVEL_BOSS_NECROMENCER, LEVEL_LOADING, LEVEL_END };
enum CAMERAS { CAMERA_BACK, CAMERA_2D, CAMERA_END};

extern HWND g_hWnd;
extern HINSTANCE g_hInstance;

#include <d3d9.h>
#include <d3dx9.h>
#include <vector>

using namespace std;

#include "Client_Struct.h"
#include "Client_Const.h"
#include "SoundMgr.h"

#define	PLAY_SOUND CSoundMgr::Get_Instance()->StopPlaySound

#endif // !__CLIENT_DEFINES_H__
