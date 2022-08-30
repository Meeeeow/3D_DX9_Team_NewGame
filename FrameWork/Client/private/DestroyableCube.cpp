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
	//���⼭ �¾� ������Ʈ �˾Ƽ� ��
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

	//�浹�Ѱ� ������ ���
	if (pAttack != nullptr)
	{
		//�浹�� ������ ���� Ÿ�� �����ε� �̹� ���� ������ ���
		if (pAttack->Get_SingleAttack() && pAttack->Get_Dead())
		{

		}
		//�浹�� ���� �Ӽ��� ������Ʈ �ı��� �ִ� ���
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
