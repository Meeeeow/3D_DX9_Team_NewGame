#include "stdafx.h"
#include "..\public\DestroyableCube.h"
#include "Attack.h"

CDestroyableCube::CDestroyableCube(_pDevice pDevice)
	: CCollisionCubeObject(pDevice)
{
}

CDestroyableCube::CDestroyableCube(const CDestroyableCube & rhs)
	: CCollisionCubeObject(rhs)
{
}

HRESULT CDestroyableCube::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CDestroyableCube NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CDestroyableCube::NativeConstruct(void * pArg)
{
	//여기서 셋업 컴포넌트 알아서 함
	DESC tDesc = *(DESC*)(pArg);
	if (FAILED(__super::NativeConstruct(tDesc.pObjInfo)))
		MSG_FAIL("CDestroyableCube NativeConstruct - __super");
	
	return S_OK;
}

_int CDestroyableCube::Tick(_float fTimeDelta)
{
	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CDestroyableCube Tick - __super");

	if (m_bDead)
		return OBJ_DEAD;

	return 0;
}

_int CDestroyableCube::LateTick(_float fTimeDelta)
{
	if (0 > (__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CDestroyableCube LateTick - __super");

	return _int();
}

HRESULT CDestroyableCube::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("CDestroyableCube Render - __super");

	return S_OK;
}

void CDestroyableCube::Collision(CGameObject * pColObj)
{
	CAttack* pAttack = dynamic_cast<CAttack*>(pColObj);

	//충돌한게 공격인 경우
	if (pAttack != nullptr)
	{
		//충돌한 공격이 단일 타겟 공격인데 이미 누굴 공격한 경우
		if (pAttack->Get_SingleAttack() && pAttack->Get_Dead())
		{

		}
		//충돌한 공격 속성에 오브젝트 파괴가 있는 경우
		else if (pAttack->Get_Destroy())
			Set_Dead();

		pAttack->Set_Dead();
	}

}

CDestroyableCube * CDestroyableCube::Create(_pDevice pDevice)
{
	CDestroyableCube* pInstance = new CDestroyableCube(pDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("CDestroyableCube Create Failed");
	}

	return pInstance;
}

CGameObject * CDestroyableCube::Clone(void * pArg)
{
	CDestroyableCube* pInstance = new CDestroyableCube(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("CDestroyableCube Clone Failed");
	}

	return pInstance;
}

void CDestroyableCube::Free()
{
	__super::Free();
}
