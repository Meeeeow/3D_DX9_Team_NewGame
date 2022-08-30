#include "stdafx.h"
#include "..\public\PoisonCubeHit.h"


CPoisonCubeHit::CPoisonCubeHit(_pDevice pDevice)
	: CCubeObject(pDevice)
{
}

CPoisonCubeHit::CPoisonCubeHit(const CPoisonCubeHit & rhs)
	: CCubeObject(rhs)
{
}

HRESULT CPoisonCubeHit::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CPoisonCubeHit NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CPoisonCubeHit::NativeConstruct(void * pArg)
{
	DESC tDesc = *(DESC*)(pArg);

	if (FAILED(__super::NativeConstruct(&tDesc.tObjInfo)))
		MSG_FAIL("CPoisonCubeHit NativeConstruct - __super");

	m_tTimeRemain.fDelay = 1.5f;

	m_vOriginalPos = Get_TransformState(CTransform::STATE_POS);

	Set_Speed(1.f * Get_TransformScale().x * 5.f);

	return S_OK;
}

_int CPoisonCubeHit::Tick(_float fTimeDelta)
{
	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CPoisonCubeHit Tick - __super");
	
	if (m_bDead)
		return OBJ_DEAD;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	Tick_Move(fTimeDelta, pGameInstance);

	m_tTimeRemain.Tick_Time(fTimeDelta);
	if (m_tTimeRemain.Check_Time())
		Set_Dead();

	RELEASE_INSTANCE(CGameInstance);

	return 0;
}

_int CPoisonCubeHit::LateTick(_float fTimeDelta)
{
	if (0 > (__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CPoisonCubeHit LateTick - __super");

	return _int();
}

HRESULT CPoisonCubeHit::Render()
{
	Set_RenderState();

	if (FAILED(__super::Render()))
		MSG_FAIL("CPoisonCubeHit Render - __super");

	Release_RenderState();

	return S_OK;
}

void CPoisonCubeHit::Tick_Move(_float fTimeDelta, CGameInstance * pGameInstance)
{
	if (pGameInstance == nullptr)
		MSG_VOID("CPoisonCubeHit::Tick_Move - pGameInstance == nullptr");

	Go_Up(fTimeDelta);

	_vec3 vPos = Get_TransformState(CTransform::STATE_POS);
	vPos.x = m_vOriginalPos.x + cosf(D3DXToRadian(vPos.y * 360.f / Get_Speed()));
	Set_TransformState(CTransform::STATE_POS, vPos);
}

void CPoisonCubeHit::Set_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_VOID("CPoisonCubeHit::Set_RenderState - m_pGraphicDevice == nullptr");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHATESTENABLE, TRUE)))
		MSG_VOID("CPoisonCubeHit::Set_RenderState - D3DRS_ALPHATESTENABLE, TRUE");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHAREF, 0)))
		MSG_VOID("CPoisonCubeHit::Set_RenderState - D3DRENDERSTATETYPE::D3DRS_ALPHAREF, 0");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHAFUNC, D3DCMP_GREATER)))
		MSG_VOID("CPoisonCubeHit::Set_RenderState - D3DRS_ALPHAFUNC, D3DCMP_GREATER");
}

void CPoisonCubeHit::Release_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_VOID("CPoisonCubeHit::Release_RenderState - m_pGraphicDevice == nullptr");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHATESTENABLE, FALSE)))
		MSG_VOID("CPoisonCubeHit::Release_RenderState - D3DRS_ALPHATESTENABLE, FALSE");
}

CPoisonCubeHit * CPoisonCubeHit::Create(_pDevice pDevice)
{
	CPoisonCubeHit* pInstance = new CPoisonCubeHit(pDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("CPoisonCubeHit Create Failed");
	}

	return pInstance;
}

CGameObject * CPoisonCubeHit::Clone(void * pArg)
{
	CPoisonCubeHit* pInstance = new CPoisonCubeHit(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("CPoisonCubeHit Clone Failed");
	}

	return pInstance;
}

void CPoisonCubeHit::Free()
{
	__super::Free();
}
