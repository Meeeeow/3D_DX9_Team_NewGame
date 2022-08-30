#include "stdafx.h"
#include "..\public\DmgFont.h"
#include "GameInstance.h"
#include "VIBufferRectColor.h"

void CDmgFont::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
}

CGameObject * CDmgFont::Clone(void * pArg /*= nullptr*/)
{
	CGameObject* pInstance = new CDmgFont(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("CDmgFont Clone Failed");
	}

	return pInstance;
}

CDmgFont * CDmgFont::Create(_pDevice pDevice)
{
	CDmgFont* pInstance = new CDmgFont(pDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("CDmgFont Create Failed");
	}

	return pInstance;
}

_vec3 CDmgFont::Get_MinPos()
{
	return _vec3();
}

_vec3 CDmgFont::Get_MaxPos()
{
	return _vec3();
}

_vec3 CDmgFont::Get_WorldPos()
{
	return _vec3();
}

_vec3 CDmgFont::Get_Length()
{
	return _vec3();
}

CDmgFont::CDmgFont(_pDevice pDevice)
	:CEffect(pDevice)
{
}

CDmgFont::CDmgFont(const CDmgFont & rhs)
	: CEffect(rhs)
{
}

HRESULT CDmgFont::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CDmgFont NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CDmgFont::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CDmgFont NativeConstruct - __super");

	DESC tDesc = *(DESC*)(pArg);

	//가드 한 경우
	if (tDesc.bGuard)
		m_bGuard = true;
	else
	//리스트에 데미지 값 채워넣음
		SetUp_ListDmg(tDesc);

	if (FAILED(SetUp_Component(pArg)))
		MSG_FAIL("CDmgFont NativeConstruct - SetUp_Component");

	m_tTimeDie.fDelay = 1.f;
	m_pTransformCom->SetUp_Scale(m_fScaleX, m_fScaleY, 1.f);

	return S_OK;
}

_int CDmgFont::Tick(_float fTimeDelta)
{
	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CDmgFont Tick - __super");

	if (m_bDead)
		return OBJ_DEAD;

	_float fUp = m_pTransformCom->Go_Up(fTimeDelta);
	m_vOriginalPos.y += fUp;

	m_tTimeDie.Tick_Time(fTimeDelta);
	if (m_tTimeDie.Check_Time())
	{
		m_fScaleX += 0.2f * m_vScaleChange.x;
		m_fScaleY -= 0.2f * m_vScaleChange.y;
		if (m_fScaleY <= 0.f)
		{
			Set_Dead();
			m_fScaleX = 0.f;
		}
	}
	else if (m_fScaleX > 1.f * m_vScaleChange.x && m_fScaleY > 1.f * m_vScaleChange.y)
	{
		m_fScaleX -= 0.3f * m_vScaleChange.x;
		m_fScaleY -= 0.3f * m_vScaleChange.y;
	}

	m_pTransformCom->SetUp_Scale(m_fScaleX, m_fScaleY, 1.f);

	return _int();
}

_int CDmgFont::LateTick(_float fTimeDelta)
{
	if (0 > (__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CDmgFont LateTick - __super");

	//데미지 0 들어왔는데 Tick에서 m_bDead 리턴 안타고 레이트 틱부터 들어온 경우 예외처리
	//가드인 경우에는 렌더 타야함
	if (m_listDmg.size() == 0 && !m_bGuard)
		return 0;

	if (m_pRendererCom == nullptr)
		MSG_MINUSONE("CDmgFont::LateTick - m_pRendererCom == nullptr");

	BillBoarding(m_pTransformCom);

	Compute_Distance(m_pTransformCom);

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_GROUP::GROUP_ALPHA, this)))
		MSG_MINUSONE("CDmgFont LateTick - Add_RenderGroup");

	return _int();
}

