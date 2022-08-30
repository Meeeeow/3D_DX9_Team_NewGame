#include "stdafx.h"
#include "..\public\Chimera.h"
#include "Player.h"
#include "PlayerTagMgr.h"
#include "Attack.h"
#include "DmgFont.h"
#include "Resurrection.h"
#include "NecromencerFire.h"
#include "IceOrb.h"
#include "Cloud.h"
#include "ResurrectionManager.h"
#include "TombStoneManager.h"
#include "MonsterColliderManager.h"
#include "DarkOrb.h"
#include "Blink.h"
#include "Portal.h"

CChimera::CChimera(_pDevice pGraphicDevice)
	: CMonster(pGraphicDevice)
{
	ZeroMemory(m_pTextureComArr, sizeof(m_pTextureComArr));
}

CChimera::CChimera(const CChimera & rhs)
	: CMonster(rhs)
{
}

HRESULT CChimera::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CChimera NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CChimera::NativeConstruct(void * pArg)
{
	CMonster::MONSTER_STAT tMonsterStat;
	tMonsterStat.iMaxHp = 100;
	tMonsterStat.iHp = tMonsterStat.iMaxHp;
	tMonsterStat.iDmg = 20;

	if (FAILED(__super::NativeConstruct(&tMonsterStat)))
		MSG_FAIL("CChimera NativeConstruct - __super");

	if (FAILED(SetUp_Component(pArg)))
		MSG_FAIL("CChimera NativeConstruct - SetUp_Component");

	Check_Frame();

	m_tDarkOrb.fDelay = 3.f;

	m_tPattern.fDelay = 5.f;

	CTombStoneManager::Get_Instance()->Repair();

	return S_OK;
}

_int CChimera::Tick(_float fTimeDelta)
{
	if (true == m_bDead)
	{
		Shokan_Watashino_Potaru();
		return OBJ_DEAD;
	}

	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CChimera Tick - __super");

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	AI(fTimeDelta);

	Move(fTimeDelta);

	m_tDarkOrb.Tick_Time(fTimeDelta);

	if (m_tDarkOrb.Check_Time())
	{
		DarkOrb();

		m_tDarkOrb.fTime = 0.f;
	}

	Check_Frame();

	m_tPattern.Tick_Time(fTimeDelta);

	Collision_Terrain(fTimeDelta, pGameInstance, m_pTransformCom);

	BillBoarding(m_pTransformCom);

	m_pBoxColliderCom->Set_WorldMatrixFromObject(*m_pTransformCom->Get_WorldMatrixPtr());
	m_pBoxColliderCom->Tick(fTimeDelta);

	RELEASE_INSTANCE(CGameInstance);

	return 0;
}

_int CChimera::LateTick(_float fTimeDelta)
{
	if (0 > (__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CChimera LateTick - __super");

	if (m_pRendererCom == nullptr)
		MSG_MINUSONE("CChimera::LateTick - m_pRendererCom == nullptr");

	if (m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONALPHA, this))
		MSG_MINUSONE("CChimera::LateTick - Add_RenderGroup");

	return _int();
}

HRESULT CChimera::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("CChimera Render - __super");

	if (m_pTransformCom == nullptr ||
		m_pTextureComArr == nullptr ||
		m_pBoxColliderCom == nullptr ||
		m_pVIBufferCom == nullptr)
		MSG_FAIL("CChimera::Render - Com is null");

	if (FAILED(m_pTransformCom->Bind_OnDevice()))
		MSG_FAIL("CChimera::Render - m_pTransformCom Bind_OnDevice");

	if (FAILED(m_pTextureComArr[m_ePreState]->Bind_OnDevice(m_tFrame.iStart)))
		MSG_FAIL("CChimera::Render - m_pTextureComArr[m_eCurState]->Bind_OnDevice");

	if (FAILED(Set_RenderState()))
		MSG_FAIL("CChimera::Render - Set_RenderState");

	if (FAILED(m_pVIBufferCom->Render()))
		MSG_FAIL("CChimera::Render - m_pVIBufferCom->Render");

	if (FAILED(Release_RenderState()))
		MSG_FAIL("CChimera::Render - Release_RenderState");

	if (FAILED(m_pBoxColliderCom->Render()))
		MSG_FAIL("CChimera::Render - m_pBoxColliderCom->Render");

	return S_OK;
}

