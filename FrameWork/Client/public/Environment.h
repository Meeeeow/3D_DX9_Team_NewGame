#pragma once
#ifndef __CLIENT_ENVIRONMENT_H__
#define __CLIENT_ENVIRONMENT_H__

#include "GameObject.h"

BEGIN(Engine)
class CRenderer;
class CVIBufferRect;
class CTransform;
class CTexture;
END

BEGIN(Client)
class CEnvironment abstract:
	public CGameObject
{
public:

public:
	explicit CEnvironment(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CEnvironment(const CEnvironment& rhs);
	virtual ~CEnvironment() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() PURE;
	virtual HRESULT NativeConstruct(void* pArg) PURE;
	virtual _int Tick(_float fTimeDelta) PURE;
	virtual _int LateTick(_float fTimeDelta) PURE;
	virtual HRESULT Render() PURE;

public:
	virtual void Free() override;
	virtual CGameObject * Clone(void *pArg = (void *)nullptr)PURE;
};
END
#endif // !__CLIENT_PLAYER_H__
