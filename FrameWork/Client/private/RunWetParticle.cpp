#include "stdafx.h"
#include "..\public\RunWetParticle.h"


CRunWetParticle::CRunWetParticle(_pDevice pDevice)
	: CCubeObject(pDevice)
{
}

CRunWetParticle::CRunWetParticle(const CRunWetParticle & rhs)
	: CCubeObject(rhs)
{
}

HRESULT CRunWetParticle::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CRunWetParticle NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CRunWetParticle::NativeConstruct(void * pArg)
{
	DESC tDesc = *(DESC*)(pArg);
	if (FAILED(__super::NativeConstruct(&tDesc.tObjInfo)))
		MSG_FAIL("CRunWetParticle NativeConstruct - __super");

	_vec3 vLook = Get_TransformState(CTransform::STATE_LOOK);
	D3DXVec3Normalize(&vLook, &vLook);

	m_vStartPos = Get_TransformState(CTransform::STATE_POS);
	vLook.y += tDesc.fPowerY;
	_vec3 vTemp1, vTemp2;
	D3DXVec3Normalize(&vTemp1, &vLook);
	vTemp2 = _vec3(vLook.x, 0.f, vLook.z);
	D3DXVec3Normalize(&vTemp2, &vTemp2);
	m_fProjectileDegree = acosf(D3DXVec3Dot(&vTemp1, &vTemp2));

	m_vProjectilePower = vLook * tDesc.fPower;
	Set_RotateSpeed(tDesc.fPower);

	m_fRandomTime = (rand() % 1000) / 1000.f + 0.5f;

	m_tTimeProjectile.fDelay = 2.f;

	return S_OK;
}

_int CRunWetParticle::Tick(_float fTimeDelta)
{
	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CRunWetParticle Tick - __super");
	
	if (m_bDead)
		return OBJ_DEAD;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	Tick_Move(fTimeDelta, pGameInstance);

	RELEASE_INSTANCE(CGameInstance);

	return 0;
}

_int CRunWetParticle::LateTick(_float fTimeDelta)
{
	if (0 > (__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CRunWetParticle LateTick - __super");

	return _int();
}

HRESULT CRunWetParticle::Render()
{
	Set_RenderState();

	if (FAILED(__super::Render()))
		MSG_FAIL("CRunWetParticle Render - __super");

	Release_RenderState();

	return S_OK;
}

void CRunWetParticle::Tick_Move(_float fTimeDelta, CGameInstance * pGameInstance)
{
	if (pGameInstance == nullptr)
		MSG_VOID("CRunWetParticle::Tick_Move - pGameInstance == nullptr");

	m_tTimeProjectile.Tick_Time(fTimeDelta * m_fRandomTime);
	Move_Projectile(m_vStartPos, m_vProjectilePower, m_tTimeProjectile.fTime, m_fProjectileDegree);

	//목표 지점 도달
	if (m_tTimeProjectile.Check_Time())
	{
		m_tTimeProjectile.fTime = 0.f;
		Set_Dead();
	}
}

void CRunWetParticle::Set_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_VOID("CRunWetParticle::Set_RenderState - m_pGraphicDevice == nullptr");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHATESTENABLE, TRUE)))
		MSG_VOID("CRunWetParticle::Set_RenderState - D3DRS_ALPHATESTENABLE, TRUE");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHAREF, 0)))
		MSG_VOID("CRunWetParticle::Set_RenderState - D3DRENDERSTATETYPE::D3DRS_ALPHAREF, 0");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHAFUNC, D3DCMP_GREATER)))
		MSG_VOID("CRunWetParticle::Set_RenderState - D3DRS_ALPHAFUNC, D3DCMP_GREATER");
}

void CRunWetParticle::Release_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_VOID("CRunWetParticle::Release_RenderState - m_pGraphicDevice == nullptr");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHATESTENABLE, FALSE)))
		MSG_VOID("CRunWetParticle::Release_RenderState - D3DRS_ALPHATESTENABLE, FALSE");
}

CRunWetParticle * CRunWetParticle::Create(_pDevice pDevice)
{
	CRunWetParticle* pInstance = new CRunWetParticle(pDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("CRunWetParticle Create Failed");
	}

	return pInstance;
}

CGameObject * CRunWetParticle::Clone(void * pArg)
{
	CRunWetParticle* pInstance = new CRunWetParticle(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("CRunWetParticle Clone Failed");
	}

	return pInstance;
}

void CRunWetParticle::Free()
{
	__super::Free();
}
