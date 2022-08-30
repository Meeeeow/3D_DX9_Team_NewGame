#include "stdafx.h"
#include "..\public\Necromencer.h"
#include "Player.h"
#include "PlayerTagMgr.h"
#include "Attack.h"
#include "DmgFont.h"
#include "Resurrection.h"
#include "NecromencerFire.h"
#include "IceOrb.h"
#include "Cloud.h"
#include "Shield.h"
#include "Blink.h"
#include "ResurrectionManager.h"
#include "MonsterColliderManager.h"


CNecromencer::CNecromencer(_pDevice pGraphicDevice)
	: CMonster(pGraphicDevice)
{
	ZeroMemory(m_pTextureComArr, sizeof(m_pTextureComArr));
}

CNecromencer::CNecromencer(const CNecromencer & rhs)
	: CMonster(rhs)
{ 
}

HRESULT CNecromencer::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CNecromencer NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CNecromencer::NativeConstruct(void * pArg)
{
	CMonster::MONSTER_STAT tMonsterStat;
	tMonsterStat.iMaxHp = 100;
	tMonsterStat.iHp = tMonsterStat.iMaxHp;
	tMonsterStat.iDmg = 20;

	if (FAILED(__super::NativeConstruct(&tMonsterStat)))
		MSG_FAIL("CNecromencer NativeConstruct - __super");

	if (FAILED(SetUp_Component(pArg)))
		MSG_FAIL("CNecromencer NativeConstruct - SetUp_Component");

	Check_Frame();

	Shield();

	m_tPattern.fDelay = 5.f;

	return S_OK;
}

_int CNecromencer::Tick(_float fTimeDelta)
{
	if (true == m_bDead)
		return OBJ_DEAD;

	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CNecromencer Tick - __super");

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_tPattern.Tick_Time(fTimeDelta);

	AI();

	Move(fTimeDelta);
	//Chimera();

	Check_Frame();

	Collision_Terrain(fTimeDelta, pGameInstance, m_pTransformCom);

	BillBoarding(m_pTransformCom);

	m_pBoxColliderCom->Set_WorldMatrixFromObject(*m_pTransformCom->Get_WorldMatrixPtr());
	m_pBoxColliderCom->Tick(fTimeDelta);

	RELEASE_INSTANCE(CGameInstance);

	return 0;
}

_int CNecromencer::LateTick(_float fTimeDelta)
{
	if (0 > (__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CNecromencer LateTick - __super");

	if (m_pRendererCom == nullptr)
		MSG_MINUSONE("CNecromencer::LateTick - m_pRendererCom == nullptr");

	if (m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONALPHA, this))
		MSG_MINUSONE("CNecromencer::LateTick - Add_RenderGroup");

	return _int();
}

HRESULT CNecromencer::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("CNecromencer Render - __super");

	if (m_pTransformCom == nullptr ||
		m_pTextureComArr == nullptr ||
		m_pBoxColliderCom == nullptr ||
		m_pVIBufferCom == nullptr)
		MSG_FAIL("CNecromencer::Render - Com is null");

	if (FAILED(m_pTransformCom->Bind_OnDevice()))
		MSG_FAIL("CNecromencer::Render - m_pTransformCom Bind_OnDevice");

	if (FAILED(m_pTextureComArr[m_ePreState]->Bind_OnDevice(m_tFrame.iStart)))
		MSG_FAIL("CNecromencer::Render - m_pTextureComArr[m_eCurState]->Bind_OnDevice");

	if (FAILED(Set_RenderState()))
		MSG_FAIL("CNecromencer::Render - Set_RenderState");

	if (FAILED(m_pVIBufferCom->Render()))
		MSG_FAIL("CNecromencer::Render - m_pVIBufferCom->Render");

	if (FAILED(Release_RenderState()))
		MSG_FAIL("CNecromencer::Render - Release_RenderState");

	if (FAILED(m_pBoxColliderCom->Render()))
		MSG_FAIL("CNecromencer::Render - m_pBoxColliderCom->Render");

	return S_OK;
}

_bool CNecromencer::Picking(const _vec3 & vMouseRay, const _vec3 & vMousePivot, CTransform * pOut)
{
	return false;
}

