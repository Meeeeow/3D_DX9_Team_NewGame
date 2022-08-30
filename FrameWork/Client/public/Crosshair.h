#pragma once
#ifndef __CLIENT_CROSSHAIR_H__
#define __CLIENT_CROSSHAIR_H__

#include "GameObject.h"

BEGIN(Engine)
class CRenderer;
class CVIBufferRect;
class CTransform;
class CTexture;
END

BEGIN(Client)
class CCrosshair final:
	public CGameObject
{
public: 
	typedef struct tagCrosshairDesc {
		OBJ_INFO tObjInfo;
	} DESC;
private:
	explicit CCrosshair(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CCrosshair(const CCrosshair& rhs);
	virtual ~CCrosshair() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public: //SET
	void Set_Render(_bool bRender) { m_bRender = bRender; }

public: //GET
	const _vec3 Get_Ray() const { return m_vCrosshairRay; }
	_mat		Get_Projection() { return m_matProject; }
public:
	_bool Picking(const _tchar* szTransformTag, const _tchar* szBufferTag, class CGameObject* pTarget, _vec3* pOut);
	_bool Picking(const _tchar* szTransformTag, const _tchar* szBufferTag, class CMonster* pTarget);
private:
	HRESULT SetUp_Component(void *pArg);

	//Render
	HRESULT Set_RenderState();
	HRESULT Release_RenderState();

	HRESULT Tick_CrosshairRay(CGameInstance* pGameInstance);



private: //Component
	CTransform*		m_pTransform = nullptr;
	CVIBufferRect*	m_pVIBuffer = nullptr;
	CRenderer*		m_pRenderer = nullptr;
	CTexture*		m_pTexture = nullptr;

private:
	_mat			m_matProject;
	_bool			m_bRender = true;

	_vec3			m_vCrosshairRay = _vec3(0.f, 0.f, 0.f);
	_vec3			m_vCrosshairPivot = _vec3(0.f, 0.f, 0.f);
public:
	static CCrosshair* Create(_pDevice pDevice);
	virtual void Free() override;
	virtual CGameObject * Clone(void *pArg = (void *)nullptr);

	

	// CMonster을(를) 통해 상속됨
	virtual _vec3 Get_MinPos() override;

	virtual _vec3 Get_MaxPos() override;

	virtual _vec3 Get_WorldPos() override;

	virtual Engine::_vec3 Get_Length();

};
END
#endif // !__CLIENT_PLAYER_H__
