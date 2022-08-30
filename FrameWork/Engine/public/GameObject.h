#pragma once
#ifndef __ENGINE_GAMEOBJECT_H__
#define __ENGINE_GAMEOBJECT_H__

#include "Base.h"
BEGIN(Engine)

class ENGINE_DLL CGameObject abstract :
	public CBase
{
public:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int	Tick(_float fTimeDelta);
	virtual _int	LateTick(_float fTimeDelta);
	virtual HRESULT Render();


public: // Collision
	virtual _vec3	Get_MinPos() PURE;
	virtual _vec3	Get_MaxPos() PURE;
	virtual _vec3	Get_MaxProjPos();
	virtual _vec3	Get_WorldPos() PURE;
	virtual _vec3	Get_Length() PURE;
	virtual _bool	Get_Collision() { return m_bCollision; };

public: //GET
	class CComponent*	Get_Component(const _tchar* szComponentTag);
	_bool				Get_Dead() { return m_bDead; }

	_bool				Check_Component(const _tchar* szComponentTag);

public: //SET
	//OBJ_INFO 셋 안하는 친구들은 그냥 넘기면 됨. 안까먹게 하기 위함
	virtual HRESULT Set_ObjInfo(void* pArg);
	virtual void Set_Dead() { m_bDead = true; }
	void Set_Collision(_bool bCollision) { m_bCollision = bCollision; }
	HRESULT Set_Pos(_vec3 vPos, class CTransform* pTransform);

	//인자로 충돌한 상대 객체 넘겨줌
	virtual void Collision(CGameObject* pColObj);

public:
	virtual HRESULT Tick_Collider(_float fTimeDelta);

protected:
	LPDIRECT3DDEVICE9	m_pGraphicDevice = nullptr;
	_bool				m_bDead = false;
	_bool				m_bCollision = false;

protected:
	unordered_map<const _tchar*, class CComponent*>			m_mapComponent;
	typedef unordered_map<const _tchar*, class CComponent*>	COMPONENTS;
protected:
	HRESULT Add_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, const _tchar* szComponentTag, CComponent** ppOut, void* pArg = nullptr);

public:
	virtual void Free() override;
	virtual CGameObject* Clone(void* pArg = nullptr) = 0;
};
END
#endif // !__ENGINE_GAMEOBJECT_H__
