#include "stdafx.h"
#include "..\public\WaterCannon.h"
#include "Player.h"
#include "BossSkeletonStageCubeAttack.h"

CWaterCannon::CWaterCannon(_pDevice pDevice)
	: CGameObject(pDevice)
{
}

CWaterCannon::CWaterCannon(const CWaterCannon & rhs)
	: CGameObject(rhs)
{
}

HRESULT CWaterCannon::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CWaterCannon NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CWaterCannon::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CWaterCannon NativeConstruct - __super");

	DESC tDesc = *(DESC*)(pArg);
	m_vPos = tDesc.vPos;
	m_tTimeShoot.fDelay = 2.f;

	return S_OK;
}

_int CWaterCannon::Tick(_float fTimeDelta)
{
	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CWaterCannon Tick - __super");

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_tTimeShoot.Tick_Time(fTimeDelta);
	if (m_tTimeShoot.Check_Time())
	{
		m_tTimeShoot.fTime = 0.f;
		CBossSkeletonStageCubeAttack::DESC tAttackDesc;
		tAttackDesc.eType = CBossSkeletonStageCubeAttack::TYPE_WET;
		tAttackDesc.tObjInfo.dwColliderColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		tAttackDesc.tObjInfo.vColliderCenter = _vec3(0.f, 0.f, 0.f);
		tAttackDesc.tObjInfo.vColliderSize = _vec3(1.f, 1.f, 1.f);
		lstrcpy(tAttackDesc.tObjInfo.szTextureName, pTagPrototypeTextureBossSkeletonStageCubeAttack);
		D3DXMatrixIdentity(&tAttackDesc.tObjInfo.matWorld);
		tAttackDesc.tObjInfo.SetUp_State(OBJ_INFO::STATE_POS, m_vPos);
		tAttackDesc.tObjInfo.SetUp_Scale(8.f, 8.f, 8.f);
		tAttackDesc.vTargetPos = m_vPos + _vec3(-150.f, 0.f, 0.f);
		tAttackDesc.bNotBoss = true;

		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pTagPrototypeBossSkeletonStageCubeAttack, pTagLayerBossSkeletonCube
			, &tAttackDesc)))
			MSG_MINUSONE("CWaterCannon::Tick - pTagPrototypeBossSkeletonStageCubeAttack");
	}

	RELEASE_INSTANCE(CGameInstance);

	return 0;
}

_int CWaterCannon::LateTick(_float fTimeDelta)
{
	if (0 > (__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CWaterCannon LateTick - __super");

	return _int();
}

HRESULT CWaterCannon::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("CWaterCannon Render - __super");

	return S_OK;
}

CWaterCannon * CWaterCannon::Create(_pDevice pDevice)
{
	CWaterCannon* pInstance = new CWaterCannon(pDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("CWaterCannon Create Failed");
	}

	return pInstance;
}

CGameObject * CWaterCannon::Clone(void * pArg)
{
	CWaterCannon* pInstance = new CWaterCannon(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("CWaterCannon Clone Failed");
	}

	return pInstance;
}

void CWaterCannon::Free()
{
	__super::Free();
}

_vec3 CWaterCannon::Get_MinPos()
{
	return _vec3();
}

_vec3 CWaterCannon::Get_MaxPos()
{
	return _vec3();
}

_vec3 CWaterCannon::Get_WorldPos()
{
	return _vec3();
}

HRESULT CWaterCannon::Tick_Collider(_float fTimeDelta)
{
	return S_OK;

}

_vec3 CWaterCannon::Get_Length()
{
	return _vec3();
}