HRESULT CDmgFont::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("CDmgFont Render - __super");

	if (m_pRendererCom == nullptr)
		MSG_FAIL("CDmgFont Render - m_pRendererCom is null");

	if (m_pVIBufferCom == nullptr)
		MSG_FAIL("CDmgFont Render - m_pVIBufferCom is null");

	if (m_pTextureCom == nullptr)
		MSG_FAIL("CDmgFont Render - m_pTextureCom is null");

	m_pTransformCom->SetUp_State(CTransform::STATE_POS, m_vOriginalPos);


	if (FAILED(Set_RenderState()))
		MSG_FAIL("CDmgFont Render - Set_RenderState");

	if (m_bGuard)
	{
		if (FAILED(m_pTransformCom->Bind_OnDevice()))
			MSG_FAIL("CDmgFont Render - m_pTransformCom Bind_OnDevice");

		if (FAILED(m_pTextureCom->Bind_OnDevice(10)))
			MSG_FAIL("CDmgFont Render - m_pTextureCom Bind_OnDevice");

		if (FAILED(m_pVIBufferCom->Render()))
			MSG_FAIL("CDmgFont Render - m_pVIBufferCom Render");
	}
	else
	{
		for (auto& iDmg : m_listDmg)
		{
			if (FAILED(m_pTransformCom->Bind_OnDevice()))
				MSG_FAIL("CDmgFont Render - m_pTransformCom Bind_OnDevice");

			if (FAILED(m_pTextureCom->Bind_OnDevice(iDmg)))
				MSG_FAIL("CDmgFont Render - m_pTextureCom Bind_OnDevice");

			if (FAILED(m_pVIBufferCom->Render()))
				MSG_FAIL("CDmgFont Render - m_pVIBufferCom Render");

			m_pTransformCom->Go_Right(1.f);
		}
	}
	
	if (FAILED(Release_RenderState()))
		MSG_FAIL("CDmgFont Render - Release_RenderState");

	return S_OK;
}

HRESULT CDmgFont::Set_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CDmgFont Set_RenderState - m_pGraphicDevice is null");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE)))
		MSG_FAIL("CDmgFont::Set_RenderState - SetRenderState(D3DRS_ALPHATESTENABLE, TRUE)");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 150)))
		MSG_FAIL("CDmgFont::Set_RenderState - SetRenderState(D3DRS_SRCBLEND, D3DBLEND::D3DBLEND_SRCALPHA)");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER)))
		MSG_FAIL("CDmgFont::Set_RenderState - SetRenderState(D3DRS_DESTBLEND, D3DBLEND::D3DBLEND_INVSRCALPHA)");

	return S_OK;
}

HRESULT CDmgFont::Release_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CDmgFont Set_RenderState - m_pGraphicDevice is null");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE)))
		MSG_FAIL("CDmgFont::Set_RenderState - SetRenderState");

	return S_OK;
}

HRESULT CDmgFont::SetUp_Component(void* pArg)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	//트랜스폼 세팅하고 넘겨받은 좌표, 스케일 적용
	CTransform::tagTransformDesc tTransformDesc;
	tTransformDesc.fSpeed = 1.f;
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTransform"), TEXT("ComTransform"), (CComponent**)&m_pTransformCom, &tTransformDesc)))
		MSG_FAIL("CDmgFont SetUp_Component - Add_Component m_pTransformCom");

	if (pArg == nullptr)
		MSG_FAIL("CDmgFont SetUp_Component - pArg == nullptr");
	DESC tDesc = *(DESC*)(pArg);
	m_pTransformCom->SetUp_State(CTransform::STATE_POS, tDesc.vPos);
	m_vScaleChange = tDesc.vScale;
	m_vOriginalPos = tDesc.vPos;

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeRenderer, pTagComRenderer, (CComponent**)&m_pRendererCom)))
		MSG_FAIL("CDmgFont SetUp_Component - Add_Component m_pRendererCom");

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeTextureDmgFont, pTagComTexture, (CComponent**)&m_pTextureCom)))
		MSG_FAIL("CDmgFont SetUp_Component - Add_Component m_pTextureCom");

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeVIBufferRect, pTagComVIBuffer, (CComponent**)&m_pVIBufferCom)))
		MSG_FAIL("CDmgFont SetUp_Component - Add_Component m_pVIBufferCom");

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CDmgFont::SetUp_ListDmg(DESC tDesc)
{
	if (tDesc.iDmg == 0)
	{
		Set_Dead();
		return;
	}

	int iR = NULL;
	while (true)
	{
		iR = tDesc.iDmg % 10;
		tDesc.iDmg /= 10;

		if (iR == 0 && tDesc.iDmg == 0)
			break;

		m_listDmg.emplace_front(iR);
	}
}