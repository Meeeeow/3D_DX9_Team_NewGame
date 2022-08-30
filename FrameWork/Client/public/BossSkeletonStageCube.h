#pragma once
#ifndef __CLIENT_BOSS_SKELETON_STAGE_CUBE_H__
#define __CLIENT_BOSS_SKELETON_STAGE_CUBE_H__

BEGIN(Engine)
class CTransform;
class CTexture;
class CRenderer;
class CVIBufferCube;
class CBoxCollider;
class CFrustum;
END

#include "TileCube.h"
#include "EnvironmentMgr.h"

BEGIN(Client)
class CBossSkeletonStageCube :
	public CTileCube
{
public:
	typedef struct tagBossSkeletonStageCubeDesc {
		OBJ_INFO tObjInfo;
	}DESC;

	enum FSM { FSM_NORMAL, FSM_WEAK_NORMAL, FSM_WET, FSM_ICE, FSM_MAGMA,
		FSM_DEAD_NORMAL, FSM_DEAD_ICE, FSM_END};

public:
	explicit CBossSkeletonStageCube(_pDevice pDevice);
	explicit CBossSkeletonStageCube(const CBossSkeletonStageCube& rhs);
	virtual ~CBossSkeletonStageCube() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public: //SET
	void Set_State(FSM eState);
	void Set_TutorialMagma();
	//불변수 전부 초기화
	void Clear_Env();
	virtual void Set_Env(CEnvironmentMgr::TYPE eType);
	void Set_FrameValue(_uint iStart) { CTileCube::Set_TextureIndex(iStart); }
	void Set_FrustrumRadius(_float fFrustrumRadius) { m_fFrustumRadius = fFrustrumRadius; }

public: //GET
	_vec3 Get_TransformScale() { return m_pTransform->Get_Scale(); }
	const FSM& Get_FSM() const { return m_ePreState; }
public:
	virtual void Collision(CGameObject* pColObj) override;

private:
	//Tick
	HRESULT Check_Frame(_float fTimeDelta);
	//노말, 마그마 상태일때 비오고 있으면 일정 확률로 타일 바뀜
	HRESULT Tick_Rain(_float fTimeDelta);
	HRESULT Tick_FsmIce(_float fTimeDelta);
	HRESULT Tick_Magma(_float fTimeDelta);
	HRESULT SetUp_Component(void* pArg);

private:
	FSM		m_eCurState = FSM_NORMAL;
	FSM		m_ePreState = FSM_END;

	_bool	m_bEnvArr[CEnvironmentMgr::TYPE_END];
	class CBossSkeletonStageCubeObserver* m_pObserver = nullptr;

	TIME	m_tTimeRain;

	TIME	m_tTimeIceRemain;

	TIME	m_tTimeMagma;
	_bool	m_bMagma;
	_bool	m_bTutorialMagma = false;

public:
	static CBossSkeletonStageCube* Create(_pDevice pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};
END
#endif // !__CLIENT_CUBE_OBJECT_H__
