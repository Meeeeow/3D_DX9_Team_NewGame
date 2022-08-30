#include "stdafx.h"
#include "..\public\SkillIcon.h"
#include "GameInstance.h"
#include "Player.h"


CSkillIcon::CSkillIcon(_pDevice pGraphicDevice)
	: CObserver(pGraphicDevice)
{
}

CSkillIcon::CSkillIcon(const CSkillIcon & rhs)
	: CObserver(rhs)
{
}

HRESULT CSkillIcon::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("Failed To CSkillIcon NativeConstruct_Prototype - super");

	return S_OK;
}

HRESULT CSkillIcon::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("Failed To CSkillIcon NativeConstruct - super");

	// ���� ���� �ҷ���       ������ �޾ƿý� �ּ� �����Ұ�

	OBJ_INFO* pInfo = (OBJ_INFO*)pArg;

	D3DXMatrixOrthoLH(&m_matProject, (float)g_iWinCX, (float)g_iWinCY, 0.f, 1.f);
	m_fSizeX = pInfo->Get_Scale().x;
	m_fSizeY = pInfo->Get_Scale().y;
	m_fX = pInfo->Get_State(OBJ_INFO::STATE_POS).x;
	m_fY = pInfo->Get_State(OBJ_INFO::STATE_POS).y;

	if (FAILED(SetUp_Component()))
		MSG_FAIL("CBackGround NativeConstruct - SetUp_Component");

	m_pTransformCom->SetUp_State(CTransform::STATE::STATE_POS, _vec3(m_fX - ((float)g_iWinCX * 0.5f), m_fY - ((float)g_iWinCY * 0.5f), 0.f));
	m_pTransformCom->SetUp_Scale(m_fSizeX, m_fSizeY, 1.f);

	return S_OK;
}

_int CSkillIcon::Tick(_float fTimeDelta)
{
	// ��Ÿ��
	if (0.f <= m_fCoolTime)
	{
		m_fCoolTime -= fTimeDelta;

		if (0.f >= m_fCoolTime)
		{
			m_fCoolTime = 0.f;
			m_bCoolDown = false;
		}
	}

	return _int();
}

_int CSkillIcon::LateTick(_float fTimeDelta)
{
	if (true == m_bControl)
	{
		if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_GROUP::GROUP_UI, this)))
			MSG_MINUSONE("CPlayer LateTick - Add_RenderGroup");
	}

	return _int();
}

HRESULT CSkillIcon::Render()
{

	if (FAILED(__super::Render()))
		MSG_FAIL("CBackGround Render - __super");

	if (m_pRendererCom == nullptr)
		MSG_FAIL("CBackGround Render - m_pRendererCom is null");

	if (m_pVIBufferCom == nullptr)
		MSG_FAIL("CBackGround Render - m_pVIBufferCom is null");

	if (m_pWarriorSkillTextureCom == nullptr &&
		m_pMageSkillTextureCom == nullptr &&
		m_pThiefSkillTextureCom == nullptr)
		MSG_FAIL("CBackGround Render - m_pTextureCom is null");

	m_pTransformCom->Bind_OnDevice();

	_mat tmp;
	D3DXMatrixIdentity(&tmp);

	m_pGraphicDevice->SetTransform(D3DTS_VIEW, &tmp);
	m_pGraphicDevice->SetTransform(D3DTS_PROJECTION, &m_matProject);

	// ��ų �̹��� �ѹ�
	switch (m_iClassType)
	{
	case 0:
		if (false == m_bCoolDown)
			m_pWarriorSkillTextureCom->Bind_OnDevice(m_iSkillNum);
		else
			m_pWarriorSkillTextureCom->Bind_OnDevice(m_iSkillNum + 3);
		break;
	case 1:
		if (false == m_bCoolDown)
			m_pMageSkillTextureCom->Bind_OnDevice(m_iSkillNum);
		else
			m_pMageSkillTextureCom->Bind_OnDevice(m_iSkillNum + 3);
		break;
	case 2:
		if (false == m_bCoolDown)
			m_pThiefSkillTextureCom->Bind_OnDevice(m_iSkillNum);
		else
			m_pThiefSkillTextureCom->Bind_OnDevice(m_iSkillNum + 3);
		break;
	}

	m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHAREF, 100);
	m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// �̹��� ���
	m_pVIBufferCom->Render();

	// �ؽ�Ʈ ���
	if (true == m_bCoolDown)
	{
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

		_tchar szBuff[MAX_PATH]{};

		if (1 <= m_fCoolTime)
			swprintf(szBuff, sizeof(_tchar)*MAX_PATH, TEXT("%d"), (_uint)m_fCoolTime);
		else
			swprintf(szBuff, sizeof(_tchar)*MAX_PATH, TEXT("%.1f"), m_fCoolTime);

		_vec3 vPos = { m_fX - lstrlen(szBuff) * 7.5f, g_iWinCY - m_fY - 15.f, 0.f };

		pGameInstance->Draw_Font(szBuff, D3DXCOLOR(1.f, 0.f, 0.f, 1.f), &vPos);

		RELEASE_INSTANCE(CGameInstance);
	}

	m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

