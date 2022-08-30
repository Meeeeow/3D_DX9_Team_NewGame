#include "stdafx.h"
#include "..\public\TombStone.h"
#include "GameInstance.h"
#include "Attack.h"
#include "DmgFont.h"
#include "TombStoneManager.h"
#include "TombStoneParticles.h"


CTombStone::CTombStone(_pDevice pGraphicDevice)
	: CMonster(pGraphicDevice)
{
}

CTombStone::CTombStone(const CTombStone & rhs)
	: CMonster(rhs)
{
}

HRESULT CTombStone::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CChimera NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CTombStone::NativeConstruct(void * pArg)
{
	CMonster::MONSTER_STAT tMonsterStat;
	tMonsterStat.iMaxHp = 100;
	tMonsterStat.iHp = tMonsterStat.iMaxHp;
	tMonsterStat.iDmg = 20;

	if (FAILED(__super::NativeConstruct(&tMonsterStat)))
		MSG_FAIL("CChimera NativeConstruct - __super");

	if (FAILED(SetUp_Component(pArg)))
		MSG_FAIL("CChimera NativeConstruct - SetUp_Component");

	m_tParticle.fDelay = 0.5f;

	CTombStoneManager::Get_Instance()->Add_TombStone(this);

	return S_OK;
}

_int CTombStone::Tick(_float fTimeDelta)
{
	if (true == m_bDead)
	{
		CTombStoneManager::Get_Instance()->Sub_TombStone(this);
		return OBJ_DEAD;
	}

	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CChimera Tick - __super");

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	AI(fTimeDelta);

	Particle(fTimeDelta);

	Collision_Terrain(fTimeDelta, pGameInstance, m_pTransformCom);

	m_pBoxColliderCom->Set_WorldMatrixFromObject(*m_pTransformCom->Get_WorldMatrixPtr());
	m_pBoxColliderCom->Tick(fTimeDelta);

	RELEASE_INSTANCE(CGameInstance);

	return 0;
}