_bool CChimera::Picking(const _vec3 & vMouseRay, const _vec3 & vMousePivot, CTransform * pOut)
{
	return false;
}

HRESULT CChimera::SetUp_Component(void * pArg)
{
	OBJ_INFO tObjInfo = *(OBJ_INFO*)(pArg);

	CTransform::TRANSFORM_DESC tTransformDesc;
	tTransformDesc.fSpeed = 20.f;
	tTransformDesc.fDegree = 3.f;

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeTransform, pTagComTransform, (CComponent**)&m_pTransformCom, &tTransformDesc)))
		MSG_FAIL("CChimera::SetUp_Component - m_pTransformCom");
	m_pTransformCom->Set_Matrix(tObjInfo.matWorld);

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeRenderer, pTagComRenderer, (CComponent**)&m_pRendererCom)))
		MSG_FAIL("CChimera::SetUp_Component - m_pRendererCom");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("ChimeraBuffer"), pTagComVIBuffer, (CComponent**)&m_pVIBufferCom)))
		MSG_FAIL("CChimera::SetUp_Component - m_pVIBufferCom");

	// Attack
	if (FAILED(Add_Component(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureChimeraAttack"), L"ComTextureAttack", (CComponent**)&m_pTextureComArr[STATE_ATTACK])))
		MSG_FAIL("CChimera::SetUp_Component - TEXTURE_ATTACK");

	// Cast
	if (FAILED(Add_Component(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureChimeraCast"), L"ComTextureCast", (CComponent**)&m_pTextureComArr[STATE_CAST])))
		MSG_FAIL("CChimera::SetUp_Component - TEXTURE_CAST");

	// Death
	if (FAILED(Add_Component(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureChimeraDeath"), L"ComTextureDeath", (CComponent**)&m_pTextureComArr[STATE_DEATH])))
		MSG_FAIL("CChimera::SetUp_Component - TEXTURE_DEATH");

	// Idle
	if (FAILED(Add_Component(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureChimeraIdle"), L"ComTextureIdle", (CComponent**)&m_pTextureComArr[STATE_IDLE])))
		MSG_FAIL("CChimera::SetUp_Component - TEXTURE_IDLE");

	// Move
	if (FAILED(Add_Component(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureChimeraMove"), L"ComTextureMove", (CComponent**)&m_pTextureComArr[STATE_MOVE])))
		MSG_FAIL("CChimera::SetUp_Component - TEXTURE_MOVE");

	OBJ_INFO* pColDat = CMonsterColliderManager::Get_Instance()->Get_ColliderDat(CMonsterColliderManager::COLLIDER_CHIMERA);

	m_pBoxColliderCom = CBoxCollider::Create_Tool(m_pGraphicDevice, pColDat->vColliderCenter, pColDat->vColliderSize, pColDat->dwColliderColor, this);

	return S_OK;
}

HRESULT CChimera::Set_RenderState()
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

HRESULT CChimera::Release_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CChimera::Set_RenderState - m_pGraphicDevice == nullptr");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE)))
		MSG_FAIL("CChimera::Set_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");

	return S_OK;
}

void CChimera::Shokan_Watashino_Potaru()
{
	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);
	CPortal::DESC tOuterPortal;
	tOuterPortal.eNextLevel = LEVEL::LEVEL_BOSS_CIRCUS;
	tOuterPortal.vPos = _vec3(29.5f, 1.f, 50.5f);
	lstrcpy(tOuterPortal.szBufferName, L"PrototypeVIBufferCubeColorB");
	tOuterPortal.vScale = _vec3(2.f, 2.f, 2.f);
	tOuterPortal.vTrans = _vec3(10.f, 3.f, 3.f);
	if (FAILED(pInstance->Add_GameObjectToLayer(LEVEL_STATIC, pTagPrototypePortal, L"LayerPortal", &tOuterPortal)))
		MSGBOX("CChimera Shokan_Watashino_Potaru - Add_GameObjectToLayer pTagPrototypePortal");

	RELEASE_INSTANCE(CGameInstance);
}

void CChimera::Set_State(STATE eState)
{
	if (m_bStateLock)
		return;

	m_eCurState = eState;
}

void CChimera::Set_Idle()
{
	m_bStateLock = false;
	Set_State(STATE_IDLE);
	Check_Frame();
}

void CChimera::Check_Frame()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case Client::CChimera::STATE_IDLE:
			m_tFrame.Set_Value(m_pTextureComArr[STATE_IDLE]->Get_ImageCount() - 1);
			break;
		case Client::CChimera::STATE_MOVE:
			m_tFrame.Set_Value(m_pTextureComArr[STATE_MOVE]->Get_ImageCount() - 1);
			break;
		case Client::CChimera::STATE_ATTACK:
			m_tFrame.Set_Value(m_pTextureComArr[STATE_ATTACK]->Get_ImageCount() - 1, 0, 0.05f);
			break;
		case Client::CChimera::STATE_CAST:
			m_tFrame.Set_Value(m_pTextureComArr[STATE_CAST]->Get_ImageCount() - 1, 0, 0.05f);
			break;
		case Client::CChimera::STATE_DEATH:
			m_tFrame.Set_Value(m_pTextureComArr[STATE_DEATH]->Get_ImageCount() - 1);
			break;
		}

		m_ePreState = m_eCurState;
	}
}

void CChimera::Tick_Frame(_float fTimeDelta)
{
	m_tFrame.fTime += fTimeDelta;
	if (m_tFrame.fTime >= m_tFrame.fDelay)
	{
		m_tFrame.fTime = 0.f;
		m_tFrame.iStart++;

		if (m_ePreState == STATE_CAST && m_tFrame.iStart == 8)
		{
			if (m_bFire == true)
				Fire();
			else if (m_bBlink == true)
			{
				Blink();
				m_bBlink = false;
			}
			else
			{
				++m_iPatternNum;

				switch (m_iPatternNum % 4)
				{
				case 0:
					Resurrection();
					break;
				case 1:
					Fire();
					break;
				case 2:
					Eletric();
					break;
				case 3:
					Ice();
					break;
				}
			}
		}

		if (m_ePreState == STATE_ATTACK && m_tFrame.iStart == 6)
		{
			Attack();
		}

		if (m_tFrame.iStart >= m_tFrame.iEnd)
		{
  			m_tFrame.iStart = 0;

			switch (m_ePreState)
			{
			case Client::CChimera::STATE_CAST:
				if (true == m_bFire)
				{
					++m_iShotCount;

					if (3 <= m_iShotCount)
					{
						m_iShotCount = 0;
						m_eCurState = STATE::STATE_IDLE;
						m_bStateLock = false;
						m_bFire = false;
					}
					break;
				}

				m_eCurState = STATE::STATE_IDLE;
				m_bStateLock = false;
				break;
			case Client::CChimera::STATE_ATTACK:
				m_eCurState = STATE::STATE_IDLE;
				m_bStateLock = false;
				break;
			case Client::CChimera::STATE_DEATH:
				m_tFrame.iStart = m_pTextureComArr[STATE_DEATH]->Get_ImageCount() - 1;
				m_bDead = true;
				break;
			}
		}
	}
}

void CChimera::Collision(CGameObject * pColObj)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CAttack* pAttack = dynamic_cast<CAttack*>(pColObj);

	if (pAttack != nullptr && m_eCurState != STATE_DEATH)
	{
		_uint iDmg = pAttack->Get_Dmg();
		//공격이 단일 공격인데 이미 맞은 오브젝트가 잇는 경우
		if (pAttack->Get_SingleAttack() && pAttack->Get_Dead())
			return;

		if (iDmg > 0)
		{
			Sub_Hp(iDmg);
		}

		CDmgFont::DESC tDesc;
		tDesc.iDmg = iDmg;
		tDesc.vPos = m_pTransformCom->Get_State(CTransform::STATE_POS) + m_pTransformCom->Get_State(CTransform::STATE_UP) * 0.5f;
		tDesc.vScale = m_pTransformCom->Get_Scale();

		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pTagPrototypeDmgFont, pTagLayerDmgFont, &tDesc)))
			MSG_VOID("CChimera pGameInstance->Add_GameObjectToLayer - pTagPrototypeDmgFont");

	}
	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CChimera::Sub_Hp(_uint iDmg)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(__super::Sub_Hp(iDmg)))
		MSG_FAIL("CChimera::Sub_Hp - __super::Sub_Hp");

	CDmgFont::DESC tDesc;
	tDesc.iDmg = iDmg;
	tDesc.vPos = m_pTransformCom->Get_State(CTransform::STATE_POS) + m_pTransformCom->Get_State(CTransform::STATE_UP) * 0.5f;
	tDesc.vScale = m_pTransformCom->Get_Scale();

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pTagPrototypeDmgFont, pTagLayerDmgFont, &tDesc)))
		MSG_FAIL("CChimera pGameInstance->Add_GameObjectToLayer - pTagPrototypeDmgFont");

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CChimera::AI(_float fTimeDelta)
{
	m_pPlayerTransformCom = CPlayerTagMgr::Get_Instance()->Get_CurPlayerTransform();
	if (m_pPlayerTransformCom == nullptr)
		MSG_VOID("CNecromencer::AI - m_pTransformComPlayer == nullptr");

	if (m_bStateLock == false)
	{
		_float fDistance = m_pTransformCom->Get_DistanceTarget(m_pPlayerTransformCom);

		if (fDistance >= 30.f)
		{
			Set_State(STATE::STATE_CAST);
			m_bBlink = true;
		}
		else if (fDistance <= 1.f)
		{
			Set_State(STATE::STATE_ATTACK);
			m_bStateLock = true;
		}
		else
		{			
			Set_State(STATE::STATE_MOVE);
		}

		if (m_tPattern.Check_Time())
		{
			Set_State(STATE::STATE_CAST);
			m_bStateLock = true;

			m_tPattern.fTime = 0.f;
		}

		if (m_bFire == true)
		{
			Set_State(STATE::STATE_CAST);
			m_bStateLock = true;
		}

	}

	if (Check_Dead() && m_ePreState != STATE::STATE_DEATH)
	{
		m_bStateLock = false;
		Set_State(STATE::STATE_DEATH);
		m_bStateLock = true;
	}
}

void CChimera::Move(_float fTimeDelta)
{
	if (m_pPlayerTransformCom == nullptr ||
		m_pTransformCom == nullptr)
		MSG_VOID("CChimera::Move - null");

	if (m_eCurState != STATE::STATE_MOVE)
		return;

	m_pTransformCom->Chase_Target(m_pPlayerTransformCom, fTimeDelta * 8.f, 0.5f);
}

void CChimera::Fire()
{
	m_bFire = true;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CNecromencerFire::DESC pDesc{};

	OBJ_INFO pObj{};
	_vec3 vPos;
	vPos = m_pTransformCom->Get_State(CTransform::STATE_POS);
	vPos.y += 1.f;
	pObj.SetUp_Scale(0.7f, 0.7f, 0.7f);
	pObj.SetUp_State(OBJ_INFO::STATE_POS, vPos);

	vPos = m_pPlayerTransformCom->Get_State(CTransform::STATE_POS);
	vPos.y += 0.5f;
	pDesc.vTargetPos = vPos;
	pDesc.pObj = &pObj;

	pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, TEXT("PrototypeNecromencerFire"), pTagLayerMonsterAttack, &pDesc);

	RELEASE_INSTANCE(CGameInstance);
}

void CChimera::Ice()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_mat matWorld = *m_pTransformCom->Get_WorldMatrixPtr();

	for (int i = 0; i < 255; ++i)
	{
		OBJ_INFO	tObj;
		_vec3		vPos, vLook;

		tObj.matWorld = matWorld;
		memcpy(&tObj.matWorld.m[3][0], &_vec3(29.75f, 0.f, 29.75f), sizeof(_vec3));
		tObj.SetUp_Rotation(_vec3(0.f, 1.f, 0.f), _float(rand() % 360));
		vPos = tObj.Get_State(OBJ_INFO::STATE_POS);
		vPos.y += 5.f;
		vLook = tObj.Get_State(OBJ_INFO::STATE_LOOK);
		vPos += *D3DXVec3Normalize(&vLook, &vLook) * FLOAT(rand() % 30);
		tObj.SetUp_State(OBJ_INFO::STATE_POS, vPos);

		pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, TEXT("PrototypeIceOrb"), pTagLayerMonster, &tObj);
	}

	RELEASE_INSTANCE(CGameInstance);
}

