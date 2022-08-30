#include "stdafx.h"
#include "..\public\TagGauge.h"
#include "GameInstance.h"
#include "VIBufferRectColor.h"
#include "TagGaugeObserver.h"
#include "PlayerTagMgr.h"

void CTagGauge::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
}

CGameObject * CTagGauge::Clone(void * pArg /*= nullptr*/)
{
	CGameObject* pInstance = new CTagGauge(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("CTagGauge Clone Failed");
	}

	return pInstance;
}

CTagGauge * CTagGauge::Create(_pDevice pDevice)
{
	CTagGauge* pInstance = new CTagGauge(pDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("CTagGauge Create Failed");
	}

	return pInstance;
}

_vec3 CTagGauge::Get_MinPos()
{
	return _vec3();
}

_vec3 CTagGauge::Get_MaxPos()
{
	return _vec3();
}

_vec3 CTagGauge::Get_WorldPos()
{
	return _vec3();
}

_vec3 CTagGauge::Get_Length()
{
	return _vec3();
}

CTagGauge::CTagGauge(_pDevice pDevice)
	:CEffect(pDevice)
{
}

CTagGauge::CTagGauge(const CTagGauge & rhs)
	: CEffect(rhs)
{
}

HRESULT CTagGauge::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CTagGauge NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CTagGauge::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CTagGauge NativeConstruct - __super");

	if (FAILED(SetUp_Component(pArg)))
		MSG_FAIL("CTagGauge NativeConstruct - SetUp_Component");

	DESC tDesc = *(DESC*)(pArg);
	D3DXMatrixOrthoLH(&m_matProject, (float)g_iWinCX, (float)g_iWinCY, 0.f, 1.f);
	_float fSizeX = tDesc.vScale.x;
	_float fSizeY = tDesc.vScale.y;
	_float fX = tDesc.vPos.x;
	_float fY = tDesc.vPos.y;

	m_pTransformCom->SetUp_State(CTransform::STATE::STATE_POS, _vec3(fX - ((float)g_iWinCX * 0.5f), fY - ((float)g_iWinCY * 0.5f), 0.f));
	m_pTransformCom->SetUp_Scale(fSizeX, fSizeY, 1.f);

	//타입 설정
	m_eType = tDesc.eType;
	//전사면 바로 처음부터 렌더
	if (m_eType == CPlayer::TYPE_WARRIOR)
		m_bRender = true;

	//옵저버 생성
	CObserver* pObserver = nullptr;
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pTagPrototypeObserverTagGauge, pTagLayerObserver, this, (void**)&pObserver)))
		MSG_FAIL("CTagGauge::NativeConstruct - pTagPrototypeObserverTagGauge");
	CPlayerTagMgr::Get_Instance()->Subscribe_ChangePlayer(pObserver);
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

_int CTagGauge::Tick(_float fTimeDelta)
{
	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CTagGauge Tick - __super");

	return _int();
}

_int CTagGauge::LateTick(_float fTimeDelta)
{
	if (0 > (__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CTagGauge LateTick - __super");

	if (m_pRendererCom == nullptr)
		MSG_MINUSONE("CTagGauge::LateTick - m_pRendererCom == nullptr");

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_GROUP::GROUP_UI_ALPHA, this)))
		MSG_MINUSONE("CTagGauge LateTick - Add_RenderGroup");

	return _int();
}

HRESULT CTagGauge::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("CTagGauge Render - __super");

	//렌더 트루인 친구만 렌더하기
	if (!m_bRender)
		return S_OK;

	_float fTagCooltimeRatio = CPlayerTagMgr::Get_Instance()->Get_TagCooltimeRatio();

	if (m_pRendererCom == nullptr)
		MSG_FAIL("CTagGauge Render - m_pRendererCom is null");

	if (m_pVIBufferCom == nullptr)
		MSG_FAIL("CTagGauge Render - m_pVIBufferCom is null");

	if (m_pTextureCom == nullptr)
		MSG_FAIL("CTagGauge Render - m_pTextureCom is null");

	//쿨타임 비율에 맞게 버퍼 수정해야함
	if (FAILED(m_pVIBufferCom->Decrease_Top(fTagCooltimeRatio)))
		MSG_FAIL("CTagGauge Render - Decrease_Top");

	if (FAILED(m_pTransformCom->Bind_OnDevice()))
		MSG_FAIL("CTagGauge Render - m_pTransformCom Bind_OnDevice");

	if (FAILED(m_pTextureCom->Bind_OnDevice(3)))
		MSG_FAIL("CTagGauge Render - m_pTextureCom Bind_OnDevice");

	if (FAILED(Set_RenderState()))
		MSG_FAIL("CTagGauge Render - Set_RenderState");

	if (FAILED(m_pVIBufferCom->Render()))
		MSG_FAIL("CTagGauge Render - m_pVIBufferCom Render");

	if (FAILED(Release_RenderState()))
		MSG_FAIL("CTagGauge Render - Release_RenderState");

	//버퍼 수정한거 다시 원상 복구해야 다른 객체들이 문제 안생김
	if (FAILED(m_pVIBufferCom->Reset()))
		MSG_FAIL("CTagGauge Render - Reset");

	return S_OK;
}

void CTagGauge::Change_Player(CPlayer::TYPE eType)
{
	if (m_eType == eType)
		m_bRender = true;
	else
		m_bRender = false;
}

HRESULT CTagGauge::Set_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CTagGauge Set_RenderState - m_pGraphicDevice is null");

	_mat tmp;
	D3DXMatrixIdentity(&tmp);

	m_pGraphicDevice->SetTransform(D3DTS_VIEW, &tmp);
	m_pGraphicDevice->SetTransform(D3DTS_PROJECTION, &m_matProject);

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE)))
		MSG_FAIL("CTagGauge::Set_RenderState - SetRenderState(D3DRS_ALPHATESTENABLE, TRUE)");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND::D3DBLEND_SRCALPHA)))
		MSG_FAIL("CTagGauge::Set_RenderState - SetRenderState(D3DRS_SRCBLEND, D3DBLEND::D3DBLEND_SRCALPHA)");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND::D3DBLEND_INVSRCALPHA)))
		MSG_FAIL("CTagGauge::Set_RenderState - SetRenderState(D3DRS_DESTBLEND, D3DBLEND::D3DBLEND_INVSRCALPHA)");

	return S_OK;
}

HRESULT CTagGauge::Release_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CTagGauge Set_RenderState - m_pGraphicDevice is null");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE)))
		MSG_FAIL("CTagGauge::Set_RenderState - SetRenderState");

	return S_OK;
}

HRESULT CTagGauge::SetUp_Component(void* pArg)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	//트랜스폼 세팅하고 넘겨받은 좌표, 스케일 적용
	CTransform::tagTransformDesc tTransformDesc;
	tTransformDesc.fSpeed = 1.f;
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTransform"), TEXT("ComTransform"), (CComponent**)&m_pTransformCom, &tTransformDesc)))
		MSG_FAIL("CTagGauge SetUp_Component - Add_Component m_pTransformCom");

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeRenderer, pTagComRenderer, (CComponent**)&m_pRendererCom)))
		MSG_FAIL("CTagGauge SetUp_Component - Add_Component m_pRendererCom");

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeTexturePlayerInfo, pTagComTexture, (CComponent**)&m_pTextureCom)))
		MSG_FAIL("CTagGauge SetUp_Component - Add_Component m_pTextureCom");

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeVIBufferRect, pTagComVIBuffer, (CComponent**)&m_pVIBufferCom)))
		MSG_FAIL("CTagGauge SetUp_Component - Add_Component m_pVIBufferCom");

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}