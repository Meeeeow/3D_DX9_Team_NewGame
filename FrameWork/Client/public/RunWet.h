#pragma once
#ifndef __CLIENT_RUN_WET_H__
#define __CLIENT_RUN_WET_H__

#include "GameObject.h"

BEGIN(Engine)
class CRenderer;
class CVIBufferRect;
class CTransform;
class CTexture;
class CBoxCollider;
END

BEGIN(Client)
class CRunWet final:
	public CGameObject
{
public:
	typedef struct tagRunWetDesc {
		tagRunWetDesc() {
			D3DXMatrixIdentity(&matWorld);
		}
		_mat matWorld;
	} DESC;
private:
	explicit CRunWet(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CRunWet(const CRunWet& rhs);
	virtual ~CRunWet() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;

public: //GET

private:
	//SetUp
	HRESULT SetUp_Component(void *pArg);

private: //Component
	CTransform*		m_pTransform = nullptr;

public:
	static CRunWet* Create(_pDevice pDevice);
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