void CChimera::Eletric()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, TEXT("PrototypeCloud"), pTagLayerMonster, &OBJ_INFO{});

	RELEASE_INSTANCE(CGameInstance);
}

void CChimera::Resurrection()
{
	CResurrectionManager::Get_Instance()->Resurrection();

	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);

	CResurrection::DESC tDesc;
	OBJ_INFO pObj;
	_vec3 vPos;
	pObj.matWorld = *m_pTransformCom->Get_WorldMatrixPtr();
	vPos = pObj.Get_State(OBJ_INFO::STATE_POS);
	vPos.y += 2.f;
	tDesc.vPos = vPos;
	tDesc.vScale = _vec3(2.f, 2.f, 2.f);

	if (FAILED(pInstance->Add_GameObjectToLayer(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeResurrection"), TEXT("LayerEffect"), &tDesc)))
		MSG_VOID("CChimera Shield - Add_GameObjectToLayer PrototypeResurrection");

	RELEASE_INSTANCE(CGameInstance);
}

void CChimera::DarkOrb()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CDarkOrb::DESC pDesc;

	OBJ_INFO tObj, tObj2;
	_vec3		vPos, vLook;
	_mat matWorld = *m_pTransformCom->Get_WorldMatrixPtr();

	for (int i = 0; i < 6; ++i)
	{
		tObj.matWorld = matWorld;
		tObj.SetUp_Rotation(_vec3(0.f, 1.f, 0.f), _float(rand() % 360));
		vPos = tObj.Get_State(OBJ_INFO::STATE_POS);
		vPos.y += 5.f;
		vLook = tObj.Get_State(OBJ_INFO::STATE_LOOK);
		vPos += *D3DXVec3Normalize(&vLook, &vLook) * _float(rand() % 3);
		tObj.SetUp_State(OBJ_INFO::STATE_POS, vPos);

		// 목적지 설정
		pDesc.vTargetPos = tObj.Get_State(OBJ_INFO::STATE_POS);

		tObj2.SetUp_Scale(0.6f, 0.6f, 0.6f);
		tObj2.SetUp_State(OBJ_INFO::STATE_POS, m_pTransformCom->Get_State(CTransform::STATE_POS));

		pDesc.pObj = &tObj2;

		pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, TEXT("PrototypeDarkOrb"), pTagLayerMonsterAttack, &pDesc);
	}

	RELEASE_INSTANCE(CGameInstance);
}