HRESULT CNecromencer::SetUp_Component(void * pArg)
{
	OBJ_INFO tObjInfo = *(OBJ_INFO*)(pArg);

	CTransform::TRANSFORM_DESC tTransformDesc;
	tTransformDesc.fSpeed = 10.f;
	tTransformDesc.fDegree = 3.f;

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeTransform, pTagComTransform, (CComponent**)&m_pTransformCom, &tTransformDesc)))
		MSG_FAIL("CNecromencer::SetUp_Component - m_pTransformCom");
	m_pTransformCom->Set_Matrix(tObjInfo.matWorld);

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeRenderer, pTagComRenderer, (CComponent**)&m_pRendererCom)))
		MSG_FAIL("CNecromencer::SetUp_Component - m_pRendererCom");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("NecromencerBuffer"), pTagComVIBuffer, (CComponent**)&m_pVIBufferCom)))
		MSG_FAIL("CNecromencer::SetUp_Component - m_pVIBufferCom");

	// Cast
	if (FAILED(Add_Component(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureNecromencerCast"), L"ComTextureCast", (CComponent**)&m_pTextureComArr[STATE_CAST])))
		MSG_FAIL("CNecromencer::SetUp_Component - TEXTURE_ATTACK1");

	// Death
	if (FAILED(Add_Component(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureNecromencerDeath"), L"ComTextureDeath", (CComponent**)&m_pTextureComArr[STATE_DEATH])))
		MSG_FAIL("CNecromencer::SetUp_Component - TEXTURE_DEATH");

	// Die
	if (FAILED(Add_Component(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureNecromencerDie"), L"ComTextureDie", (CComponent**)&m_pTextureComArr[STATE_DIE])))
		MSG_FAIL("CNecromencer::SetUp_Component - TEXTURE_DIE");

	// Hit
	if (FAILED(Add_Component(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureNecromencerHit"), L"ComTextureHit", (CComponent**)&m_pTextureComArr[STATE_HIT])))
		MSG_FAIL("CNecromencer::SetUp_Component - TEXTURE_HIT");

	// Idle
	if (FAILED(Add_Component(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureNecromencerIdle"), L"ComTextureIdle", (CComponent**)&m_pTextureComArr[STATE_IDLE])))
		MSG_FAIL("CNecromencer::SetUp_Component - TEXTURE_IDLE");

	// Move
	if (FAILED(Add_Component(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureNecromencerMove"), L"ComTextureMove", (CComponent**)&m_pTextureComArr[STATE_MOVE])))
		MSG_FAIL("CNecromencer::SetUp_Component - TEXTURE_MOVE");

	OBJ_INFO* pColDat = CMonsterColliderManager::Get_Instance()->Get_ColliderDat(CMonsterColliderManager::COLLIDER_NECROMENCER);

	m_pBoxColliderCom = CBoxCollider::Create_Tool(m_pGraphicDevice, pColDat->vColliderCenter, pColDat->vColliderSize, pColDat->dwColliderColor, this);

	return S_OK;
}

HRESULT CNecromencer::Set_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CNecromencer::Set_RenderState - m_pGraphicDevice == nullptr");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE)))
		MSG_FAIL("CNecromencer::Set_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 150)))
		MSG_FAIL("CNecromencer::Set_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER)))
		MSG_FAIL("CNecromencer::Set_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");

	return S_OK;
}

HRESULT CNecromencer::Release_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CNecromencer::Set_RenderState - m_pGraphicDevice == nullptr");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE)))
		MSG_FAIL("CNecromencer::Set_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");

	return S_OK;
}

void CNecromencer::Set_State(STATE eState)
{
	if (m_bStateLock)
		return;

	m_eCurState = eState;
}

void CNecromencer::Set_Idle()
{
	m_bStateLock = false;
	Set_State(STATE_IDLE);
	Check_Frame();
}

void CNecromencer::Check_Frame()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case Client::CNecromencer::STATE_IDLE:
			m_tFrame.Set_Value(m_pTextureComArr[STATE_IDLE]->Get_ImageCount() - 1);
			break;
		case Client::CNecromencer::STATE_MOVE:
			m_tFrame.Set_Value(m_pTextureComArr[STATE_MOVE]->Get_ImageCount() - 1);
			break;
		case Client::CNecromencer::STATE_CAST:
			m_tFrame.Set_Value(m_pTextureComArr[STATE_CAST]->Get_ImageCount() - 1);
			break;
		case Client::CNecromencer::STATE_HIT:
			m_tFrame.Set_Value(m_pTextureComArr[STATE_HIT]->Get_ImageCount() - 1);
			break;
		case Client::CNecromencer::STATE_DEATH:
			m_tFrame.Set_Value(m_pTextureComArr[STATE_DEATH]->Get_ImageCount() - 1);
			break;
		case Client::CNecromencer::STATE_DIE:
			m_tFrame.Set_Value(m_pTextureComArr[STATE_DIE]->Get_ImageCount() - 1);
			break;
		}

		m_ePreState = m_eCurState;
	}
}

void CNecromencer::Tick_Frame(_float fTimeDelta)
{
	m_tFrame.fTime += fTimeDelta;
	if (m_tFrame.fTime >= m_tFrame.fDelay)
	{
		m_tFrame.fTime = 0.f;
		m_tFrame.iStart++;

		if (m_ePreState == STATE_CAST && m_tFrame.iStart == 4)
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

		if (m_tFrame.iStart >= m_tFrame.iEnd)
		{
			m_tFrame.iStart = 0;

			switch (m_ePreState)
			{
			case Client::CNecromencer::STATE_CAST:
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
			case Client::CNecromencer::STATE_HIT:
				m_eCurState = STATE::STATE_IDLE;
				m_bStateLock = false;
				break;
			case Client::CNecromencer::STATE_DEATH:
				m_eCurState = STATE::STATE_DIE;
				Chimera();
				break;
			}
		}
	}
}

void CNecromencer::Collision(CGameObject * pColObj)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CAttack* pAttack = dynamic_cast<CAttack*>(pColObj);

	if (pAttack != nullptr && m_eCurState != STATE_DIE && m_eCurState != STATE_DEATH && m_bShield == false)
	{
		_uint iDmg = pAttack->Get_Dmg();
		//공격이 단일 공격인데 이미 맞은 오브젝트가 잇는 경우
		if (pAttack->Get_SingleAttack() && pAttack->Get_Dead())
			return;

		if (iDmg > 0)
		{
			Sub_Hp(iDmg);
		}

		//공격 당한 모션
		m_bStateLock = false;
		Set_State(STATE_HIT);
		m_tTimeHit.fTime = 0.f;
		m_tTimeHit.fDelay = 0.5f;
		m_tFrame.iStart = 0;
		m_bStateLock = true;

		pAttack->Set_Dead();

	}
	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CNecromencer::Sub_Hp(_uint iDmg)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(__super::Sub_Hp(iDmg)))
		MSG_FAIL("CRangeSkull::Sub_Hp - __super::Sub_Hp");

	CDmgFont::DESC tDesc;
	tDesc.iDmg = iDmg;
	tDesc.vPos = m_pTransformCom->Get_State(CTransform::STATE_POS) + m_pTransformCom->Get_State(CTransform::STATE_UP);
	tDesc.vScale = m_pTransformCom->Get_Scale();

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pTagPrototypeDmgFont, pTagLayerDmgFont, &tDesc)))
		MSG_FAIL("CNecromencer pGameInstance->Add_GameObjectToLayer - pTagPrototypeDmgFont");

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CNecromencer::AI()
{

	m_pPlayerTransformCom = CPlayerTagMgr::Get_Instance()->Get_CurPlayerTransform();
	if (m_pPlayerTransformCom == nullptr)
		MSG_VOID("CNecromencer::AI - m_pTransformComPlayer == nullptr");

	if (m_bStateLock == false && m_bShield == false)
	{
		_float fDistance = m_pTransformCom->Get_DistanceTarget(m_pPlayerTransformCom);

		if (fDistance >= 30.f)
		{
			Set_State(STATE::STATE_CAST);
			m_bBlink = true;
		}
		else if (fDistance >= 15.f)
		{
			Set_State(STATE::STATE_MOVE);
		}
		else
		{
			Set_State(STATE::STATE_IDLE);
		}

		if (m_eCurState == STATE::STATE_IDLE)
		{
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
	}

	if (Check_Dead() && m_ePreState != STATE::STATE_DIE && m_ePreState != STATE::STATE_DEATH && m_bShield == false)
	{
		m_bStateLock = false;
		Set_State(STATE::STATE_DEATH);
		m_bStateLock = true;
	}
}

void CNecromencer::Move(_float fTimeDelta)
{
	if (m_pPlayerTransformCom == nullptr ||
		m_pTransformCom == nullptr)
		MSG_VOID("CNecromencer::Move - null");

	if (m_eCurState != STATE::STATE_MOVE)
		return;

	m_pTransformCom->Chase_Target(m_pPlayerTransformCom, fTimeDelta, 0.5f);
}

void CNecromencer::Fire()
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

void CNecromencer::Ice()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_mat matWorld = *m_pPlayerTransformCom->Get_WorldMatrixPtr();

	OBJ_INFO	tObj;
	_vec3		vPos, vLook;

	for (int i = 0; i < 32; ++i)
	{	
		tObj.matWorld = matWorld;
		tObj.SetUp_Rotation(_vec3(0.f, 1.f, 0.f), _float(rand() % 360));
		vPos = tObj.Get_State(OBJ_INFO::STATE_POS);
		vPos.y += 5.f;
		vLook = tObj.Get_State(OBJ_INFO::STATE_LOOK);
		vPos += *D3DXVec3Normalize(&vLook, &vLook) * _float(rand() % 16);
		tObj.SetUp_State(OBJ_INFO::STATE_POS, vPos);

		pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, TEXT("PrototypeIceOrb"), pTagLayerMonster, &tObj);
	}

	RELEASE_INSTANCE(CGameInstance);
}

void CNecromencer::Eletric()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, TEXT("PrototypeCloud"), pTagLayerMonster, &OBJ_INFO{});

	RELEASE_INSTANCE(CGameInstance);
}

void CNecromencer::Resurrection()
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
		MSG_VOID("CNecromencer Shield - Add_GameObjectToLayer PrototypeResurrection");

	RELEASE_INSTANCE(CGameInstance);
}

void CNecromencer::Shield()
{
	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);

	m_bShield = true;

	// 쉴드
	CShield* pShield;
	OBJ_INFO pObj;
	_vec3 vPos;
	_vec3 vScale = m_pTransformCom->Get_Scale();
	pObj.SetUp_Scale(vScale.x * 5.f, vScale.y * 5.f, vScale.z * 5.f);
	vPos = m_pTransformCom->Get_State(CTransform::STATE_POS);
	vPos.y += 2.f;
	pObj.SetUp_State(OBJ_INFO::STATE_POS, vPos);
	if (FAILED(pInstance->Add_GameObjectToLayer(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeShield"), TEXT("LayerEffect"), &pObj, (void**)&pShield)))
		MSG_VOID("CNecromencer Shield - Add_GameObjectToLayer PrototypeShield");

	pShield->Add_Master(this);

	RELEASE_INSTANCE(CGameInstance);
}

void CNecromencer::Chimera()
{
	OBJ_INFO	tObj;
	_vec3		vPos;
	vPos = m_pTransformCom->Get_State(CTransform::STATE_POS);
	vPos.y += 1.f;
	tObj.matWorld = *m_pTransformCom->Get_WorldMatrixPtr();
	memcpy(&tObj.matWorld.m[3][0], &vPos, sizeof(_vec3));

	CResurrectionManager::Get_Instance()->Chimera(&tObj);

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, TEXT("PrototypeBlackHole"), pTagLayerNonCollision, &tObj);

	RELEASE_INSTANCE(CGameInstance);

	m_bDead = true; 
}

void CNecromencer::Blink()
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

void CNecromencer::Hit(_float fTimeDelta)
{
	if (m_eCurState != STATE_HIT)
		return;

	m_tTimeHit.Tick_Time(fTimeDelta);
	if (m_tTimeHit.Check_Time())
	{
		Set_Idle();
		m_tTimeHit.fTime = 0.f;
	}
}

void CNecromencer::Free()
{
	__super::Free();

	Safe_Release(m_pBoxColliderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);

	for (auto& pTexture : m_pTextureComArr)
		Safe_Release(pTexture);
}

CNecromencer * CNecromencer::Create(_pDevice pDevice)
{
	CNecromencer* pInstance = new CNecromencer(pDevice);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CNecromencer Create - NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CNecromencer::Clone(void * pArg)
{
	CGameObject* pInstance = new CNecromencer(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CNecromencer Create - NativeConstruct");
		Safe_Release(pInstance);
	}

	return pInstance;
}

_vec3 CNecromencer::Get_MinPos()
{
	_vec3 vMinPos = m_pBoxColliderCom->Get_MinPos();
	return vMinPos;
}

_vec3 CNecromencer::Get_MaxPos()
{
	_vec3 vMaxPos = m_pBoxColliderCom->Get_MaxPos();
	return vMaxPos;
}

_vec3 CNecromencer::Get_WorldPos()
{
	return m_pBoxColliderCom->Get_ColliderWolrdPos();
}

Engine::_vec3 CNecromencer::Get_Length()
{
	return m_pTransformCom->Get_Scale();
}

void CNecromencer::Notify(void * pMessage)
{

}

HRESULT CNecromencer::Tick_Collider(_float fTimeDelta)
{
	m_pBoxColliderCom->Set_WorldMatrixFromObject(*m_pTransformCom->Get_WorldMatrixPtr());
	m_pBoxColliderCom->Tick(fTimeDelta);

	return S_OK;
}

void CNecromencer::Set_PoisonHitTransform(CPoisonCubeHit::DESC * pDesc)
{
	memcpy(&pDesc->tObjInfo.matWorld, m_pTransformCom->Get_WorldMatrixPtr(), sizeof(_mat));
}

const _vec3 & CNecromencer::Get_BoxColliderCenter() const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_pBoxColliderCom->Get_ColliderCenter();
}
