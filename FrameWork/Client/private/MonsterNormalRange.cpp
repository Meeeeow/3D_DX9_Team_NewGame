#include "stdafx.h"
#include "..\public\MonsterNormalRange.h"
#include "Player.h"
#include "PlayerTagMgr.h"
#include "MagicMissile.h"
#include "Attack.h"
#include "DmgFont.h"

CMonsterNormalRange::CMonsterNormalRange(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CMonster(pGraphicDevice)
{
	ZeroMemory(m_pTextureArr, sizeof(m_pTextureArr));
}

CMonsterNormalRange::CMonsterNormalRange(const CMonsterNormalRange & rhs)
	:CMonster(rhs)
{
}

HRESULT CMonsterNormalRange::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CMonsterNormalRange NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CMonsterNormalRange::NativeConstruct(void * pArg)
{
	CMonster::MONSTER_STAT tStat;
	tStat.iDmg = 10;
	tStat.iHp = 15;
	tStat.iMaxHp = 15;
	if (FAILED(__super::NativeConstruct(&tStat)))
		MSG_FAIL("CMonsterNormalRange NativeConstruct - __super");

	if (FAILED(SetUp_Component(pArg)))
		MSG_FAIL("CMonsterNormalRange NativeConstruct - SetUp_Component");

	Check_Frame();
	return S_OK;
}

_int CMonsterNormalRange::Tick(_float fTimeDelta)
{
	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CMonsterNormalRange Tick - __super");

	if (m_bDead)
		return OBJ_DEAD;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	AI();
	Check_Frame();
	Move(fTimeDelta);
	Hit(fTimeDelta);
	//지형 타기
	Collision_Terrain(fTimeDelta, pGameInstance, m_pTransform);
	//빌보딩
	BillBoarding(m_pTransform);
	m_pBoxCollider->Set_WorldMatrixFromObject(*m_pTransform->Get_WorldMatrixPtr());
	m_pBoxCollider->Tick(fTimeDelta);
	RELEASE_INSTANCE(CGameInstance);
	return 0;
}

_int CMonsterNormalRange::LateTick(_float fTimeDelta)
{
	if (0 > (__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CMonsterNormalRange LateTick - __super");

	if (m_pRenderer == nullptr)
		MSG_MINUSONE("CMonsterNormalRange::LateTick - m_pRenderer == nullptr");

	if (m_pRenderer->Add_RenderGroup(CRenderer::GROUP_NONALPHA, this))
		MSG_MINUSONE("CMonsterNormalRange::LateTick - Add_RenderGroup");

	return _int();
}

HRESULT CMonsterNormalRange::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("CMonsterNormalRange Render - __super");

	if (m_pTransform == nullptr ||
		m_pTextureArr == nullptr ||
		m_pBoxCollider == nullptr ||
		m_pVIBuffer == nullptr)
		MSG_FAIL("CMonsterNormalRange::Render - Com is null");

	if (FAILED(m_pTransform->Bind_OnDevice()))
		MSG_FAIL("CMonsterNormalRange::Render - m_pTransform Bind_OnDevice");

	if (FAILED(m_pTextureArr[m_eCurState]->Bind_OnDevice(Get_FrameIndex())))
		MSG_FAIL("CMonsterNormalRange::Render - m_pTexture[m_eCurState]->Bind_OnDevice");

	if (FAILED(Set_RenderState()))
		MSG_FAIL("CMonsterNormalRange::Render - Set_RenderState");

	if (FAILED(m_pVIBuffer->Render()))
		MSG_FAIL("CMonsterNormalRange::Render - m_pVIBuffer->Render");

	if (FAILED(Release_RenderState()))
		MSG_FAIL("CMonsterNormalRange::Render - Release_RenderState");

	if (FAILED(m_pBoxCollider->Render()))
		MSG_FAIL("CMonsterNormalRange::Render - m_pBoxCollider->Render");

	return S_OK;
}

void CMonsterNormalRange::Set_State(MONSTER_FSM eState)
{
	if (m_bStateLock)
		return;

	m_eCurState = eState;
}

void CMonsterNormalRange::Set_Idle()
{
	m_bStateLock = false;
	Set_State(FSM_IDLE);
}

HRESULT CMonsterNormalRange::Sub_Hp(_uint iDmg)
{
	__super::Sub_Hp(iDmg);

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CDmgFont::DESC tDesc;
	tDesc.iDmg = iDmg;
	tDesc.vPos = m_pTransform->Get_State(CTransform::STATE_POS) + m_pTransform->Get_State(CTransform::STATE_UP);
	tDesc.vScale = m_pTransform->Get_Scale();
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pTagPrototypeDmgFont, pTagLayerDmgFont, &tDesc)))
		MSG_FAIL("pGameInstance->Add_GameObjectToLayer - pTagPrototypeDmgFont");

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMonsterNormalRange::SetUp_Component(void *pArg)
{
	OBJ_INFO tObjInfo = *(OBJ_INFO*)(pArg);
	CTransform::TRANSFORM_DESC tTransformDesc;
	tTransformDesc.fSpeed = 10.f;
	tTransformDesc.fDegree = 3.f;
	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeTransform, pTagComTransform, (CComponent**)&m_pTransform, &tTransformDesc)))
		MSG_FAIL("CMonsterNormalRange::SetUp_Component - m_pTransform");
	m_pTransform->Set_Matrix(tObjInfo.matWorld);

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeRenderer, pTagComRenderer, (CComponent**)&m_pRenderer)))
		MSG_FAIL("CMonsterNormalRange::SetUp_Component - m_pRenderer");

	//if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeVIBufferRect, pTagComVIBuffer, (CComponent**)&m_pVIBuffer)))
	//	MSG_FAIL("CMonsterNormalRange::SetUp_Component - m_pVIBuffer");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PlayerBuffer"), pTagComVIBuffer, (CComponent**)&m_pVIBuffer)))
		MSG_FAIL("CPlayer SetUp_Component - Add_Component m_pVIBufferCom");

	wstring wstrTextureName;

	wstrTextureName = tObjInfo.szTextureName;
	wstrTextureName += L"Idle";
	if (FAILED(Add_Component(LEVEL_STATIC, wstrTextureName.c_str(), L"ComTextureIdle", (CComponent**)&m_pTextureArr[FSM_IDLE])))
		MSG_FAIL("CMonsterNormalRange::SetUp_Component - FSM_IDLE");

	wstrTextureName = tObjInfo.szTextureName;
	wstrTextureName += L"Attack";
	if (FAILED(Add_Component(LEVEL_STATIC, wstrTextureName.c_str(), L"ComTextureAttack", (CComponent**)&m_pTextureArr[FSM_ATTACK])))
		MSG_FAIL("CMonsterNormalRange::SetUp_Component - FSM_ATTACK");

	wstrTextureName = tObjInfo.szTextureName;
	wstrTextureName += L"Die";
	if (FAILED(Add_Component(LEVEL_STATIC, wstrTextureName.c_str(), L"ComTextureDie", (CComponent**)&m_pTextureArr[FSM_DIE])))
		MSG_FAIL("CMonsterNormalRange::SetUp_Component - FSM_DIE");

	wstrTextureName = tObjInfo.szTextureName;
	wstrTextureName += L"Hit";
	if (FAILED(Add_Component(LEVEL_STATIC, wstrTextureName.c_str(), L"ComTextureHit", (CComponent**)&m_pTextureArr[FSM_HIT])))
		MSG_FAIL("CMonsterNormalRange::SetUp_Component - FSM_HIT");

	wstrTextureName = tObjInfo.szTextureName;
	wstrTextureName += L"Move";
	if (FAILED(Add_Component(LEVEL_STATIC, wstrTextureName.c_str(), L"ComTextureMove", (CComponent**)&m_pTextureArr[FSM_MOVE])))
		MSG_FAIL("CMonsterNormalRange::SetUp_Component - FSM_MOVE");

	m_pBoxCollider = CBoxCollider::Create_Tool(m_pGraphicDevice, tObjInfo.vColliderCenter, tObjInfo.vColliderSize, tObjInfo.dwColliderColor, this);

	return S_OK;
}

