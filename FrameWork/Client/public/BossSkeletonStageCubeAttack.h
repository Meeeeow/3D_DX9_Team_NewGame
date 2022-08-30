#pragma once
#ifndef __CLIENT_BOSS_SKELETON_STAGE_CUBE_ATTACK_H__
#define __CLIENT_BOSS_SKELETON_STAGE_CUBE_ATTACK_H__

BEGIN(Engine)
class CTransform;
class CTexture;
class CRenderer;
class CVIBufferCube;
class CBoxCollider;
class CFrustum;
END

#include "BossSkeletonStageCube.h"
#include "EnvironmentMgr.h"

BEGIN(Client)
class CBossSkeletonStageCubeAttack :
	public CBossSkeletonStageCube
{
public:
	enum TYPE { TYPE_NORMAL_WEAK, TYPE_NORMAL_STONE, TYPE_WET, TYPE_SMALL_STONE, TYPE_ICE, TYPE_MAGMA, TYPE_END};

	typedef struct tagBossSkeletonStageCubeDesc {
		tagBossSkeletonStageCubeDesc() {
			ZeroMemory(this, sizeof(tagBossSkeletonStageCubeDesc));
		}
		OBJ_INFO	tObjInfo;
		TYPE		eType;

		//작은 돌 사출하는 힘
		_float		fPower;

		//재활용 위한 변수
		_bool		bNotBoss;
		_vec3		vTargetPos;
	}DESC;

public:
	explicit CBossSkeletonStageCubeAttack(_pDevice pDevice);
	explicit CBossSkeletonStageCubeAttack(const CBossSkeletonStageCubeAttack& rhs);
	virtual ~CBossSkeletonStageCubeAttack() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public: //SET
	void Set_State(TYPE eType);
	virtual void Set_Env(CEnvironmentMgr::TYPE eType) override;

public: //GET

public:
	virtual void Collision(CGameObject* pColObj) override;

private:
	HRESULT Tick_Stone(_float fTimeDelta, CGameInstance* pGameInstance);
	HRESULT Tick_SmallStone(_float fTimeDelta, CGameInstance* pGameInstance);
	HRESULT Tick_Wet(_float fTimeDelta, CGameInstance* pGameInstance);
	HRESULT Tick_NormalWeak(_float fTimeDelta, CGameInstance* pGameInstance);
	HRESULT Tick_Magma(_float fTimeDelta, CGameInstance* pGameInstance);

	void	Dead_EventNormalStone(_float fTimeDelta, CGameInstance* pGameInstance);
	void	Dead_EventSmallStone(_float fTimeDelta, CGameInstance* pGameInstance);

private:
	TYPE	m_eType = TYPE_END;

	//stone
	_vec3			m_vProjectilePower = _vec3(0.f, 0.f, 0.f);
	_vec3			m_vStartPos = _vec3(0.f, 0.f, 0.f);
	_float			m_fStoneProjectileTime = 0.f;
	const _float	m_fStoneProjectileMaxTime = 2.f;
	const _float	m_fStoneProjectileDegree = 45.f;

	//small stone
	_float			m_fNX = 1;

	//wet
	//힘, 시작 지점은 스톤꺼 갖다쓰자
	_float			m_fWetProjectileTime = 0.f;
	const _float	m_fWetProjectileMaxTime = 5.f;
	const _float    m_fWetProjectileDegree = 60.f;
	TIME			m_tIceTime;

	//normal weak
	_float			m_fNormalWeakDist = 0.f;

public:
	static CBossSkeletonStageCubeAttack* Create(_pDevice pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};
END
#endif // !__CLIENT_CUBE_OBJECT_H__
