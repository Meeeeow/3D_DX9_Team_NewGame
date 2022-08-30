#include "stdafx.h"
#include "..\public\Confetti.h"



CConfetti::CConfetti(_pDevice pGraphicDevice)
	: CCubeObject(pGraphicDevice)
{
}

CConfetti::CConfetti(const CConfetti & rhs)
	: CCubeObject(rhs)
{
}

HRESULT CConfetti::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CConfetti NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CConfetti::NativeConstruct(void * pArg)
{
	DESC tDesc = *(DESC*)(pArg);
	if (FAILED(__super::NativeConstruct(&tDesc.tObjInfo)))
		MSG_FAIL("CConfetti NativeConstruct - __super");

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

_int CConfetti::Tick(_float fTimeDelta)
{
	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CConfetti Tick - __super");

	if (m_bDead)
		return OBJ_DEAD;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	Tick_Move(fTimeDelta, pGameInstance);
	Tick_Rotate(fTimeDelta, pGameInstance);

	RELEASE_INSTANCE(CGameInstance);

	return 0;
}

_int CConfetti::LateTick(_float fTimeDelta)
{
	if (0 > (__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CConfetti LateTick - __super");

	return _int();
}

HRESULT CConfetti::Render()
{
	Set_RenderState();

	//Set_FrameValue(rand() % 2);
	if (FAILED(__super::Render()))
		MSG_FAIL("CConfetti Render - __super");

	Release_RenderState();

	return S_OK;
}

void CConfetti::Tick_Move(_float fTimeDelta, CGameInstance * pGameInstance)
{
	if (pGameInstance == nullptr)
		MSG_VOID("CConfetti::Tick_Move - pGameInstance == nullptr");

	m_tTimeProjectile.Tick_Time(fTimeDelta * m_fRandomTime);
	Move_Projectile(m_vStartPos, m_vProjectilePower, m_tTimeProjectile.fTime, m_fProjectileDegree);

	//목표 지점 도달
	if (m_tTimeProjectile.Check_Time())
	{
		m_tTimeProjectile.fTime = 0.f;
		Set_Dead();
	}
}

void CConfetti::Tick_Rotate(_float fTimeDelta, CGameInstance * pGameInstance)
{
	if (FAILED(Rotation_Axis(_vec3(1.f, 1.f, 0.f), 10.f)))
		MSG_VOID("CConfetti::Tick_Rotate - Rotation_Axis");
}

void CConfetti::Set_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_VOID("CConfetti::Set_RenderState - m_pGraphicDevice == nullptr");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHATESTENABLE, TRUE)))
		MSG_VOID("CConfetti::Set_RenderState - D3DRS_ALPHATESTENABLE, TRUE");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHAREF, 0)))
		MSG_VOID("CConfetti::Set_RenderState - D3DRENDERSTATETYPE::D3DRS_ALPHAREF, 0");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHAFUNC, D3DCMP_GREATER)))
		MSG_VOID("CConfetti::Set_RenderState - D3DRS_ALPHAFUNC, D3DCMP_GREATER");
}

void CConfetti::Release_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_VOID("CConfetti::Release_RenderState - m_pGraphicDevice == nullptr");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHATESTENABLE, FALSE)))
		MSG_VOID("CConfetti::Release_RenderState - D3DRS_ALPHATESTENABLE, FALSE");
}

CConfetti * CConfetti::Create(_pDevice pGraphicDevice)
{
	CConfetti* pInstance = new CConfetti(pGraphicDevice);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CConfetti Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CConfetti::Clone(void * pArg)
{
	CConfetti* pInstance = new CConfetti(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CConfetti Clone Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CConfetti::Free()
{
	__super::Free();
}
