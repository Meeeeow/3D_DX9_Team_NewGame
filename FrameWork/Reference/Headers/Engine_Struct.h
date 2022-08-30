#pragma once
#ifndef __ENGINE_STRUCT_H__
#define __ENGINE_STRUCT_H__

namespace Engine
{
	typedef struct tagVtxCol 
	{
		D3DXVECTOR3			vPos;
		unsigned long		dwColor;
	}VTXCOL;

	typedef struct tagVtxCube
	{
		D3DXVECTOR3			vPos;
		unsigned long		dwColor;
	}VTXCUBECOL;

	typedef struct tagVtxCubeTexture
	{
		D3DXVECTOR3			vPos;
		D3DXVECTOR3			vTexUV;
	}VTXCUBETEX;

	typedef struct tagVtxTexture
	{
		D3DXVECTOR3			vPos;
		D3DXVECTOR2			vTexUV;
	}VTXTEX;

	typedef struct tagVtxTextureColor
	{
		D3DXVECTOR3			vPos;
		DWORD				dwColor;
		D3DXVECTOR2			vTexUV;
	}VTXTEXCOL;

	typedef struct tagIndices16L
	{
		unsigned short _1, _2;
	}INDICES_LINE;

	typedef struct tagIndices16
	{
		unsigned short _1, _2, _3;
	}INDICES_TRI16;

	typedef struct tagIndices32
	{
		unsigned long _1, _2, _3;
	}INDICES_TRI32;

	typedef struct tagAStar
	{
		_vec3 vStartPos;
		_vec3 vGoalPos;
		void* pAStar;
	}ASTAR;

	typedef struct tagPathInfo
	{
		wchar_t szObjectKey[MAX_PATH];
		wchar_t szStateKey[MAX_PATH];
		wchar_t szFilePath[MAX_PATH];
		int dwCount = 0;
	}PATH_INFO;

	typedef struct tagTime
	{
		tagTime() {
			ZeroMemory(this, sizeof(tagTime));
		}

		void Tick_Time(_float fTimeDelta)
		{
			fTime += fTimeDelta;
		}

		_bool Check_Time() {
			if (fTime >= fDelay)
				return true;
			return false;
		}

		_float fDelay;
		_float fTime;
	}TIME;

	typedef struct tagFrame
	{
		tagFrame() {
			iStart = 0; iEnd = 0;
		}

		TIME	tTime;
		int		iStart;
		int		iEnd;

		void Set_Value(int iEnd, int iStart = 0, float fDelay = 0.1f, float fTime = 0.f)
		{
			this->iEnd = iEnd;
			this->iStart = iStart;
			tTime.fDelay = fDelay;
			tTime.fTime = fTime;
		}

		void Tick_Time(_float fTimeDelta)
		{
			tTime.Tick_Time(fTimeDelta);
		}

		_bool Check_Time() {
			return tTime.Check_Time();
		}
	}FRAME;

	typedef struct tagCollisionTag {

		tagCollisionTag()
		{
			ZeroMemory(this, sizeof(tagCollisionTag));
		}

		tagCollisionTag(_uint iFirstLevel, const _tchar* pFirstTag, _uint iSecondLevel, const _tchar* pSecondTag)
		{
			Set_CollisionValue(iFirstLevel, pFirstTag, iSecondLevel, pSecondTag);
		}

		_uint			iFirstLevel;
		const _tchar*	pFirstTag;

		_uint			iSecondLevel;
		const _tchar*	pSecondTag;

		void Set_CollisionValue(_uint iFirstLevel, const _tchar* pFirstTag, _uint iSecondLevel, const _tchar* pSecondTag)
		{
			this->iFirstLevel = iFirstLevel;
			this->pFirstTag = pFirstTag;
			this->iSecondLevel = iSecondLevel;
			this->pSecondTag = pSecondTag;
		}
	} COLLISION_TAG;

}


#endif // !__ENGINE_TYPEDEF_H__
