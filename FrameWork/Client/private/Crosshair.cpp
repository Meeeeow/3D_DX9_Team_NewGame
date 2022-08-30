#include "stdafx.h"
#include "..\public\Crosshair.h"
#include "Player.h"
#include "Monster.h"
#include "PlayerTagMgr.h"
#include "Attack.h"
#include "DmgFont.h"
#include "CrosshairMgr.h"

CCrosshair::CCrosshair(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CGameObject(pGraphicDevice)
{
}

CCrosshair::CCrosshair(const CCrosshair & rhs)
	: CGameObject(rhs)
{
}

HRESULT CCrosshair::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CCrosshair NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CCrosshair::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CCrosshair NativeConstruct - __super");

	if (FAILED(SetUp_Component(pArg)))
		MSG_FAIL("CCrosshair NativeConstruct - SetUp_Component");

	D3DXMatrixOrthoLH(&m_matProject, (float)g_iWinCX, (float)g_iWinCY, 0.f, 1.f);
	_float fSizeX = 64.f;
	_float fSizeY = 64.f;
	_float fX = g_iWinCX * 0.5f;
	_float fY = g_iWinCY * 0.5f;

	m_pTransform->SetUp_State(CTransform::STATE::STATE_POS, _vec3(fX - ((float)g_iWinCX * 0.5f), -fY + ((float)g_iWinCY * 0.5f), 0.f));
	m_pTransform->SetUp_Scale(fSizeX, fSizeY, 1.f);

	CCrosshairMgr::Get_Instance()->Set_Crosshair(this);

	return S_OK;
}

_int CCrosshair::Tick(_float fTimeDelta)
{
	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CCrosshair Tick - __super");

	if (m_bDead)
		return OBJ_DEAD;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(Tick_CrosshairRay(pGameInstance)))
		MSG_MINUSONE("CCrosshair::Tick - Tick_CrosshairRay");


	RELEASE_INSTANCE(CGameInstance);
	return 0;
}

_int CCrosshair::LateTick(_float fTimeDelta)
{
	if (0 > (__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CCrosshair LateTick - __super");

	if (m_pRenderer == nullptr)
		MSG_MINUSONE("CCrosshair::LateTick - m_pRenderer == nullptr");

	if (m_pRenderer->Add_RenderGroup(CRenderer::GROUP_UI, this))
		MSG_MINUSONE("CCrosshair::LateTick - Add_RenderGroup");

	return _int();
}

HRESULT CCrosshair::Render()
{
	if (m_bRender == false)
		return S_OK;

	if (FAILED(__super::Render()))
		MSG_FAIL("CCrosshair Render - __super");

	if (m_pTransform == nullptr ||
		m_pTexture == nullptr ||
		m_pVIBuffer == nullptr)
		MSG_FAIL("CCrosshair::Render - Com is null");

	_mat matIdentity;
	D3DXMatrixIdentity(&matIdentity);
	m_pGraphicDevice->SetTransform(D3DTS_VIEW, &matIdentity);
	m_pGraphicDevice->SetTransform(D3DTS_PROJECTION, &m_matProject);

	if (FAILED(m_pTransform->Bind_OnDevice()))
		MSG_FAIL("CCrosshair::Render - m_pTransform Bind_OnDevice");

	if (FAILED(m_pTexture->Bind_OnDevice()))
		MSG_FAIL("CCrosshair::Render - m_pTexture[m_eCurState]->Bind_OnDevice");

	if (FAILED(Set_RenderState()))
		MSG_FAIL("CCrosshair::Render - Set_RenderState");

	if (FAILED(m_pVIBuffer->Render()))
		MSG_FAIL("CCrosshair::Render - m_pVIBuffer->Render");

	if (FAILED(Release_RenderState()))
		MSG_FAIL("CCrosshair::Render - Release_RenderState");

	return S_OK;
}

_bool CCrosshair::Picking(const _tchar * szTransformTag, const _tchar * szBufferTag, CGameObject * pTarget, _vec3 * pOut)
{
	CTransform* pTargetTransform = dynamic_cast<CTransform*>(pTarget->Get_Component(szTransformTag));
	if (pTargetTransform == nullptr)
		MSG_FALSE("CCrosshair::Picking - pTargetTransform is null");

	CVIBuffer* pTargetVIBuffer = dynamic_cast<CVIBuffer*>(pTarget->Get_Component(szBufferTag));
	if (pTargetVIBuffer == nullptr)
		MSG_FALSE("CCrosshair::Picking - pTargetVIBuffer is null");

	_mat matWorld, matInverse;
	_vec3 vLocalPivot, vLocalRay;
	matWorld = *pTargetTransform->Get_WorldMatrixPtr();
	D3DXMatrixInverse(&matInverse, nullptr, &matWorld);
	D3DXVec3TransformCoord(&vLocalPivot, &m_vCrosshairPivot, &matInverse);
	D3DXVec3TransformNormal(&vLocalRay, &m_vCrosshairRay, &matInverse);
	D3DXVec3Normalize(&vLocalRay, &vLocalRay);

	if (pTargetVIBuffer->Picking(vLocalRay, vLocalPivot, pOut))
	{
		D3DXVec3TransformCoord(pOut, pOut, &matWorld);
		return true;
	}

	return false;
}

_bool CCrosshair::Picking(const _tchar * szTransformTag, const _tchar * szBufferTag, CMonster * pTarget)
{
	CTransform* pTargetTransform = dynamic_cast<CTransform*>(pTarget->Get_Component(szTransformTag));
	if (pTargetTransform == nullptr)
		MSG_FALSE("CCrosshair::Picking - pTargetTransform is null");

	CVIBuffer* pTargetVIBuffer = dynamic_cast<CVIBuffer*>(pTarget->Get_Component(szBufferTag));
	if (pTargetVIBuffer == nullptr)
		MSG_FALSE("CCrosshair::Picking - pTargetVIBuffer is null");

	_mat matWorld, matInverse;
	_vec3 vLocalPivot, vLocalRay;
	CTransform* pTransform = nullptr;
	matWorld = *pTargetTransform->Get_WorldMatrixPtr();
	D3DXMatrixInverse(&matInverse, nullptr, &matWorld);
	D3DXVec3TransformCoord(&vLocalPivot, &m_vCrosshairPivot, &matInverse);
	D3DXVec3TransformNormal(&vLocalRay, &m_vCrosshairRay, &matInverse);
	D3DXVec3Normalize(&vLocalRay, &vLocalRay);
	if (pTarget->Picking(vLocalRay, vLocalPivot, pTransform))
		return true;

	return false;
}

HRESULT CCrosshair::SetUp_Component(void *pArg)
{
	OBJ_INFO tObjInfo = *(OBJ_INFO*)(pArg);

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeTransform, pTagComTransform, (CComponent**)&m_pTransform)))
		MSG_FAIL("CCrosshair::SetUp_Component - m_pTransform");
	m_pTransform->Set_Matrix(tObjInfo.matWorld);

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeRenderer, pTagComRenderer, (CComponent**)&m_pRenderer)))
		MSG_FAIL("CCrosshair::SetUp_Component - m_pRenderer");

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeVIBufferRect, pTagComVIBuffer, (CComponent**)&m_pVIBuffer)))
		MSG_FAIL("CCrosshair::SetUp_Component - m_pVIBuffer");

	wstring wstrTextureName;

	wstrTextureName = tObjInfo.szTextureName;
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, wstrTextureName.c_str(), pTagComTexture, (CComponent**)&m_pTexture)))
		MSG_FAIL("CCrosshair::SetUp_Component - wstrTextureName");

	return S_OK;
}

