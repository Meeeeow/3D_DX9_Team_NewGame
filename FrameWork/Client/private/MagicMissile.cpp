#include "stdafx.h"
#include "..\public\MagicMissile.h"
#include "Player.h"
#include "Attack.h"
#include "Monster.h"
#include "BossSkeletonStageCube.h"
#include "MagicMissile.h"
#include "MagicMissileParticle.h"

CMagicMissile::CMagicMissile(_pDevice pDevice)
	: CCollisionCubeObject(pDevice)
{
}

CMagicMissile::CMagicMissile(const CMagicMissile & rhs)
	: CCollisionCubeObject(rhs)
{
}

HRESULT CMagicMissile::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CMagicMissile NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CMagicMissile::NativeConstruct(void * pArg)
{
	//받아온 정보 넣어주고
	if (pArg == nullptr)
		MSG_FAIL("CMagicMissile::NativeConstruct - pArg == nullptr");
	DESC tMagicMissileDesc;
	memcpy(&tMagicMissileDesc, pArg, sizeof(DESC));

	//생성 정보 넣어주고
	OBJ_INFO tObjInfo;
	tObjInfo.dwColliderColor = 0xffffffff;
	tObjInfo.vColliderCenter = _vec3(0.f, 0.f, 0.f);
	tObjInfo.vColliderSize = _vec3(1.f, 1.f, 1.f);
	tObjInfo.eVIBufferType = OBJ_INFO::VI_BUFFER_TYPE_CUBE;
	tObjInfo.SetUp_Scale(1.f, 1.f, 1.f);
	tObjInfo.SetUp_State(OBJ_INFO::STATE_POS, tMagicMissileDesc.vPos);
	lstrcpy(tObjInfo.szObjName, L"MagicMissile");
	lstrcpy(tObjInfo.szTextureName, pTagPrototypeTextureMagicMissile);
	tObjInfo.iTextureIndex = tMagicMissileDesc.iTextureNum;

	//여기서 셋업 컴포넌트 알아서 함
	if (FAILED(__super::NativeConstruct(&tObjInfo)))
		MSG_FAIL("CMagicMissile NativeConstruct - __super");

	//빙결 마법
	if (tObjInfo.iTextureIndex == 1)
		PLAY_SOUND(L"MageSkillMagicMissile.ogg", CSoundMgr::MAGE_MAGIC_MISSILE);

	//방향 구하기
	_vec3 vLook = tMagicMissileDesc.vTargetPos - tMagicMissileDesc.vPos, vRight, vUp = _vec3(0.f, 1.f, 0.f);
	D3DXVec3Normalize(&vLook, &vLook);

	//방향 구했으면 직교를 위해 라업룩 전부 세팅해줘야함
	//룩 넣어줌
	if (FAILED(Set_TransformState(CTransform::STATE_LOOK, vLook)))
		MSG_FAIL("CMagicMissile::NativeConstruct - Set_TransformState");
	

	//라이트 넣어줌
	D3DXVec3Cross(&vRight, &vUp, &vLook);
	if (FAILED(Set_TransformState(CTransform::STATE_RIGHT, vRight)))
		MSG_FAIL("CMagicMissile::NativeConstruct - Set_TransformState");

	//업 넣어줌
	D3DXVec3Cross(&vUp, &vLook, &vRight);
	if (FAILED(Set_TransformState(CTransform::STATE_UP, vUp)))
		MSG_FAIL("CMagicMissile::NativeConstruct - Set_TransformState");

	//크기가 너무 커서 스케일 조정
	if (FAILED(Set_TransformScale(0.2f, 0.2f, 0.2f)))
		MSG_FAIL("CMagicMissile::NativeConstruct - Set_TransformScale");

	//회전속도 이동속도 세팅
	if (FAILED(Set_Speed(tMagicMissileDesc.tTransformDesc.fSpeed)))
		MSG_FAIL("CMagicMissile::NativeConstruct - Set_Speed");
	
	if (FAILED(Set_RotateSpeed(tMagicMissileDesc.tTransformDesc.fDegree)))
		MSG_FAIL("CMagicMissile::NativeConstruct - Set_RotateSpeed");
	
	return S_OK;
}

_int CMagicMissile::Tick(_float fTimeDelta)
{
	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CMagicMissile Tick - __super");

	if (m_bDead)
	{
		//빙결일 경우
		if (Get_FrameIndex() == 1)
			PLAY_SOUND(L"MaggicMissileIceHit.ogg", CSoundMgr::MAGE_MAGIC_MISSILE);

		return OBJ_DEAD;
	}

	Move(fTimeDelta);
	RotateZ(fTimeDelta);
	Tick_Particle(fTimeDelta);

	return 0;
}