HRESULT CMonsterNormalRange::Set_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CMonsterNormalRange::Set_RenderState - m_pGraphicDevice == nullptr");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE)))
		MSG_FAIL("CMonsterNormalRange::Set_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 150)))
		MSG_FAIL("CMonsterNormalRange::Set_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER)))
		MSG_FAIL("CMonsterNormalRange::Set_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");

	return S_OK;
}

HRESULT CMonsterNormalRange::Release_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CMonsterNormalRange::Set_RenderState - m_pGraphicDevice == nullptr");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE)))
		MSG_FAIL("CMonsterNormalRange::Set_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");

	return S_OK;
}

void CMonsterNormalRange::AI()
{
	m_pTransformPlayer = CPlayerTagMgr::Get_Instance()->Get_CurPlayerTransform();
	if (m_pTransformPlayer == nullptr)
		MSG_VOID("CMonsterNormalRange::AI - m_pTransformPlayer == nullptr");

	_float fDistance = m_pTransform->Get_DistanceTarget(m_pTransformPlayer);

	if (fDistance <= 20.f)
		Set_State(MONSTER_FSM::FSM_MOVE);
	else
		Set_State(MONSTER_FSM::FSM_IDLE);

	if (m_eCurState == MONSTER_FSM::FSM_MOVE)
	{
		if (fDistance <= 10.f)
		{
			Set_State(MONSTER_FSM::FSM_ATTACK);
			m_bStateLock = true;
		}
	}

	if (Check_Dead())
	{
		m_bStateLock = false;
		Set_State(MONSTER_FSM::FSM_DIE);
		m_bStateLock = true;
	}
}