void CSkillIcon::Notify(void * pMessage)
{
	CPlayer::PLAYER_SKILL * pArg = (CPlayer::PLAYER_SKILL*)pMessage;

	// ���� ���� ���
	if (nullptr == pArg)
	{
		// �̹��� ��� ǥ�� ��ȯ
		m_bCoolDown = true;
		// ��Ÿ�� ����
		m_fCoolTime = m_fMaxCoolTime;
	}
	// ���� ���� ���(�ʱ�ȭ)
	else
	{
		// ǥ�� ��ȯ
		//m_bControl = pArg->bControl;
		// ���� ����
		m_iClassType = pArg->eType;
		// ��ų ��ȣ ����
		m_iSkillNum = pArg->iSkillNum;
		// ���� Notify�� ������ ��ü�� ���� �̹��� ��ȣ ����
		++(pArg->iSkillNum);
		// 2���� Ŀ���� 0���� �ʱ�ȭ
		if (2 < pArg->iSkillNum)
			pArg->iSkillNum = 0;
		// ��Ÿ�� ����
		switch (m_iSkillNum)
		{
		case 0:
			m_fMaxCoolTime = pArg->fCooltime1;
			break;
		case 1:
			m_fMaxCoolTime = pArg->fCooltime2;
			break;
		case 2:
			m_fMaxCoolTime = pArg->fCooltime3;
			break;
		}
	}
}

HRESULT CSkillIcon::SetUp_Component()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTransform"), TEXT("ComTransform"), (CComponent**)&m_pTransformCom)))
		MSG_FAIL("CSkillIcon SetUp_Component - Add_Component m_pTransformCom");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeRenderer"), TEXT("ComRenderer"), (CComponent**)&m_pRendererCom)))
		MSG_FAIL("CSkillIcon SetUp_Component - Add_Component m_pRendererCom");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeVIBufferRect"), TEXT("ComVIBufferRect"), (CComponent**)&m_pVIBufferCom)))
		MSG_FAIL("CSkillIcon SetUp_Component - Add_Component m_pVIBufferCom");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTextureWarriorSkill"), TEXT("ComTextureWarrior"), (CComponent**)&m_pWarriorSkillTextureCom)))
		MSG_FAIL("CSkillIcon SetUp_Component - Add_Component m_pWarriorSkillTextureCom");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTextureMageSkill"), TEXT("ComTextureMage"), (CComponent**)&m_pMageSkillTextureCom)))
		MSG_FAIL("CSkillIcon SetUp_Component - Add_Component m_pMageSkillTextureCom");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTextureThiefSkill"), TEXT("ComTextureThief"), (CComponent**)&m_pThiefSkillTextureCom)))
		MSG_FAIL("CSkillIcon SetUp_Component - Add_Component m_pThiefSkillTextureCom");

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CSkillIcon * CSkillIcon::Create(_pDevice pGraphicDevice)
{
	CSkillIcon* pInstance = new CSkillIcon(pGraphicDevice);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed To CSkillIcon Create - NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSkillIcon::Clone(void * pArg)
{
	CSkillIcon* pInstance = new CSkillIcon(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed To CSkillIcon Create - NativeConstruct");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkillIcon::Free()
{
	__super::Free();

	Safe_Release(m_pWarriorSkillTextureCom);
	Safe_Release(m_pMageSkillTextureCom);
	Safe_Release(m_pThiefSkillTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
}

_vec3 CSkillIcon::Get_MinPos()
{
	return _vec3();
}

_vec3 CSkillIcon::Get_MaxPos()
{
	return _vec3();
}

_vec3 CSkillIcon::Get_WorldPos()
{
	return _vec3();
}

_vec3 CSkillIcon::Get_Length()
{
	return _vec3();
}
