#pragma once
#ifndef __CLIENT_ITEM_H__
#define __CLIENT_ITEM_H__

#include "GameObject.h"

BEGIN(Engine)
class CRenderer;
class CVIBufferRect;
class CTransform;
class CTexture;
END

BEGIN(Client)
class CItem abstract :
	public CGameObject
{
public:
	typedef struct tagMonsterStat {
		tagMonsterStat() { ZeroMemory(this, sizeof(*this)); }
		_int	iHp;
		_int	iMaxHp;
		_int	iDmg;

	}MONSTER_STAT;

public:
	explicit CItem(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CItem(const CItem& rhs);
	virtual ~CItem() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public: //SET

public: //GET

protected:
	virtual void Collision_Terrain(_float fTimeDelta, class CGameInstance* pInstance, class CTransform* pTransform);
	virtual HRESULT BillBoarding(class CTransform* pTransform);

	virtual HRESULT SetUp_RenderState();
	virtual HRESULT Release_RenderState();

protected:
	FRAME				m_tFrame;
	MONSTER_STAT		m_tStat;

	_bool				m_bPoison = false;
	TIME				m_tTimePoisonTick;
	TIME				m_tTimePoisonEnd;
	_int				m_iPoisonDmg = 0;

	TIME				m_tTimePoisonCreateHitParticle;
	_bool				m_bCreateRight = false;
public:
	virtual void Free() override;
	virtual CGameObject * Clone(void *pArg = (void *)nullptr)PURE;

	virtual _vec3 Get_MinPos() override;
	virtual _vec3 Get_MaxPos() override;
	virtual _vec3 Get_WorldPos() override;
	virtual _vec3 Get_Length() override;

};
END
#endif // !__CLIENT_PLAYER_H__
