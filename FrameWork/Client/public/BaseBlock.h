#pragma once

#ifndef __BASEBLOCK_H__
#define __BASEBLOCK_H__

#include "GameObject.h"

BEGIN(Engine)
class CRenderer;
class CVIBufferCube;
class CTransform;
class CTexture;
END

BEGIN(Client)
class CBaseBlock final
	: public CGameObject
{
private:
	explicit CBaseBlock(_pDevice pGraphicDevice);
	explicit CBaseBlock(const CBaseBlock& rhs);
	virtual ~CBaseBlock() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT SetUp_Component(void* pArg);
	HRESULT Set_RenderState();
	HRESULT Release_RenderState();

private:
	CTransform*		m_pTransformCom = nullptr;
	CVIBufferCube*	m_pVIBufferCom = nullptr;
	CRenderer*		m_pRendererCom = nullptr;
	CTexture*		m_pTextureCom = nullptr;

public:
	virtual void Free() override;
	static CBaseBlock* Create(_pDevice pDevice);
	virtual CGameObject * Clone(void * pArg = (void*)nullptr) override;

public:
	virtual _vec3 Get_MinPos() override;
	virtual _vec3 Get_MaxPos() override;
	virtual _vec3 Get_WorldPos() override;
	virtual _vec3 Get_Length() override;

};
END
#endif // !__BASEBLOCK_H__
