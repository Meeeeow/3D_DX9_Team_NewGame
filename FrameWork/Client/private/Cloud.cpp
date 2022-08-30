#include "stdafx.h"
#include "..\public\Cloud.h"
#include "PlayerTagMgr.h"


CCloud::CCloud(_pDevice pGraphicDevice)
	: CGameObject(pGraphicDevice)
{
}

CCloud::CCloud(const CCloud & rhs)
	: CGameObject(rhs)
{
}

HRESULT CCloud::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CIceOrb NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CCloud::NativeConstruct(void * pArg)
{
	if (pArg == nullptr)
		MSG_FAIL("CIceOrb::NativeConstruct - pArg == nullptr");

	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CIceOrb NativeConstruct - __super::NativeConstruct");

	if (FAILED(SetUp_Component(pArg)))
		MSG_FAIL("CIceOrb NativeConstruct - SetUp_Component");

	return S_OK;
}

_int CCloud::Tick(_float fTimeDelta)
{
	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CIceOrb Tick - __super");

	if (m_bDead)
		return OBJ_DEAD;

	Move(fTimeDelta);

	Shot(fTimeDelta);

	Update_Frame(fTimeDelta);

	return 0;
}

_int CCloud::LateTick(_float fTimeDelta)
{
	if (0 > (__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CIceOrb LateTick - __super");

	BillBoarding(m_pTransformCom);

	if (m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONALPHA, this))
		MSG_MINUSONE("CIceOrb::LateTick - Add_RenderGroup");

	return _int();
}

HRESULT CCloud::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("CIceOrb Render - __super");

	if (m_pTransformCom == nullptr ||
		m_pVIBufferCom == nullptr)
		MSG_FAIL("CIceOrb::Render - Com is null");

	if (FAILED(m_pTransformCom->Bind_OnDevice()))
		MSG_FAIL("CIceOrb::Render - m_pTransformCom Bind_OnDevice");

	if (m_eCurState == STATE_CHARGE)
	{
		if (FAILED(m_pTextureCom1->Bind_OnDevice(m_tFram.iStart)))
			MSG_FAIL("CIceOrb::Render - m_pTextureComArr->Bind_OnDevice");
	}
	else
	{
		if (FAILED(m_pTextureCom2->Bind_OnDevice(m_tFram.iStart)))
			MSG_FAIL("CIceOrb::Render - m_pTextureComArr->Bind_OnDevice");
	}
	if (FAILED(Set_RenderState()))
		MSG_FAIL("CIceOrb::Render - Set_RenderState");

	if (FAILED(m_pVIBufferCom->Render()))
		MSG_FAIL("CIceOrb::Render - m_pVIBufferCom->Render");

	if (FAILED(Release_RenderState()))
		MSG_FAIL("CIceOrb::Render - Release_RenderState");

	return S_OK;
}

HRESULT CCloud::SetUp_Component(void * pArg)
{
	CTransform::TRANSFORM_DESC tTransformDesc;
	tTransformDesc.fSpeed = 10.f;
	tTransformDesc.fDegree = 3.f;

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeTransform, pTagComTransform, (CComponent**)&m_pTransformCom, &tTransformDesc)))
		MSG_FAIL("CIceOrb::SetUp_Component - m_pTransformCom");

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeRenderer, pTagComRenderer, (CComponent**)&m_pRendererCom)))
		MSG_FAIL("CIceOrb::SetUp_Component - m_pRendererCom");

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeVIBufferRect, pTagComVIBuffer, (CComponent**)&m_pVIBufferCom)))
		MSG_FAIL("CIceOrb::SetUp_Component - m_pVIBufferCom");

	if (FAILED(Add_Component(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureCloud_Charge"), TEXT("ComTextureCloud_Charge"), (CComponent**)&m_pTextureCom1)))
		MSG_FAIL("CIceOrb::SetUp_Component - ComTextureCloud_Charge");

	if (FAILED(Add_Component(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureCloud_Shot"), TEXT("ComTextureCloud_Shot"), (CComponent**)&m_pTextureCom2)))
		MSG_FAIL("CIceOrb::SetUp_Component - ComTextureCloud_Shot");

	m_pPlayerTransformCom = CPlayerTagMgr::Get_Instance()->Get_CurPlayerTransform();
	if (m_pPlayerTransformCom == nullptr)
		MSG_FAIL("CIceOrb::AI - m_pTransformComPlayer == nullptr");

	m_tFram.Set_Value(m_pTextureCom1->Get_ImageCount() - 1);

	return S_OK;
}

HRESULT CCloud::Set_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CIceOrb::Set_RenderState - m_pGraphicDevice == nullptr");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE)))
		MSG_FAIL("CIceOrb::Set_RenderState - m_pGraphicDevice->SetRenderState");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 150)))
		MSG_FAIL("CIceOrb::Set_RenderState - m_pGraphicDevice->SetRenderState");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER)))
		MSG_FAIL("CIceOrb::Set_RenderState - m_pGraphicDevice->SetRenderState");

	return S_OK;
}