HRESULT CCrosshair::Set_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CCrosshair::Set_RenderState - m_pGraphicDevice == nullptr");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE)))
		MSG_FAIL("CCrosshair::Set_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 150)))
		MSG_FAIL("CCrosshair::Set_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER)))
		MSG_FAIL("CCrosshair::Set_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");

	return S_OK;
}

HRESULT CCrosshair::Release_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CCrosshair::Set_RenderState - m_pGraphicDevice == nullptr");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE)))
		MSG_FAIL("CCrosshair::Set_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");

	return S_OK;
}

HRESULT CCrosshair::Tick_CrosshairRay(CGameInstance* pGameInstance)
{
	_vec3 vCrosshairPos;
	vCrosshairPos.x = 0.f;
	vCrosshairPos.y = 0.f;
	vCrosshairPos.z = 0.f;

	_mat matProj = pGameInstance->Get_CamProj();
	D3DXMatrixInverse(&matProj, nullptr, &matProj);
	D3DXVec3TransformCoord(&vCrosshairPos, &vCrosshairPos, &matProj);

	m_vCrosshairPivot = _vec3(0.f, 0.f, 0.f);
	m_vCrosshairRay = vCrosshairPos - m_vCrosshairPivot;

	_mat matView = pGameInstance->Get_CamView();
	D3DXMatrixInverse(&matView, nullptr, &matView);
	D3DXVec3TransformCoord(&m_vCrosshairPivot, &m_vCrosshairPivot, &matView);
	D3DXVec3TransformNormal(&m_vCrosshairRay, &m_vCrosshairRay, &matView);

	CCrosshairMgr::Get_Instance()->Set_PivotToMgr(m_vCrosshairPivot);
	CCrosshairMgr::Get_Instance()->Set_RayToMgr(m_vCrosshairRay);

	return S_OK;
}

CCrosshair * CCrosshair::Create(_pDevice pDevice)
{
	CCrosshair* pInstance = new CCrosshair(pDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("CCrosshair Create Failed");
	}

	return pInstance;
}

void CCrosshair::Free()
{
	Safe_Release(m_pRenderer);
	Safe_Release(m_pTransform);
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pTexture);

	__super::Free();
}

CGameObject * CCrosshair::Clone(void * pArg)
{
	CGameObject* pInstance = new CCrosshair(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("CCrosshair Clone Failed");
	}

	return pInstance;
}

_vec3 CCrosshair::Get_MinPos()
{
	return _vec3();
}

_vec3 CCrosshair::Get_MaxPos()
{
	return _vec3();
}

_vec3 CCrosshair::Get_WorldPos()
{
	return _vec3();
}

Engine::_vec3 CCrosshair::Get_Length()
{
	return m_pTransform->Get_Scale();
}
