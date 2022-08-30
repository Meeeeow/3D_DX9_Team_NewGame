#include "stdafx.h"
#include "..\public\MagicMissileParticle.h"


CMagicMissileParticle::CMagicMissileParticle(_pDevice pDevice)
	: CCubeObject(pDevice)
{
}

CMagicMissileParticle::CMagicMissileParticle(const CMagicMissileParticle & rhs)
	: CCubeObject(rhs)
{
}

HRESULT CMagicMissileParticle::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CMagicMissileParticle NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CMagicMissileParticle::NativeConstruct(void * pArg)
{
	DESC tDesc = *(DESC*)(pArg);
	if (FAILED(__super::NativeConstruct(&tDesc.tObjInfo)))
		MSG_FAIL("CMagicMissileParticle NativeConstruct - __super");

	Set_Speed(tDesc.fSpeed);
	m_tTime.fDelay = 0.5f;
	m_eType = tDesc.eType;
	return S_OK;
}

_int CMagicMissileParticle::Tick(_float fTimeDelta)
{
	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CMagicMissileParticle Tick - __super");
	
	if (m_bDead)
		return OBJ_DEAD;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	Tick_Move(fTimeDelta, pGameInstance);

	m_tTime.Tick_Time(fTimeDelta);
	if (m_tTime.Check_Time())
		Set_Dead();

	RELEASE_INSTANCE(CGameInstance);

	return 0;
}

_int CMagicMissileParticle::LateTick(_float fTimeDelta)
{
	if (0 > (__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CMagicMissileParticle LateTick - __super");

	return _int();
}

HRESULT CMagicMissileParticle::Render()
{
	Set_RenderState();

	if (FAILED(__super::Render()))
		MSG_FAIL("CMagicMissileParticle Render - __super");

	Release_RenderState();

	return S_OK;
}

void CMagicMissileParticle::Tick_Move(_float fTimeDelta, CGameInstance * pGameInstance)
{
	if (pGameInstance == nullptr)
		MSG_VOID("CMagicMissileParticle::Tick_Move - pGameInstance == nullptr");

	if (m_eType == TYPE_END)
		MSG_VOID("CMagicMissileParticle::Tick_Move - m_eType == TYPE_END");

	switch (m_eType)
	{
	case Client::CMagicMissileParticle::TYPE_LEFT:
		Go_Left(fTimeDelta);
		break;
	case Client::CMagicMissileParticle::TYPE_RIGHT:
		Go_Left(-fTimeDelta);
		break;
	case Client::CMagicMissileParticle::TYPE_UP:
		Go_Up(fTimeDelta);
		break;
	case Client::CMagicMissileParticle::TYPE_DOWN:
		Go_Up(-fTimeDelta);
		break;
	}
}

void CMagicMissileParticle::Set_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_VOID("CMagicMissileParticle::Set_RenderState - m_pGraphicDevice == nullptr");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHATESTENABLE, TRUE)))
		MSG_VOID("CMagicMissileParticle::Set_RenderState - D3DRS_ALPHATESTENABLE, TRUE");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHAREF, 0)))
		MSG_VOID("CMagicMissileParticle::Set_RenderState - D3DRENDERSTATETYPE::D3DRS_ALPHAREF, 0");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHAFUNC, D3DCMP_GREATER)))
		MSG_VOID("CMagicMissileParticle::Set_RenderState - D3DRS_ALPHAFUNC, D3DCMP_GREATER");
}

void CMagicMissileParticle::Release_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_VOID("CMagicMissileParticle::Release_RenderState - m_pGraphicDevice == nullptr");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHATESTENABLE, FALSE)))
		MSG_VOID("CMagicMissileParticle::Release_RenderState - D3DRS_ALPHATESTENABLE, FALSE");
}

CMagicMissileParticle * CMagicMissileParticle::Create(_pDevice pDevice)
{
	CMagicMissileParticle* pInstance = new CMagicMissileParticle(pDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("CMagicMissileParticle Create Failed");
	}

	return pInstance;
}

CGameObject * CMagicMissileParticle::Clone(void * pArg)
{
	CMagicMissileParticle* pInstance = new CMagicMissileParticle(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("CMagicMissileParticle Clone Failed");
	}

	return pInstance;
}

void CMagicMissileParticle::Free()
{
	__super::Free();
}
