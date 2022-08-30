#pragma once
#ifndef __CLIENT_BLOOD_SCREEN_H__
#define __CLIENT_BLOOD_SCREEN_H__

#include "OrthoObj.h"
BEGIN(Engine)
class CTransform;
class CRenderer;
class CVIBufferRect;
class CTexture;
END

class CBloodScreen final:
	public COrthoObj
{
public:

	typedef struct tagBloodScreenDesc {
		tagBloodScreenDesc() {
			ZeroMemory(this, sizeof(tagBloodScreenDesc));
		}
		COrthoObj::DESC tSuperDesc;
	}DESC;

public:
	explicit CBloodScreen(_pDevice pDevice);
	explicit CBloodScreen(const CBloodScreen& rhs);
	virtual ~CBloodScreen() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int	Tick(_float fTimeDelta);
	virtual _int	LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	virtual _vec3 Get_MinPos() override;
	virtual _vec3 Get_MaxPos() override;
	virtual _vec3 Get_WorldPos() override;
	virtual _vec3 Get_Length() override;
	virtual void Free() override;
	static CBloodScreen* Create(_pDevice pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr);
};

#endif // !__CLIENT_ORTHO_OBJ_H__
