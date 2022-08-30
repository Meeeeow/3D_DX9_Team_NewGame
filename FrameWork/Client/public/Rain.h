#pragma once
#ifndef __CLIENT_RAIN_H__
#define __CLIENT_RAIN_H__

#include "Environment.h"
#include "RainDrop.h"

BEGIN(Engine)
class CRenderer;
class CVIBufferRect;
class CTransform;
class CTexture;
END

BEGIN(Client)
class CRain final:
	public CEnvironment
{
public:
	typedef struct tagRainDesc{
		tagRainDesc() {
			ZeroMemory(&tRainDropDesc, sizeof(tagRainDesc));
		}
		CRainDrop::DESC tRainDropDesc;
	} DESC;
public:
	explicit CRain(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CRain(const CRain& rhs);
	virtual ~CRain() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT SetUp_Component();

private:
	DESC m_tRainDropDesc;
	TIME m_tTimeRainClone;

	CTransform* m_pTransform = nullptr;

public:
	virtual void Free() override;
	virtual CGameObject * Clone(void *pArg = (void *)nullptr)override;
	static CRain* Create(_pDevice pDevice);

	// CEnvironment을(를) 통해 상속됨
	virtual _vec3 Get_MinPos() override;
	virtual _vec3 Get_MaxPos() override;
	virtual _vec3 Get_WorldPos() override;
	virtual _vec3 Get_Length() override;
};
END
#endif // !__CLIENT_PLAYER_H__
