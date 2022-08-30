#include "..\public\Camera.h"
#include "GameInstance.h"

void CCamera::Set_MatProject(_float fAspect, _float fFovY, _float fNear, _float fFar)
{
	m_tInitCam.fAspect = fAspect;
	m_tInitCam.fFar = fFar;
	m_tInitCam.fNear = fNear;
	m_tInitCam.fFovY = fFovY;
}

HRESULT CCamera::Add_Component(_uint iLevelIndex, const _tchar * pPrototypeTag, const _tchar * pComponentTag, void * pArg)
{
	if (FAILED(__super::Add_Component(iLevelIndex, pPrototypeTag, pComponentTag, (CComponent**)&m_pTransform, pArg)))
		MSG_FAIL("CCamera Add_Component - __super");

	return S_OK;
}

HRESULT CCamera::Synchronize_CameraState()
{
	_vec3 vLook = m_tInitCam.vAt - m_tInitCam.vEye;
	D3DXVec3Normalize(&vLook, &vLook);
	m_pTransform->SetUp_State(CTransform::STATE::STATE_LOOK, vLook);

	_vec3 vRight = *D3DXVec3Cross(&vRight, &_vec3(0.f, 1.f, 0.f), &vLook);
	m_pTransform->SetUp_State(CTransform::STATE::STATE_RIGHT, vRight);
	
	_vec3 vUp = *D3DXVec3Cross(&vUp, &vLook, &vRight);
	m_pTransform->SetUp_State(CTransform::STATE::STATE_UP, vUp);

	m_pTransform->SetUp_State(CTransform::STATE::STATE_POS, m_tInitCam.vEye);

	return S_OK;
}


void CCamera::Free()
{
	Safe_Release(m_pTransform);
	__super::Free();
}

CCamera::CCamera(_pDevice pDevice)
	: CGameObject(pDevice)
{
}

CCamera::CCamera(const CCamera & rhs)
	: CGameObject(rhs)
{
}

HRESULT CCamera::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CCamera NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CCamera::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CCamera NativeConstruct - __super");

	if (pArg != nullptr)
		memcpy(&m_tInitCam, pArg, sizeof(INIT_CAMERA));

	return S_OK;
}

_int CCamera::Tick(_float fTimeDelta)
{
	if (nullptr == m_pTransform)
		MSG_MINUSONE("CCamera Tick - m_pTransform is null");

	_mat matWorld = *m_pTransform->Get_WorldMatrixPtr();
	D3DXMatrixInverse(&matWorld, nullptr, &matWorld);
	m_pGraphicDevice->SetTransform(D3DTS_VIEW, &matWorld);

	_mat matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(m_tInitCam.fFovY), m_tInitCam.fAspect, m_tInitCam.fNear, m_tInitCam.fFar);
	m_pGraphicDevice->SetTransform(D3DTS_PROJECTION, &matProj);

	return __super::Tick(fTimeDelta);
}

_int CCamera::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	return _int();
}
