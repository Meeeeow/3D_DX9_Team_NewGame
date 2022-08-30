#include "..\public\Effect.h"
#include "GameInstance.h"
#include "Transform.h"

CEffect::CEffect(_pDevice pDevice)
	:CGameObject(pDevice)
{
}

CEffect::CEffect(const CEffect & rhs)
	:CGameObject(rhs)
{
}

HRESULT CEffect::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CEffect NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CEffect::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CEffect NativeConstruct - __super");

	return S_OK;
}

_int CEffect::Tick(_float fTimeDelta, _bool bPlayOnce)
{
	if (0 > __super::Tick(fTimeDelta))
		MSG_MINUSONE("CEffect::Tick - __super");

	if (bPlayOnce == true)
		Tick_FrameOnce(fTimeDelta);
	else if (bPlayOnce == false)
		Tick_Frame(fTimeDelta);

	return 0;
}

_int CEffect::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
		MSG_MINUSONE("CEffect::LateTick - __super");

	return 0;
}

HRESULT CEffect::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("CEffect::Render - __super");

	return S_OK;
}

HRESULT CEffect::Compute_Distance(CTransform * pTransform)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_mat matCamWorld = pGameInstance->Get_CamWorld();
	_vec3 vCamPos = *(_vec3*)(&matCamWorld.m[3][0]);

	_vec3 vEffectPos = pTransform->Get_State(CTransform::STATE_POS);

	m_fCamDistance = D3DXVec3Length(&(vCamPos - vEffectPos));

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CEffect::BillBoarding(class CTransform* pTransform, BILL_BOARDING eBillBoardType)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_mat matCamWorld = pGameInstance->Get_CamWorld();
	_vec3 vEffectScale = pTransform->Get_Scale();

	pTransform->SetUp_State(CTransform::STATE_RIGHT, (*(_vec3*)(&matCamWorld.m[0][0])) * vEffectScale.x);
	if (eBillBoardType == BILL_BOARDING::BB_ALL)
		pTransform->SetUp_State(CTransform::STATE_UP, (*(_vec3*)(&matCamWorld.m[1][0])) * vEffectScale.y);
	pTransform->SetUp_State(CTransform::STATE_LOOK, (*(_vec3*)(&matCamWorld.m[2][0])) * vEffectScale.z);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CEffect::Set_FrameValue(_int iEnd, _int iStart /*= 0*/, _float fDelay /*= 0.1f*/, _float fTime /*= 0.f*/)
{
	m_tFrame.Set_Value(iEnd, iStart, fDelay, fTime);
}

void CEffect::Tick_Frame(_float fTimeDelta)
{
	m_tFrame.Tick_Time(fTimeDelta);

	if (m_tFrame.Check_Time())
	{
		m_tFrame.iStart++;
		m_tFrame.tTime.fTime = 0.f;

		if (m_tFrame.iStart >= m_tFrame.iEnd)
		{
			m_tFrame.iStart = 0;
		}
	}
}

void CEffect::Tick_FrameOnce(_float fTimeDelta)
{
	m_tFrame.Tick_Time(fTimeDelta);

	if (m_tFrame.Check_Time())
	{
		m_tFrame.iStart++;
		m_tFrame.tTime.fTime = 0.f;

		if (m_tFrame.iStart >= m_tFrame.iEnd)
		{
			m_tFrame.iStart = 0;
			Set_Dead();
		}
	}
}

void CEffect::Free()
{
	__super::Free();
}

_vec3 CEffect::Get_MinPos()
{
	return _vec3();
}

_vec3 CEffect::Get_MaxPos()
{
	return _vec3();
}

_vec3 CEffect::Get_WorldPos()
{
	return _vec3();
}

_vec3 CEffect::Get_Length()
{
	return _vec3();
}