void CChimera::Attack()
{
	if (m_ePreState != STATE::STATE_ATTACK)
		return;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CAttack::DESC tDesc;
	tDesc.bDestroy = false;
	tDesc.iDmg = 1;//m_tStat.iDmg;
	tDesc.vPos = m_pTransformCom->Get_State(CTransform::STATE_POS);
	tDesc.vScale = _vec3(4.f, 2.f, 4.f);
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pTagPrototypeAttack, pTagLayerMonsterAttack, &tDesc)))
		MSG_VOID("CPlayer::Tick_FramingWarriorSkill1 - pGameInstance->Add_GameObjectToLayer pTagPrototypeAttack");

	RELEASE_INSTANCE(CGameInstance);
}

void CChimera::Blink()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CBlink::DESC pDesc{};

	OBJ_INFO tObj{};
	_vec3 vPos, vLook;
	vPos = m_pTransformCom->Get_State(CTransform::STATE_POS);
	vPos.y += 1.f;
	tObj.SetUp_Scale(2.f, 2.f, 2.f);
	tObj.SetUp_State(OBJ_INFO::STATE_POS, vPos);

	pDesc.vPos = vPos;

	pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, TEXT("PrototypeBlink"), pTagLayerNonCollision, &pDesc);

	_mat matWorld;
	matWorld = *m_pPlayerTransformCom->Get_WorldMatrixPtr();

	tObj.matWorld = matWorld;
	tObj.SetUp_Rotation(_vec3(0.f, 1.f, 0.f), _float(rand() % 360));
	vPos = tObj.Get_State(OBJ_INFO::STATE_POS);
	vLook = tObj.Get_State(OBJ_INFO::STATE_LOOK);
	vPos += *D3DXVec3Normalize(&vLook, &vLook) * 5.f;
	tObj.SetUp_State(OBJ_INFO::STATE_POS, vPos);

	m_pTransformCom->Set_Matrix(tObj.matWorld);

	RELEASE_INSTANCE(CGameInstance);
}

