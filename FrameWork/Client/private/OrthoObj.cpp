#include "stdafx.h"
#include "..\public\OrthoObj.h"

COrthoObj::COrthoObj(_pDevice pDevice)
	:CGameObject(pDevice)
{
}

COrthoObj::COrthoObj(const COrthoObj & rhs)
	:CGameObject(rhs)
{
}

HRESULT COrthoObj::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("COrthoObj::NativeConstruct_Prototype - __super::NativeConstruct_Prototype");

	return S_OK;
}

HRESULT COrthoObj::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("COrthoObj::NativeConstruct - __super::NativeConstruct");

	if (FAILED(SetUp_Component(pArg)))
		MSG_FAIL("COrthoObj::NativeConstruct - SetUp_Component");

	D3DXMatrixOrthoLH(&m_matProject, (float)g_iWinCX, (float)g_iWinCY, 0.f, 1.f);
	D3DXMatrixIdentity(&m_matIdentity);

	return S_OK;
}

_int COrthoObj::Tick(_float fTimeDelta)
{
	if (FAILED(__super::Tick(fTimeDelta)))
		MSG_MINUSONE("COrthoObj::Tick - __super::Tick");

	return _int();
}

_int COrthoObj::LateTick(_float fTimeDelta)
{
	if (FAILED(__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("COrthoObj::LateTick - __super::LateTick");

	if (m_pRenderer == nullptr)
		MSG_MINUSONE("COrthoObj::LateTick - m_pRenderer == nullptr");

	if (m_bAlphaTest)
	{
		if (FAILED(m_pRenderer->Add_RenderGroup(CRenderer::GROUP_UI, this)))
			MSG_MINUSONE("COrthoObj::LateTick - Add_RenderGroup");
	}
	else
	{
		if (FAILED(m_pRenderer->Add_RenderGroup(CRenderer::GROUP_UI_ALPHA, this)))
			MSG_MINUSONE("COrthoObj::LateTick - Add_RenderGroup");
	}

	return _int();
}

HRESULT COrthoObj::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("COrthoObj::Render - __super::Render");

	if (m_pTransform == nullptr ||
		m_pTexture == nullptr ||
		m_pVIBuffer == nullptr)
		MSG_FAIL("COrthoObj::Render - com is null");

	SetUp_RenderState();

	if (FAILED(m_pTransform->Bind_OnDevice()))
		MSG_FAIL("COrthoObj::Render - m_pTransform->Bind_OnDevice");

	if (FAILED(m_pTexture->Bind_OnDevice(m_tFrame.iStart)))
		MSG_FAIL("COrthoObj::Render - m_pTexture->Bind_OnDevice");

	if (FAILED(m_pVIBuffer->Render()))
		MSG_FAIL("COrthoObj::Render - m_pVIBuffer->Render");

	Release_RenderState();

	return S_OK;
}

void COrthoObj::Set_OrthoMatrix()
{
	_float fSizeX = _float(g_iWinCX);
	_float fSizeY = _float(g_iWinCY);
	_float fX = m_pTransform->Get_State(CTransform::STATE_POS).x;
	_float fY = m_pTransform->Get_State(CTransform::STATE_POS).y;

	m_pTransform->SetUp_State(CTransform::STATE::STATE_POS, _vec3(fX - (fSizeX * 0.5f), -fY + (fSizeY * 0.5f), 0.f));
	m_pTransform->SetUp_Scale(fSizeX, fSizeY, 1.f);
}

void COrthoObj::Set_FrameValue(int iEnd, int iStart /*= 0*/, float fDelay /*= 0.1f*/, float fTime /*= 0.f*/)
{
	m_tFrame.Set_Value(iEnd, iStart, fDelay, fTime);
}

_bool COrthoObj::Tick_Frame(_float fTimeDelta)
{
	m_tFrame.Tick_Time(fTimeDelta);
	if (m_tFrame.Check_Time())
	{
		m_tFrame.iStart++;
		m_tFrame.tTime.fTime = 0.f;

		if (m_tFrame.iStart >= m_tFrame.iEnd)
		{
			m_tFrame.iStart = 0;
			return true;
		}
	}

	return false;
}

HRESULT COrthoObj::SetUp_Component(void * pArg)
{
	DESC tDesc = *(DESC*)(pArg);

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeTransform, pTagComTransform, (CComponent**)&m_pTransform)))
		MSG_FAIL("COrthoObj::SetUp_Component - pTagPrototypeTransform");
	m_pTransform->Set_Matrix(tDesc.tObjInfo.matWorld);
	Set_OrthoMatrix();

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeRenderer, pTagComRenderer, (CComponent**)&m_pRenderer)))
		MSG_FAIL("COrthoObj::SetUp_Component - pTagPrototypeRenderer");

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeVIBufferRect, pTagComVIBuffer, (CComponent**)&m_pVIBuffer)))
		MSG_FAIL("COrthoObj::SetUp_Component - pTagPrototypeVIBufferRect");

	if (FAILED(Add_Component(tDesc.eTextureLevel, tDesc.tObjInfo.szTextureName, pTagComTexture, (CComponent**)&m_pTexture)))
		MSG_FAIL("COrthoObj::SetUp_Component - pTagComTexture");

	return S_OK;
}