void CMonsterNormalRange::Check_Frame()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case Client::CMonsterNormalRange::FSM_IDLE:
			m_tFrame.Set_Value(1);
			break;
		case Client::CMonsterNormalRange::FSM_MOVE:
			m_tFrame.Set_Value(4);
			break;
		case Client::CMonsterNormalRange::FSM_ATTACK:
			m_tFrame.Set_Value(8);
			break;
		case Client::CMonsterNormalRange::FSM_DIE:
			m_tFrame.Set_Value(3);
			break;
		case Client::CMonsterNormalRange::FSM_HIT:
			m_tFrame.Set_Value(2);
			m_tTimeHit.fDelay = 0.5f;
			break;
		}

		m_ePreState = m_eCurState;
	}
}

void CMonsterNormalRange::Move(_float fTimeDelta)
{
	if (m_pTransformPlayer == nullptr ||
		m_pTransform == nullptr)
		MSG_VOID("CMonsterNormalRange::Move - null");

	if (m_eCurState != MONSTER_FSM::FSM_MOVE)
		return;

	m_pTransform->Chase_Target(m_pTransformPlayer, fTimeDelta, 1.f);
}

void CMonsterNormalRange::Hit(_float fTimeDelta)
{
	if (m_eCurState != FSM_HIT)
		return;

	m_tTimeHit.Tick_Time(fTimeDelta);
	if (m_tTimeHit.Check_Time())
	{
		m_tTimeHit.fTime = 0.f;
		Set_Idle();
		Check_Frame();
	}
}

