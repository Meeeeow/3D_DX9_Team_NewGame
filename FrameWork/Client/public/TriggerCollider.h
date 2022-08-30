#pragma once
#ifndef __CLIENT_TIRGGER_COLLIDER_H__
#define __CLIENT_TIRGGER_COLLIDER_H__

#include "GameObject.h"
#include "TutorialMgr.h"

BEGIN(Engine)
class CBoxCollider;
class CRenderer;
class CTrasform;
END

BEGIN(Client)
class CTriggerCollider final:
	public CGameObject
{
public:
	typedef struct tagTriggerColliderDesc {
		tagTriggerColliderDesc() {
			ZeroMemory(this, sizeof(tagTriggerColliderDesc));
			D3DXMatrixIdentity(&tObjInfo.matWorld);
			eTuto = CTutorialMgr::TUTORIAL_END;
			bTrans = true;
		}

		OBJ_INFO	tObjInfo;
		_vec3		vTargetPos;
		_bool		bTrans;
		CTutorialMgr::TUTORIAL eTuto;
	} DESC;
public:
	explicit CTriggerCollider(_pDevice pDevice);
	explicit CTriggerCollider(const CTriggerCollider& rhs);
	virtual ~CTriggerCollider() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public: //SET

public: //GET

private:
	HRESULT SetUp_Component(void* pArg);

private:
	CTransform*		m_pTransform = nullptr;
	CRenderer*		m_pRenderer = nullptr;
	CBoxCollider*	m_pBoxCollider = nullptr;

private:
	_vec3					m_vTargetPos = _vec3(0.f, 0.f, 0.f);
	_bool					m_bTrans = true;
	CTutorialMgr::TUTORIAL	m_eTuto = CTutorialMgr::TUTORIAL_END;

public:
	static CTriggerCollider* Create(_pDevice pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
	virtual void Collision(CGameObject* pColObj) override;

	virtual _vec3 Get_MinPos() override;
	virtual _vec3 Get_MaxPos() override;
	virtual _vec3 Get_WorldPos() override;
	virtual HRESULT Tick_Collider(_float fTimeDelta) override;


	// CGameObject을(를) 통해 상속됨
	virtual _vec3 Get_Length() override;

};
END
#endif // !__CLIENT_CUBE_OBJECT_H__