void COrthoObj::SetUp_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_VOID("COrthoObj::SetUp_RenderState - m_pGraphicDevice == nullptr");

	if (FAILED(m_pGraphicDevice->SetTransform(D3DTS_VIEW, &m_matIdentity)))
		MSG_VOID("COrthoObj::SetUp_RenderState - D3DTS_VIEW, &m_matIdentity");

	if (FAILED(m_pGraphicDevice->SetTransform(D3DTS_PROJECTION, &m_matProject)))
		MSG_VOID("COrthoObj::SetUp_RenderState - D3DTS_PROJECTION, &m_matProject");

	//알파 테스팅
	if (m_bAlphaTest)
	{
		if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE)))
			MSG_VOID("COrthoObj::SetUp_RenderState - D3DRS_ALPHATESTENABLE, TRUE");

		if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, m_iAlphaRef)))
			MSG_VOID("COrthoObj::SetUp_RenderState - D3DRS_ALPHAREF, TRUE");

		if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER)))
			MSG_VOID("COrthoObj::SetUp_RenderState - D3DRS_ALPHAFUNC, D3DCMP_GREATER");
	}
	//알파 블랜드
	else
	{
		if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE)))
			MSG_VOID("COrthoObj::SetUp_RenderState - SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE)");

		if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND::D3DBLEND_SRCALPHA)))
			MSG_VOID("COrthoObj::SetUp_RenderState - SetRenderState(D3DRS_SRCBLEND, D3DBLEND::D3DBLEND_SRCALPHA)");

		if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND::D3DBLEND_INVSRCALPHA)))
			MSG_VOID("COrthoObj::SetUp_RenderState - SetRenderState(D3DRS_DESTBLEND, D3DBLEND::D3DBLEND_INVSRCALPHA)");
	}
}

void COrthoObj::Release_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_VOID("COrthoObj::Release_RenderState - m_pGraphicDevice == nullptr");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE)))
		MSG_VOID("COrthoObj::Release_RenderState - D3DRS_ALPHATESTENABLE, FALSE");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE)))
		MSG_VOID("COrthoObj::Release_RenderState - D3DRS_ALPHABLENDENABLE, FALSE");

}

_vec3 COrthoObj::Get_MinPos()
{
	return _vec3();
}

_vec3 COrthoObj::Get_MaxPos()
{
	return _vec3();
}

_vec3 COrthoObj::Get_WorldPos()
{
	return _vec3();
}

_vec3 COrthoObj::Get_Length()
{
	return _vec3();
}

void COrthoObj::Free()
{
	Safe_Release(m_pTransform);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pTexture);
	Safe_Release(m_pVIBuffer);
	__super::Free();
}