_int CTombStone::LateTick(_float fTimeDelta)
{
	if (0 > (__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CChimera LateTick - __super");

	if (m_pRendererCom == nullptr)
		MSG_MINUSONE("CChimera::LateTick - m_pRendererCom == nullptr");

	if (m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONALPHA, this))
		MSG_MINUSONE("CChimera::LateTick - Add_RenderGroup");

	return _int();
}

HRESULT CTombStone::Render()
{
	if (STATE_BROKEN == m_eState)
		return S_OK;

	if (FAILED(__super::Render()))
		MSG_FAIL("CChimera Render - __super");

	if (m_pTransformCom		== nullptr ||
		m_pTextureCom		== nullptr ||
		m_pBoxColliderCom	== nullptr ||
		m_pVIBufferCom		== nullptr)
		MSG_FAIL("CChimera::Render - Com is null");

	if (FAILED(m_pTransformCom->Bind_OnDevice()))
		MSG_FAIL("CChimera::Render - m_pTransformCom Bind_OnDevice");

	if (FAILED(m_pTextureCom->Bind_OnDevice()))
		MSG_FAIL("CChimera::Render - m_pTextureCom->Bind_OnDevice");

	if (FAILED(Set_RenderState()))
		MSG_FAIL("CChimera::Render - Set_RenderState");

	//if (FAILED(m_pVIBufferCom->Render()))
	//	MSG_FAIL("CChimera::Render - m_pVIBufferCom->Render");

	if (FAILED(Release_RenderState()))
		MSG_FAIL("CChimera::Render - Release_RenderState");

	if (FAILED(m_pBoxColliderCom->Render()))
		MSG_FAIL("CChimera::Render - m_pBoxColliderCom->Render");

	return S_OK;
}

_bool CTombStone::Picking(const _vec3 & vMouseRay, const _vec3 & vMousePivot, CTransform * pOut)
{
	/*return false;*/

	_vec3 vPos;
	if (m_pVIBufferCom->Picking(vMouseRay, vMousePivot, &vPos))
	{
		pOut = dynamic_cast<CTransform*>(Get_Component(pTagComTransform));
		return true;
	}
	return false;
}

HRESULT CTombStone::SetUp_Component(void * pArg)
{
	OBJ_INFO tObjInfo = *(OBJ_INFO*)(pArg);

	CTransform::TRANSFORM_DESC tTransformDesc;
	tTransformDesc.fSpeed = 0.f;
	tTransformDesc.fDegree = 3.f;

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeTransform, pTagComTransform, (CComponent**)&m_pTransformCom, &tTransformDesc)))
		MSG_FAIL("CChimera::SetUp_Component - m_pTransformCom");
	m_pTransformCom->Set_Matrix(tObjInfo.matWorld);

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeRenderer, pTagComRenderer, (CComponent**)&m_pRendererCom)))
		MSG_FAIL("CChimera::SetUp_Component - m_pRendererCom");

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeVIBufferCube, pTagComVIBuffer, (CComponent**)&m_pVIBufferCom)))
		MSG_FAIL("CChimera::SetUp_Component - m_pVIBufferCom");

	// TombStone
	if (FAILED(Add_Component(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureTombStone"), L"ComTexture", (CComponent**)&m_pTextureCom)))
		MSG_FAIL("CChimera::SetUp_Component - PrototypeTextureTombStone");

	m_pBoxColliderCom = CBoxCollider::Create_Tool(m_pGraphicDevice, tObjInfo.vColliderCenter, tObjInfo.vColliderSize, tObjInfo.dwColliderColor, this);

	return S_OK;
}

HRESULT CTombStone::Set_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CChimera::Set_RenderState - m_pGraphicDevice == nullptr");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE)))
		MSG_FAIL("CChimera::Set_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 150)))
		MSG_FAIL("CChimera::Set_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER)))
		MSG_FAIL("CChimera::Set_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");

	return S_OK;
}

HRESULT CTombStone::Release_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CChimera::Set_RenderState - m_pGraphicDevice == nullptr");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE)))
		MSG_FAIL("CChimera::Set_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");

	return S_OK;
}

void CTombStone::Collision(CGameObject * pColObj)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CAttack* pAttack = dynamic_cast<CAttack*>(pColObj);

	if (pAttack != nullptr && m_eState != STATE_BROKEN && m_bInvincible == false)
	{
		_uint iDmg = pAttack->Get_Dmg();
		//공격이 단일 공격인데 이미 맞은 오브젝트가 잇는 경우
		if (pAttack->Get_SingleAttack() && pAttack->Get_Dead())
			return;

		if (iDmg > 0)
		{
			Sub_Hp(iDmg);

			if (0 >= m_tStat.iHp)
			{
				m_eState = STATE_BROKEN;
				CTombStoneManager::Get_Instance()->Broken();
				m_bStopParticle = true;
			}
		}

		CDmgFont::DESC tDesc;
		tDesc.iDmg = iDmg;
		tDesc.vPos = m_pTransformCom->Get_State(CTransform::STATE_POS) + m_pTransformCom->Get_State(CTransform::STATE_UP);
		tDesc.vScale = _vec3(1.f, 1.f, 1.f);

		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pTagPrototypeDmgFont, pTagLayerDmgFont, &tDesc)))
			MSG_VOID("CChimera pGameInstance->Add_GameObjectToLayer - pTagPrototypeDmgFont");

	}
	RELEASE_INSTANCE(CGameInstance);
}

void CTombStone::Collision_Terrain(_float fTimeDelta, CGameInstance * pInstance, CTransform * pTransform)
{
}

void CTombStone::Particle(_float fTimeDelta)
{
	if (m_bStopParticle == true)
		return;

	m_tParticle.Tick_Time(fTimeDelta);

	if (m_tParticle.Check_Time())
	{
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

		CTombStoneParticles::DESC tDESC;

		tDESC.vScale = _vec3(0.2f, 0.2f, 0.2f);
		tDESC.vStartPos = m_pTransformCom->Get_State(CTransform::STATE_POS);
		tDESC.vStartPos.y -= 1.f;
		tDESC.vTargetPos = m_pTransformCom->Get_State(CTransform::STATE_POS);
		tDESC.vTargetPos.y += 20.f;
		//tDESC.vTargetPos.x += 20.f;
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTombStoneParticles"), TEXT("LayerEffect"), &tDESC)))
			MSG_VOID("CChimera pGameInstance->Add_GameObjectToLayer - pTagPrototypeDmgFont");

		RELEASE_INSTANCE(CGameInstance);

		m_tParticle.fTime = 0.f;
		//m_tParticle.fDelay = 99999999.f;
	}
}

