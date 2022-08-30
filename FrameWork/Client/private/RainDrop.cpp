#include "stdafx.h"
#include "..\public\RainDrop.h"


CRainDrop::CRainDrop(_pDevice pDevice)
	: CCubeObject(pDevice)
{
}

CRainDrop::CRainDrop(const CRainDrop & rhs)
	: CCubeObject(rhs)
{
}

HRESULT CRainDrop::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CRainDrop NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CRainDrop::NativeConstruct(void * pArg)
{
	DESC tDesc = *(DESC*)(pArg);
	m_fDegree = tDesc.fDegree;

	if (FAILED(__super::NativeConstruct(tDesc.pObjInfo)))
		MSG_FAIL("CRainDrop NativeConstruct - __super");

	Set_TransformScale(0.3f, 0.1f, 0.1f);
	SetUp_Rotation(_vec3(0.f, 0.f, 1.f), m_fDegree);
	Set_Speed(20.f);

	return S_OK;
}

_int CRainDrop::Tick(_float fTimeDelta)
{
	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CRainDrop Tick - __super");

	if (m_bDead)
		return OBJ_DEAD;

	Go_Dir(fTimeDelta, _vec3(cosf(D3DXToRadian(m_fDegree)), sinf(D3DXToRadian(m_fDegree)), 0.f));
	
	return 0;
}

_int CRainDrop::LateTick(_float fTimeDelta)
{
	if (0 > (__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CRainDrop LateTick - __super");

	_vec3 vPos = Get_TransformState(CTransform::STATE_POS);
	if (vPos.y <= -2.f)
		Set_Dead();

	return _int();
}

HRESULT CRainDrop::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("CRainDrop Render - __super");

	return S_OK;
}

CRainDrop * CRainDrop::Create(_pDevice pDevice)
{
	CRainDrop* pInstance = new CRainDrop(pDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("CRainDrop Create Failed");
	}

	return pInstance;
}

CGameObject * CRainDrop::Clone(void * pArg)
{
	CRainDrop* pInstance = new CRainDrop(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("CRainDrop Clone Failed");
	}

	return pInstance;
}

void CRainDrop::Free()
{
	__super::Free();
}

_vec3 CRainDrop::Get_MinPos()
{
	return _vec3();
}

_vec3 CRainDrop::Get_MaxPos()
{
	return _vec3();
}

_vec3 CRainDrop::Get_WorldPos()
{
	return _vec3();
}

_vec3 CRainDrop::Get_Length()
{
	return _vec3();
}