void CChimera::Free()
{
	__super::Free();

	Safe_Release(m_pBoxColliderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);

	for (auto& pTexture : m_pTextureComArr)
		Safe_Release(pTexture);
}

CChimera * CChimera::Create(_pDevice pDevice)
{
	CChimera* pInstance = new CChimera(pDevice);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CChimera Create - NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CChimera::Clone(void * pArg)
{
	CGameObject* pInstance = new CChimera(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CChimera Clone - NativeConstruct");
		Safe_Release(pInstance);
	}

	return pInstance;
}

_vec3 CChimera::Get_MinPos()
{
	_vec3 vMinPos = m_pBoxColliderCom->Get_MinPos();
	return vMinPos;
}

_vec3 CChimera::Get_MaxPos()
{
	_vec3 vMaxPos = m_pBoxColliderCom->Get_MaxPos();
	return vMaxPos;
}

_vec3 CChimera::Get_WorldPos()
{
	return m_pBoxColliderCom->Get_ColliderWolrdPos();
}

_vec3 CChimera::Get_Length()
{
	return m_pTransformCom->Get_Scale();
}

void CChimera::Notify(void * pMessage)
{

}

HRESULT CChimera::Tick_Collider(_float fTimeDelta)
{
	m_pBoxColliderCom->Set_WorldMatrixFromObject(*m_pTransformCom->Get_WorldMatrixPtr());
	m_pBoxColliderCom->Tick(fTimeDelta);

	return S_OK;
}

void CChimera::Set_PoisonHitTransform(CPoisonCubeHit::DESC * pDesc)
{
	memcpy(&pDesc->tObjInfo.matWorld, m_pTransformCom->Get_WorldMatrixPtr(), sizeof(_mat));
}

const _vec3 & CChimera::Get_BoxColliderCenter() const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_pBoxColliderCom->Get_ColliderCenter();
}