void CTombStone::AI(_float fTimeDelta)
{
	if (STATE_BROKEN == m_eState)
		return;

	m_fTimeSpawn += fTimeDelta;

	if (m_fTimeDelay <= m_fTimeSpawn)
	{
		if(85 <= (rand() % 100))
			Spawn();

		m_fTimeSpawn = 0.f;
	}
}

void CTombStone::Spawn()
{
	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);

	_vec3 vPos, vUp, vLook;
	OBJ_INFO tObj;
	tObj.matWorld = *m_pTransformCom->Get_WorldMatrixPtr();
	vPos = tObj.Get_State(OBJ_INFO::STATE_POS);
	vLook = tObj.Get_State(OBJ_INFO::STATE_LOOK);

	vUp = tObj.Get_State(OBJ_INFO::STATE_UP);
	D3DXVec3Normalize(&vLook, &vLook);
	D3DXVec3Normalize(&vUp, &vUp);
	vPos += vLook * 1.f;
	vPos -= vUp * 1.f;

	tObj.SetUp_State(OBJ_INFO::STATE_POS, vPos);
	tObj.dwColliderColor = 0xffffffff;
	tObj.vColliderCenter = _vec3(0.f, 0.f, 0.f);
	tObj.vColliderSize = _vec3(1.f, 1.f, 1.f);
	tObj.SetUp_Scale(1.f, 1.f, 1.f);

	if (3 > (rand() % 7))
	{
		if (FAILED(pInstance->Add_GameObjectToLayer(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeMeleeSkull"), pTagLayerMonster, &tObj)))
			MSG_VOID("CLevel_Logo Ready_Layer_Backgroud - Add_GameObjectToLayer PrototypeMeleeSkull");
	}
	else
	{
		if (FAILED(pInstance->Add_GameObjectToLayer(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeRangeSkull"), pTagLayerMonster, &tObj)))
			MSG_VOID("CLevel_Logo Ready_Layer_Backgroud - Add_GameObjectToLayer PrototypeRangeSkull");
	}

	RELEASE_INSTANCE(CGameInstance);
}

void CTombStone::Free()
{
	__super::Free();

	Safe_Release(m_pBoxColliderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}

CTombStone * CTombStone::Create(_pDevice pDevice)
{
	CTombStone* pInstance = new CTombStone(pDevice);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CChimera Create - NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTombStone::Clone(void * pArg)
{
	CGameObject* pInstance = new CTombStone(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CChimera Clone - NativeConstruct");
		Safe_Release(pInstance);
	}

	return pInstance;
}

_vec3 CTombStone::Get_MinPos()
{
	_vec3 vMinPos = m_pBoxColliderCom->Get_MinPos();
	return vMinPos;
}

_vec3 CTombStone::Get_MaxPos()
{
	_vec3 vMaxPos = m_pBoxColliderCom->Get_MaxPos();
	return vMaxPos;
}

_vec3 CTombStone::Get_WorldPos()
{
	return m_pBoxColliderCom->Get_ColliderWolrdPos();
}

_vec3 CTombStone::Get_Length()
{
	return m_pTransformCom->Get_Scale();
}


void CTombStone::Notify(void * pMessage)
{
	m_eState = STATE_IDLE;

	m_bInvincible = true;
}

HRESULT CTombStone::Tick_Collider(_float fTimeDelta)
{
	m_pBoxColliderCom->Set_WorldMatrixFromObject(*m_pTransformCom->Get_WorldMatrixPtr());
	m_pBoxColliderCom->Tick(fTimeDelta);

	return S_OK;
}

void CTombStone::Set_PoisonHitTransform(CPoisonCubeHit::DESC * pDesc)
{
	memcpy(&pDesc->tObjInfo.matWorld, m_pTransformCom->Get_WorldMatrixPtr(), sizeof(_mat));
}

const _vec3 & CTombStone::Get_BoxColliderCenter() const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_pBoxColliderCom->Get_ColliderCenter();
}
