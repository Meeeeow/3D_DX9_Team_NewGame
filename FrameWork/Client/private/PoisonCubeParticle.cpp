#include "stdafx.h"
#include "..\public\PoisonCubeParticle.h"


CPoisonCubeParticle::CPoisonCubeParticle(_pDevice pDevice)
	: CCubeObject(pDevice)
{
}

CPoisonCubeParticle::CPoisonCubeParticle(const CPoisonCubeParticle & rhs)
	: CCubeObject(rhs)
{
}

HRESULT CPoisonCubeParticle::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CPoisonCubeParticle NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CPoisonCubeParticle::NativeConstruct(void * pArg)
{
	DESC tDesc = *(DESC*)(pArg);
	if (FAILED(__super::NativeConstruct(&tDesc.tObjInfo)))
		MSG_FAIL("CPoisonCubeParticle NativeConstruct - __super");

	_vec3 vLook = Get_TransformState(CTransform::STATE_LOOK);
	D3DXVec3Normalize(&vLook, &vLook);

	m_vStartPos = Get_TransformState(CTransform::STATE_POS);
	vLook.y += 2.f;
	m_vProjectilePower = vLook * tDesc.fPower;
	Set_RotateSpeed(tDesc.fPower);

	m_fRandomTime = (rand() % 1000) / 1000.f + 0.5f;

	m_tTimeProjectile.fDelay = 2.f;

	return S_OK;
}

_int CPoisonCubeParticle::Tick(_float fTimeDelta)
{
	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CPoisonCubeParticle Tick - __super");
	
	if (m_bDead)
		return OBJ_DEAD;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	Tick_Move(fTimeDelta, pGameInstance);
	Tick_Rotate(fTimeDelta, pGameInstance);

	RELEASE_INSTANCE(CGameInstance);

	return 0;
}

_int CPoisonCubeParticle::LateTick(_float fTimeDelta)
{
	if (0 > (__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CPoisonCubeParticle LateTick - __super");

	return _int();
}

HRESULT CPoisonCubeParticle::Render()
{
	Set_RenderState();

	if (FAILED(__super::Render()))
		MSG_FAIL("CPoisonCubeParticle Render - __super");

	Release_RenderState();

	return S_OK;
}

void CPoisonCubeParticle::Tick_Move(_float fTimeDelta, CGameInstance * pGameInstance)
{
	if (pGameInstance == nullptr)
		MSG_VOID("CPoisonCubeParticle::Tick_Move - pGameInstance == nullptr");

	m_tTimeProjectile.Tick_Time(fTimeDelta * m_fRandomTime);
	Move_Projectile(m_vStartPos, m_vProjectilePower, m_tTimeProjectile.fTime, m_fProjectileDegree);

	//목표 지점 도달
	if (m_tTimeProjectile.Check_Time())
	{
		m_tTimeProjectile.fTime = 0.f;
		Set_Dead();
	}
}

void CPoisonCubeParticle::Tick_Rotate(_float fTimeDelta, CGameInstance * pGameInstance)
{
	if (FAILED(Rotation_Axis(_vec3(1.f, 1.f, 0.f), 10.f)))
		MSG_VOID("CPoisonCubeParticle::Tick_Rotate - Rotation_Axis");
}

void CPoisonCubeParticle::Set_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_VOID("CPoisonCubeParticle::Set_RenderState - m_pGraphicDevice == nullptr");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHATESTENABLE, TRUE)))
		MSG_VOID("CPoisonCubeParticle::Set_RenderState - D3DRS_ALPHATESTENABLE, TRUE");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHAREF, 0)))
		MSG_VOID("CPoisonCubeParticle::Set_RenderState - D3DRENDERSTATETYPE::D3DRS_ALPHAREF, 0");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHAFUNC, D3DCMP_GREATER)))
		MSG_VOID("CPoisonCubeParticle::Set_RenderState - D3DRS_ALPHAFUNC, D3DCMP_GREATER");
}

void CPoisonCubeParticle::Release_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_VOID("CPoisonCubeParticle::Release_RenderState - m_pGraphicDevice == nullptr");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHATESTENABLE, FALSE)))
		MSG_VOID("CPoisonCubeParticle::Release_RenderState - D3DRS_ALPHATESTENABLE, FALSE");
}

CPoisonCubeParticle * CPoisonCubeParticle::Create(_pDevice pDevice)
{
	CPoisonCubeParticle* pInstance = new CPoisonCubeParticle(pDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("CPoisonCubeParticle Create Failed");
	}

	return pInstance;
}

CGameObject * CPoisonCubeParticle::Clone(void * pArg)
{
	CPoisonCubeParticle* pInstance = new CPoisonCubeParticle(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("CPoisonCubeParticle Clone Failed");
	}

	return pInstance;
}

void CPoisonCubeParticle::Free()
{
	__super::Free();
}