void CMonsterNormalRange::Tick_Frame(_float fTimeDelta)
{
	m_tFrame.fTime += fTimeDelta;
	if (m_tFrame.fTime >= m_tFrame.fDelay)
	{
		m_tFrame.fTime = 0.f;
		m_tFrame.iStart++;

		if (m_tFrame.iStart >= m_tFrame.iEnd)
		{
			m_tFrame.iStart = 0;

			if (m_eCurState == FSM_ATTACK)
			{
				Set_Idle();
				CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

				//!!!!!현재 레벨을 받아와야하나? 게임 플레이 레벨에서만 쓰면 상관없지만 다른 레벨에서 쓸때 고민
				CMagicMissile::DESC tMagicMissileDesc;
				tMagicMissileDesc.vPos = m_pTransform->Get_State(CTransform::STATE_POS);
				tMagicMissileDesc.vTargetPos = m_pTransformPlayer->Get_State(CTransform::STATE_POS);
				pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pTagPrototypeMagicMissile , pTagLayerMonsterAttack, &tMagicMissileDesc);
				RELEASE_INSTANCE(CGameInstance);
			}

			if (m_eCurState == FSM_DIE)
			{
				Set_Dead();
				m_tFrame.iStart = m_tFrame.iEnd - 1;
			}
		}
	}
}

_bool CMonsterNormalRange::Picking(const _vec3 & vMouseRay, const _vec3 & vMousePivot, CTransform * pOut)
{
	_vec3 vPos;
	if (m_pVIBuffer->Picking(vMouseRay, vMousePivot, &vPos))
	{
		pOut = dynamic_cast<CTransform*>(Get_Component(pTagComTransform));
		return true;
	}
	return false;
}

CMonsterNormalRange * CMonsterNormalRange::Create(_pDevice pDevice)
{
	CMonsterNormalRange* pInstance = new CMonsterNormalRange(pDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("CMonsterNormalRange Create Failed");
	}

	return pInstance;
}

void CMonsterNormalRange::Free()
{
	Safe_Release(m_pBoxCollider);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pTransform);
	Safe_Release(m_pVIBuffer);

	for (auto& pTexture : m_pTextureArr)
		Safe_Release(pTexture);

	__super::Free();
}

CGameObject * CMonsterNormalRange::Clone(void * pArg)
{
	CGameObject* pInstance = new CMonsterNormalRange(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("CMonsterNormalRange Clone Failed");
	}

	return pInstance;
}

_vec3 CMonsterNormalRange::Get_MinPos()
{
	_vec3 vMinPos = m_pBoxCollider->Get_MinPos();

	return vMinPos;
}

_vec3 CMonsterNormalRange::Get_MaxPos()
{
	_vec3 vMaxPos = m_pBoxCollider->Get_MaxPos();

	return vMaxPos;
}

_vec3 CMonsterNormalRange::Get_WorldPos()
{
	return m_pBoxCollider->Get_ColliderWolrdPos();
}

_vec3 CMonsterNormalRange::Get_Length()
{
	return m_pTransform->Get_Scale();
}

void CMonsterNormalRange::Collision(CGameObject * pColObj)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CAttack* pAttack = dynamic_cast<CAttack*>(pColObj);

	if (pAttack != nullptr)
	{
		//광역 공격이거나 아직 공격하지 않은 Attack 객체
		if (!pAttack->Get_SingleAttack() || !pAttack->Get_Attack())
		{
			_uint iDmg = pAttack->Get_Dmg();
			//데미지 있으면 체력 감소
			if (iDmg > 0)
				Sub_Hp(iDmg);

			m_bStateLock = false;
			Set_State(FSM_HIT);
			m_bStateLock = true;
			m_tFrame.iStart = 0;
		}

		pAttack->Set_Dead();
	}

	RELEASE_INSTANCE(CGameInstance);
}

const _vec3 & CMonsterNormalRange::Get_BoxColliderCenter() const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_pBoxCollider->Get_ColliderCenter();
}

void CMonsterNormalRange::Set_PoisonHitTransform(CPoisonCubeHit::DESC * pDesc)
{
	memcpy(&pDesc->tObjInfo.matWorld, m_pTransform->Get_WorldMatrixPtr(), sizeof(_mat));
}
