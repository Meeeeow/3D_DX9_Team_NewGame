#include "stdafx.h"
#include "..\public\Dagger.h"
#include "Player.h"
#include "PlayerTagMgr.h"
#include "Attack.h"
#include "DmgFont.h"
#include "Monster.h"

CDagger::CDagger(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CCollisionRect(pGraphicDevice)
{
}

CDagger::CDagger(const CDagger & rhs)
	: CCollisionRect(rhs)
{
}

HRESULT CDagger::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CDagger NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CDagger::NativeConstruct(void * pArg)
{
	DESC tDesc = *(DESC*)(pArg);

	if (FAILED(__super::NativeConstruct(&tDesc.tSuperDesc)))
		MSG_FAIL("CDagger NativeConstruct - __super");

	D3DXVec3Normalize(&m_vMoveDir, &(tDesc.vTargetPos - Get_TransformState(CTransform::STATE_POS)));

	if (FAILED(Set_UpOrtho(m_vMoveDir)))
		MSG_FAIL("CDagger NativeConstruct - Set_UpOrtho");


	return S_OK;
}

_int CDagger::Tick(_float fTimeDelta)
{
	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CDagger Tick - __super");

	if (m_bDead)
		return OBJ_DEAD;

	m_fDist += Go_Dir(fTimeDelta, m_vMoveDir);
	if (m_fDist >= m_fMaxDist)
		Set_Dead();

	return 0;
}

_int CDagger::LateTick(_float fTimeDelta)
{
	if (0 > (__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CDagger LateTick - __super");

	return _int();
}

HRESULT CDagger::Render()
{
	m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	if (FAILED(__super::Render()))
		MSG_FAIL("CDagger Render - __super");
	m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

void CDagger::Collision(CGameObject * pColObj)
{
	CMonster* pMonster = dynamic_cast<CMonster*>(pColObj);
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	DO;
	//충돌한게 몬스터 아니면 나감
	if (pMonster == nullptr)
		break;

	CAttack::DESC tAttackDesc;
	tAttackDesc.iDmg = 51;
	tAttackDesc.vPos = Get_TransformState(CTransform::STATE_POS);
	tAttackDesc.vScale = Get_TransformScale();

	if (pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pTagPrototypeAttack
		, pTagLayerPlayerAttack, &tAttackDesc))
		MSG_VOID("CDagger::Collision - pTagPrototypeAttack");
	
	Set_Dead();
	WHILE;

	RELEASE_INSTANCE(CGameInstance);
	return;
}


CDagger * CDagger::Create(_pDevice pDevice)
{
	CDagger* pInstance = new CDagger(pDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("CDagger Create Failed");
	}

	return pInstance;
}

void CDagger::Free()
{
	__super::Free();
}

CGameObject * CDagger::Clone(void * pArg)
{
	CGameObject* pInstance = new CDagger(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("CDagger Clone Failed");
	}

	return pInstance;
}