HRESULT CCloud::Release_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CIceOrb::Release_RenderState - m_pGraphicDevice == nullptr");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE)))
		MSG_FAIL("CIceOrb::Release_RenderState - m_pGraphicDevice->SetRenderState");

	return S_OK;
}

HRESULT CCloud::BillBoarding(CTransform * pTransform)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_mat matCamWorld = pGameInstance->Get_CamWorld();
	_vec3 vEffectScale = pTransform->Get_Scale();

	pTransform->SetUp_State(CTransform::STATE_RIGHT, (*(_vec3*)(&matCamWorld.m[0][0])) * vEffectScale.x);
	pTransform->SetUp_State(CTransform::STATE_UP, (*(_vec3*)(&matCamWorld.m[1][0])) * vEffectScale.y);
	pTransform->SetUp_State(CTransform::STATE_LOOK, (*(_vec3*)(&matCamWorld.m[2][0])) * vEffectScale.z);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CCloud::Move(_float fTimeDelta)
{
	if (true == m_bShot)
		return S_OK;

	if (true == m_bChase)
	{
		_vec3 vPos;
		_mat matPos = *m_pPlayerTransformCom->Get_WorldMatrixPtr();
		memcpy(&vPos, &matPos.m[3][0], sizeof(_vec3));
		vPos.y += 2.5f;
		m_pTransformCom->SetUp_State(CTransform::STATE_POS, vPos);
	}

	m_fChargeTime += fTimeDelta;

	if (m_fShotTime <= m_fChargeTime)
	{
		if (true == m_bChase)
		{
			m_bChase = false;

			m_fChargeTime = 0.f;
			m_fShotTime = 0.9f;
		}
		else
		{
			m_bShot = true;

			m_eCurState = STATE_SHOT;
			m_tFram.Set_Value(m_pTextureCom2->Get_ImageCount() - 1);

			m_fChargeTime = 0.f;
			m_fShotTime = 0.9f;

			CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

			OBJ_INFO	tObj;
			_vec3		vPos;
			_mat matWorld = *m_pTransformCom->Get_WorldMatrixPtr();

			tObj.matWorld = matWorld;
			vPos = tObj.Get_State(OBJ_INFO::STATE_POS);
			vPos.y -= 1.7f;
			tObj.SetUp_Scale(3.f, 3.f, 3.f);
			tObj.SetUp_State(OBJ_INFO::STATE_POS, vPos);

			pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, TEXT("PrototypeElectric"), pTagLayerMonsterAttack, &tObj);

			RELEASE_INSTANCE(CGameInstance);
		}
	}

	return S_OK;
}

void CCloud::Shot(_float fTimeDelta)
{
	if (false == m_bShot)
		return;

	m_fChargeTime += fTimeDelta;

	if (m_fShotTime <= m_fChargeTime)
	{
		m_bDead = true;
	}
}

void CCloud::Update_Frame(_float fTimeDelta)
{
	m_tFram.fTime += fTimeDelta;

	if (m_tFram.fDelay <= m_tFram.fTime)
	{
		++m_tFram.iStart;

		m_tFram.fTime = 0.f;

		if (m_tFram.iEnd <= m_tFram.iStart)
		{
			m_tFram.iStart = 0;
		}
	}
}

void CCloud::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureCom1);
	Safe_Release(m_pTextureCom2);
}

CGameObject * CCloud::Clone(void * pArg)
{
	CCloud* pInstance = new CCloud(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("CIceOrb MSG_FAIL Failed");
	}

	return pInstance;
}

CCloud * CCloud::Create(_pDevice pGraphicDevice)
{
	CCloud* pInstance = new CCloud(pGraphicDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("CIceOrb Create Failed");
	}

	return pInstance;
}

_vec3 CCloud::Get_MinPos()
{
	return _vec3();
}

_vec3 CCloud::Get_MaxPos()
{
	return _vec3();
}

_vec3 CCloud::Get_WorldPos()
{
	return _vec3();
}

_vec3 CCloud::Get_Length()
{
	return _vec3();
}