_int CMagicMissile::LateTick(_float fTimeDelta)
{
	if (0 > (__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CMagicMissile LateTick - __super");

	return _int();
}

HRESULT CMagicMissile::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("CMagicMissile Render - __super");

	return S_OK;
}

HRESULT CMagicMissile::Move(_float fTimeDelta)
{
	_float fDist = 0.f;
	if (0.f > (fDist = Go_Straight(fTimeDelta)))
		MSG_FAIL("CMagicMissile::Move - Go_Straight");

	m_fRange += fDist;
	if (m_fMaxRange <= m_fRange)
		Set_Dead();

	return S_OK;
}

HRESULT CMagicMissile::RotateZ(_float fTimeDelta)
{
	if (FAILED(Rotation_Axis(Get_TransformState(CTransform::STATE_LOOK), fTimeDelta)))
		MSG_FAIL("CMagicMissile::RotateZ - Rotation_Axis");

	return S_OK;
}

void CMagicMissile::Tick_Particle(_float fTimeDelta)
{
	if (Get_FrameIndex() != 1)
		return;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	//Set_Speed(1.f);
	
	m_tTimeParticle.Tick_Time(fTimeDelta);
	if (m_tTimeParticle.Check_Time())
	{
		m_tTimeParticle.fTime = 0.f;
		CMagicMissileParticle::DESC tDesc;
		tDesc.fSpeed = 2.f;
		tDesc.tObjInfo.matWorld = *Get_TransformMatPtr();
		tDesc.tObjInfo.SetUp_Scale(0.05f, 0.05f, 0.05f);
		tDesc.tObjInfo.iTextureIndex = 1;
		lstrcpy(tDesc.tObjInfo.szTextureName, pTagPrototypeTextureMagicMissile);

		_vec3 vPosArr[4];
		CMagicMissileParticle::TYPE eTypeArr[4] = {
			CMagicMissileParticle::TYPE_LEFT,
			CMagicMissileParticle::TYPE_RIGHT,
			CMagicMissileParticle::TYPE_UP,
			CMagicMissileParticle::TYPE_DOWN
		};
		Get_BottomBufferPos(vPosArr);
		

		for (size_t i = 0; i < 4; ++i)
		{
			D3DXVec3TransformCoord(&vPosArr[i], &vPosArr[i], Get_TransformMatPtr());
			tDesc.tObjInfo.SetUp_State(OBJ_INFO::STATE_POS, vPosArr[i]);
			tDesc.eType = eTypeArr[i];
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pTagPrototypeMagicMissileParticle
				, pTagLayerNonCollision, &tDesc)))
				MSG_VOID("CMagicMissile::Tick_Particle - pTagPrototypeMagicMissileParticle");
		}
	}

	RELEASE_INSTANCE(CGameInstance);
}

CMagicMissile * CMagicMissile::Create(_pDevice pDevice)
{
	CMagicMissile* pInstance = new CMagicMissile(pDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("CMagicMissile Create Failed");
	}

	return pInstance;
}

CGameObject * CMagicMissile::Clone(void * pArg)
{
	CMagicMissile* pInstance = new CMagicMissile(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("CMagicMissile Clone Failed");
	}

	return pInstance;
}

void CMagicMissile::Free()
{
	__super::Free();
}

void CMagicMissile::Collision(CGameObject * pColObj)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pColObj);

	if (pPlayer != nullptr)
	{
		CAttack::DESC tDesc;
		tDesc.iDmg = 10;
		tDesc.vPos = Get_TransformState(CTransform::STATE_POS);
		tDesc.vScale = Get_TransformScale();
		tDesc.bSingleAttack = true;
		pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pTagPrototypeAttack, pTagLayerMonsterAttack, &tDesc);

		Set_Dead();
	}

	CMonster* pMonster = dynamic_cast<CMonster*>(pColObj);

	//법사 빙결 마법
	if (pMonster != nullptr)
	{
		CAttack::DESC tDesc;
		tDesc.iDmg = 5;
		tDesc.vPos = Get_TransformState(CTransform::STATE_POS);
		tDesc.vScale = Get_TransformScale();
		tDesc.bFreeze = true;
		pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pTagPrototypeAttack, pTagLayerPlayerAttack, &tDesc);

		//pMonster->Sub_Hp(5);

		Set_Dead();
	}

	CBossSkeletonStageCube* pBossSkeletonStageCube = dynamic_cast<CBossSkeletonStageCube*>(pColObj);

	//플레이어 빙결 마법
	if (pBossSkeletonStageCube != nullptr)
	{
		CAttack::DESC tDesc;
		tDesc.iDmg = 5;
		tDesc.vPos = Get_TransformState(CTransform::STATE_POS);
		tDesc.vScale = Get_TransformScale();
		tDesc.bFreeze = true;
		pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pTagPrototypeAttack, pTagLayerPlayerAttack, &tDesc);
		Set_Dead();
	}

	RELEASE_INSTANCE(CGameInstance);
}

